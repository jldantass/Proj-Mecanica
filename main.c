#include "trelica.h"
#include <stdio.h>

int main() {
    // Definir nós (exemplo: treliça em "V" com 3 nós)
    No nos[3] = {
        {0.0, 0.5, 0.0, 0.0},   // Nó 1 (livre, carga aplicada aqui)
        {-0.5, 0.0, 0.0, 0.0},  // Nó 2 (fixo)
        {0.5, 0.0, 0.0, 0.0}    // Nó 3 (fixo)
    };

    // Definir barras (seção quadrada vazada: 12mm externo, 9mm interno)
    Barra barras[2];
    double a_externo = 0.012, a_interno = 0.009;
    double area = a_externo * a_externo - a_interno * a_interno; // 63e-6 m²
    double I = (pow(a_externo, 4) - pow(a_interno, 4)) / 12.0;  // 738.75e-12 m⁴

    // Inicializar barras
    for (int i = 0; i < 2; i++) {
        barras[i].no1 = 0;       // Conecta ao nó 1 (central)
        barras[i].no2 = i + 1;   // Conecta aos nós 2 e 3
        barras[i].area = area;
        barras[i].I = I;
    }

    // Calcular comprimentos
    for (int i = 0; i < 2; i++) {
        calcular_comprimento(&barras[i], nos);
    }

    // Calcular forças axiais
    calcular_forcas_axiais(barras, nos, 2);

    // Verificar tensão e flambagem
    for (int i = 0; i < 2; i++) {
        verificar_tensao(&barras[i]);
        verificar_flambagem(&barras[i]);
    }

    // Imprimir resultados
    imprimir_resultados(nos, 3, barras, 2);

    return 0;
}