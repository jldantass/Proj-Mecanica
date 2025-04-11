#include "trelica.h"
#include <stdio.h>

// calcula o comprimento da barra usando as coord. dos nós
// L = raiz( (x2-x1)² - (y2-y1)² )
// precisa para calcular a flambagem, massa da barra e direçao dos vetores
void calcular_comprimento(Barra *barra, No nos[]) {
    float dx = nos[barra->no2].x - nos[barra->no1].x;
    float dy = nos[barra->no2].y - nos[barra->no1].y;
    barra->L = sqrt(dx*dx + dy*dy);
}

// resolve sistema 2x2 linear: a*x = b
void resolver_sistema_2x2(float A[2][2], float b[2], float x[2]) {
    float det = A[0][0]*A[1][1] - A[0][1]*A[1][0];

    x[0] = (b[0]*A[1][1] - b[1]*A[0][1]) / det; // x0 = (b0*a11 - b1*a01)/det
    x[1] = (A[0][0]*b[1] - A[1][0]*b[0]) / det; // x0 = (b00*a1 - b10*a0)/det
}

void calcular_forcas_por_equilibrio(Barra barras[], No nos[]) {
    // identifica os nós
    int no_movel = 0; // inf direito (movel)
    int no1 = barras[0].no2; // sup esquerdo (fixo)
    int no2 = barras[1].no2; // inf esquerdo (fixo)

    // vetores barra 0
    float dx0 = nos[no1].x - nos[no_movel].x;
    float dy0 = nos[no1].y - nos[no_movel].y;
    float L0 = sqrt(dx0*dx0 + dy0*dy0);
    float cos0 = dx0 / L0;
    float sin0 = dy0 / L0;

    // vetores barra 1
    float dx1 = nos[no2].x - nos[no_movel].x;
    float dy1 = nos[no2].y - nos[no_movel].y;
    float L1 = sqrt(dx1*dx1 + dy1*dy1);
    float cos1 = dx1 / L1;
    float sin1 = dy1 / L1;

    // montar sistema
    float A[2][2] = {
        {cos0, cos1},
        {sin0, sin1}
    };
    float b[2] = {0, -F};

    // forças nas barras
    float f[2]; 
    resolver_sistema_2x2(A, b, f);

    barras[0].forca = f[0];
    barras[1].forca = f[1];
}

// verifica se a tensao ta dentro do limite
void verificar_tensao(Barra *barra) {
    float sigma = fabs(barra->forca) / barra->area; // (SIGMA = |F|/A)
    barra->coef_seguranca_tensao = Sy / sigma; // COEF. SEG. TENSAO= (TENSAO DE ESCOAMENTO/SIGMA)
}

// verifica se a flambagem ta dentro do limite
void verificar_flambagem(Barra *barra) {
    // se a barra estiver em compressao (força < 0)
    if (barra->forca < 0) {
        float P_cr = (PI * PI * E * barra->I) / pow(barra->L, 2); // Pcr = (PI² * E * I) / L²
        barra->coef_seguranca_flambagem = P_cr / fabs(barra->forca); // COEF. SEG. FLAMBAGEM = Pcr / |F|
    } else {
        barra->coef_seguranca_flambagem = INFINITY; // nao aplica flambagem se estiver em traçao (força > 0)
    }
}

// recalcula o momento de inércia (I) baseado na nova area
float recalcular_I(float area) {
    // considera a seção quadrada vazada proporcional: A = a_ext² - a_int²
    // assumindo que a_int = 0.75 * a_ext, como no exemplo original
    float a_ext = sqrt(area / (1 - 0.75 * 0.75));
    float a_int = 0.75 * a_ext;
    return (pow(a_ext, 4) - pow(a_int, 4)) / 12.0; // I = (a_ext^4 - a_int^4) / 12
}

// funçao para otimizar as areas das barras
// reduz progressivamente (10%) a area de cada barra ate atingir o limite min. permitido de segurança
void otimizar_trelica(Barra barras[], int num_barras) {
    float area_minima = 0.00001;

    for (int i = 0; i < num_barras; i++) {
        // verifica os coeficientes antes de entrar no loop do while
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);

        while (1) {
            // diminui a area e recalcula
            barras[i].area *= 0.9;
            barras[i].I = recalcular_I(barras[i].area);
            verificar_tensao(&barras[i]);
            verificar_flambagem(&barras[i]);

            // verifica se passou dos limites permitidos
            if (barras[i].coef_seguranca_tensao <= N_TENSAO ||
                barras[i].coef_seguranca_flambagem <= N_FLAMBAGEM ||
                barras[i].area <= area_minima) {
                // reverte a ultima reduçao ja que passou do limites
                barras[i].area /= 0.887;
                barras[i].I = recalcular_I(barras[i].area);
                verificar_tensao(&barras[i]);
                verificar_flambagem(&barras[i]);
                break;
            }
        }
    }
}

// imprimes os resultados
void imprimir_resultados(No nos[], int num_nos, Barra barras[], int num_barras) {
    printf("\n=== DESLOCAMENTOS NODAIS ===\n");
    printf("| No | Desloc. X     | Desloc. Y \n");
    for (int i = 0; i < num_nos; i++) {
        printf("| %-2d | %-13.6f | %-13.6f\n", i + 1, nos[i].desloc_x, nos[i].desloc_y);
    }

    printf("\n=== FORCAS E COEF. DE SEGURANCA ===\n");
    printf("| Barra | Forca     | Coef. Tensao  | Coef. Flambagem\n");
    for (int i = 0; i < num_barras; i++) {
        printf("| %-5d | %-9.2f | %-13.2f | ",
               i + 1, barras[i].forca, barras[i].coef_seguranca_tensao);

        if (isinf(barras[i].coef_seguranca_flambagem)) {
            printf("%-15s\n", "N/A (tracionada)");
        } else {
            printf("%-15.2f\n", barras[i].coef_seguranca_flambagem);
        }
    }

    float massa = 0;
    for (int i = 0; i < num_barras; i++) {
        massa += 2800.0 * barras[i].area * barras[i].L;
    }

    printf("\nMassa total da trelica: %.4f kg\n", massa);

}
