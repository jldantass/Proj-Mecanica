#include "trelica.h"
#include <stdio.h>

int main() {
    // definir nós
    No nos[3] = {
        {2.0, 0.0, 0.0, 0.0},   // nó 1 (livre)
        {0.0, 1.0, 0.0, 0.0},   // nó 2 (fixo)
        {0.0, 0.0, 0.0, 0.0}    // nó 3 (fixo)
    };

    // definir barras (quadrado vazado: 12mm externo, 9mm interno)
    Barra barras[2];
    float a_externo = 0.012, a_interno = 0.009;
    float area = 4.0 * (a_externo * a_externo - a_interno * a_interno); // 63e-6 m²
    float I = (pow(a_externo, 4) - pow(a_interno, 4)) / 12.0;           // 738.75e-12 m⁴

    // ligamento dos nós
    barras[0].no1 = 0;  // diagonal (nó 1 ao nó 2)
    barras[0].no2 = 1;

    barras[1].no1 = 0;  // base (nó 1 ao nó 3)
    barras[1].no2 = 2;

    // atreibuir área e I (momento de inercia)
    for (int i = 0; i < 2; i++) {
        barras[i].area = area;
        barras[i].I = I;
    }

    // calcular comprimentos das barras
    for (int i = 0; i < 2; i++) {
        calcular_comprimento(&barras[i], nos);
    }

    // calcular as forças e coeficientes antes da otimização
    calcular_forcas_por_equilibrio(barras, nos);

    for (int i = 0; i < 2; i++) {
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);
    }

    // calcula massa antes da otimização para ver a reduçao no final
    float massa_inicial = 0;
    for (int i = 0; i < 2; i++) {
        massa_inicial += 2800.0 * barras[i].area * barras[i].L;
    }

    printf("\n--- ANTES DA OTIMIZACAO ---\n");
    imprimir_resultados(nos, 3, barras, 2);

    // otimiza as areas das barras
    otimizar_trelica(barras, 2);

    // recalca as forças e coeficientes depois da otimização
    calcular_forcas_por_equilibrio(barras, nos);
    for (int i = 0; i < 2; i++) {
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);
    }

    // calcula massa depois otimização para ver a reduçao no final
    float massa_final = 0;
    for (int i = 0; i < 2; i++) {
        massa_final += 2800.0 * barras[i].area * barras[i].L;
    }

    printf("\n--- APOS OTIMIZACAO ---\n");
    imprimir_resultados(nos, 3, barras, 2);

    // compara as massas inicial e final
    float reducao_percentual = ((massa_inicial - massa_final) / massa_inicial) * 100.0;
    printf("\nMassa inicial: %.4f kg", massa_inicial);
    printf("\nMassa final:   %.4f kg", massa_final);
    printf("\nReducao de massa: %.2f%%\n", reducao_percentual);

    return 0;
}
