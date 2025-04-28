#ifndef TRELICA_H
#define TRELICA_H

#include <math.h>

// Constantes
#define PI 3.14159
#define E 70e9          // módulo de elasticidade
#define Sy 70e6         // tensão de escoamento
#define F 1000          // força aplicada
#define N_TENSAO 2      // coef. segurança tensão
#define N_FLAMBAGEM 1.2 // coef. segurança flambagem

// Estruturas
typedef struct {
    float x, y;
    float desloc_x, desloc_y;
} No;

typedef struct {
    int no1, no2;
    float area, I, L;
    float forca;
    float coef_seguranca_tensao;
    float coef_seguranca_flambagem;
} Barra;

// Funções
void calcular_comprimento(Barra *barra, No nos[]);
void calcular_forcas_por_equilibrio(Barra barras[], No nos[]);
void resolver_sistema_2x2(float A[2][2], float b[2], float x[2]);
void verificar_tensao(Barra *barra);
void verificar_flambagem(Barra *barra);
void imprimir_resultados(No nos[], int num_nos, Barra barras[], int num_barras);
void otimizar_trelica(Barra barras[], int num_barras);
float recalcular_I(float area);
void calcular_deslocamentos(Barra barras[], No nos[], int num_barras);

#endif
