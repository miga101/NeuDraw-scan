
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
char gListaSym[10][15];

void generate_random_weights();
void phase_1(int arrSym[]);
void phase_2(int symbol_index);
void phase_3();
void phase_4(int arrSym[]);
void SalvarPesos(void);
void train (int IDs, int arrSym[], float gSzGrid, struct s_punto scnGrafVx[]);
void train2 (int IDs, int arrSym[], float gSzGrid);
void forward_pass1();
void forward_pass2();
void BeginTraining(void);
int MatchPattern(int *arrSym, float *w);
void LeerPesos(void);
void MapWeight(float gSzGrid, int out_my, struct s_punto scnGrafVx[]);
void DrawNeuronas(float x, float y, float gSzGrid);
void MapOUT(char *buf[]);
void trainScans(int IDs, char lstSym[10][15], float gSzGrid);
int buscIDvx(char nomb[15]);

