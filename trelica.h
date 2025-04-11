#ifndef TRELICA_H
#define TRELICA_H

#include <math.h>

// defindo as constantes 
#define PI 3.14159
#define E 70e9          // modulo de elasticidade longitudinal 
#define Sy 70e6         // tensao de escoamento 
#define F 1000          // força aplicada no nó 1 
#define N_TENSAO 2      // coef de segurança p/ tensao
#define N_FLAMBAGEM 1.2 // coef de segurança p/ flambagem

// struct p/ nó
typedef struct {
    float x, y;                 // coordenadas do nó
    float desloc_x, desloc_y;   // deslocamentos do nó
} No;

// struct p/ barra
typedef struct {
    int no1, no2;                    // nós
    float area;                      // área (m²)
    float I;                         // momento de inercia
    float L;                         // comp da barra
    float forca;                     // força 
    float coef_seguranca_tensao;     // coef de segurança p/ tensao
    float coef_seguranca_flambagem;  // coef de segurança p/ flambagem 
} Barra;

// funçoes
void calcular_comprimento(Barra *barra, No nos[]);
void calcular_forcas_por_equilibrio(Barra barras[], No nos[]);
void resolver_sistema_2x2(float A[2][2], float b[2], float x[2]);
void verificar_tensao(Barra *barra);
void verificar_flambagem(Barra *barra);
void imprimir_resultados(No nos[], int num_nos, Barra barras[], int num_barras);
void otimizar_trelica(Barra barras[], int num_barras);
float recalcular_I(float area);

#endif
