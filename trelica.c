#include "trelica.h"
#include <stdio.h>

// Calcula o comprimento da barra usando as coordenadas dos nós
void calcular_comprimento(Barra *barra, No nos[]) {
    float dx = nos[barra->no2].x - nos[barra->no1].x;
    float dy = nos[barra->no2].y - nos[barra->no1].y;
    barra->L = sqrt(dx*dx + dy*dy);
}

// Calcula as forças axiais nas barras (Método dos Nós)
void calcular_forcas_axiais(Barra barras[], No nos[], int num_barras) {
    // Ângulo da barra 1 (assumindo simetria)
    float theta = atan2(nos[1].y - nos[0].y, nos[1].x - nos[0].x);
    
    // Força em cada barra (equilíbrio no nó 1)
    float forca = (F / 2) / sin(theta);
    
    for (int i = 0; i < num_barras; i++) {
        barras[i].forca = forca;
    }
}

// Verifica se a tensão está dentro do limite admissível
void verificar_tensao(Barra *barra) {
    float sigma = fabs(barra->forca) / barra->area; // Tensão absoluta
    float sigma_adm = Sy / N_TENSAO;
    barra->coef_seguranca_tensao = Sy / sigma;
}

// Verifica risco de flambagem (se a barra está sob compressão)
void verificar_flambagem(Barra *barra) {
    if (barra->forca < 0) { // Só se for compressão
        float P_cr = (PI * PI * E * barra->I) / pow(barra->L, 2);
        barra->coef_seguranca_flambagem = P_cr / fabs(barra->forca);
    } else {
        barra->coef_seguranca_flambagem = INFINITY; // Não aplicável
    }
}

// Imprime resultados em tabelas formatadas
void imprimir_resultados(No nos[], int num_nos, Barra barras[], int num_barras) {
    printf("\n=== DESLOCAMENTOS NODAIS ===\n");
    printf("No | Desloc. X (m) | Desloc. Y (m)\n");
    for (int i = 0; i < num_nos; i++) {
        printf("%d  | %-12.6f | %-12.6f\n", i+1, nos[i].desloc_x, nos[i].desloc_y);
    }

    printf("\n=== FORCAS E COEF. DE SEGURANCA ===\n");
    printf("Barra | Forca (N) | Coef. Tensao | Coef. Flambagem\n");
    for (int i = 0; i < num_barras; i++) {
        printf("%-5d | %-9.2f | %-12.2f | %-15.2f\n", 
               i+1, barras[i].forca, barras[i].coef_seguranca_tensao,
               barras[i].coef_seguranca_flambagem);
    }
}