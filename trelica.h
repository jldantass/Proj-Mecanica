#ifndef TRELICA_H
#define TRELICA_H

#include <math.h>

// Constantes 
#define PI 3.14159
#define E 70e9          // Módulo de elasticidade longitudinal (Pa)
#define Sy 70e6         // Tensão de escoamento (Pa)
#define F 1000          // Força aplicada no nó 1 (N)
#define N_TENSAO 2      // Coeficiente de segurança (tensão)
#define N_FLAMBAGEM 1.2 // Coeficiente de segurança (flambagem)

// Struct pro nó
typedef struct {
    float x, y;                 // Coordenadas do nó
    float desloc_x, desloc_y;   // Deslocamentos do nó
} No;

// Struct pra barra
typedef struct {
    int no1, no2;                     // Nós
    float area;                      // Área da seção transversal (m²)
    float I;                         // Momento de inércia (m⁴)
    float L;                         // Comprimento da barra (m)
    float forca;                     // Força axial (N)
    float coef_seguranca_tensao;     // Coeficiente de segurança para tensão
    float coef_seguranca_flambagem;  // Coeficiente para flambagem 
} Barra;

// Funções
void calcular_comprimento(Barra *barra, No nos[]);
void calcular_forcas_axiais(Barra barras[], No nos[], int num_barras);
void verificar_tensao(Barra *barra);
void verificar_flambagem(Barra *barra);
void imprimir_resultados(No nos[], int num_nos, Barra barras[], int num_barras);
void otimizar_trelica(Barra barras[], int num_barras);
float recalcular_I(float area);

#endif
