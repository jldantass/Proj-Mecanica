#include "trelica.h"
#include <stdio.h>

int main() {
    // Definir nós (exemplo: treliça em "V" com 3 nós)
    No nos[3] = {
        {2.0, 0.0, 0.0, 0.0},   // Nó 1 (livre, carga aplicada aqui)
        {0.0, 1.0, 0.0, 0.0},  // Nó 2 (fixo)
        {0.0, 0.0, 0.0, 0.0}    // Nó 3 (fixo)
    };

    // Definir barras (seção quadrada vazada: 12mm externo, 9mm interno)
    Barra barras[2];
    float a_externo = 0.012, a_interno = 0.009;
    float area = 4.0 * (a_externo * a_externo - a_interno * a_interno); // 63e-6 m²
    float I = (pow(a_externo, 4) - pow(a_interno, 4)) / 12.0;    // 738.75e-12 m⁴

    // Conectividade correta das barras
    barras[0].no1 = 0;  // Diagonal (nó 1 ao nó 3)
    barras[0].no2 = 1;

    barras[1].no1 = 0;  // Base (nó 2 ao nó 3)
    barras[1].no2 = 2;

    // Atribuir área e I
    for (int i = 0; i < 2; i++) {
        barras[i].area = area;
        barras[i].I = I;
    }

    // Calcular comprimentos
    for (int i = 0; i < 2; i++) {
        calcular_comprimento(&barras[i], nos);
    }

    // Calcular forças axiais e coeficientes antes da otimização
    calcular_forcas_por_equilibrio(barras, nos);

    for (int i = 0; i < 2; i++) {
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);
    }

    // Calcular massa antes da otimização
    float massa_inicial = 0;
    for (int i = 0; i < 2; i++) {
        massa_inicial += 2800.0 * barras[i].area * barras[i].L;
    }

    printf("\n--- ANTES DA OTIMIZACAO ---\n");
    imprimir_resultados(nos, 3, barras, 2);

    // Otimizar áreas das barras
    otimizar_trelica(barras, 2);

    // Recalcular forças e coeficientes após otimização
    calcular_forcas_por_equilibrio(barras, nos);
    for (int i = 0; i < 2; i++) {
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);
    }

    // Calcular massa após otimização
    float massa_final = 0;
    for (int i = 0; i < 2; i++) {
        massa_final += 2800.0 * barras[i].area * barras[i].L;
    }

    printf("\n--- APOS OTIMIZACAO ---\n");
    imprimir_resultados(nos, 3, barras, 2);

    // Comparar massas
    float reducao_percentual = ((massa_inicial - massa_final) / massa_inicial) * 100.0;
    printf("\nMassa inicial: %.4f kg", massa_inicial);
    printf("\nMassa final:   %.4f kg", massa_final);
    printf("\nReducao de massa: %.2f%%\n", reducao_percentual);

    return 0;
}
