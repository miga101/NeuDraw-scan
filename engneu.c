#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "engneu.h"

/*Neural Network definitions*/
#define SYMBOL_SIZE		   (144)//***36-256
#define TOTAL_OUTPUT	   (10)
#define TOTAL_SYMBOLS	   (10)
#define TOTAL_HIDDEN	   (144)//50
#define TOTAL_INPUT		   (SYMBOL_SIZE)
#define sigmoid(x)		   (1/(1+ (1/ (exp (-1*x) ) ) ))
float input_weight[TOTAL_INPUT][TOTAL_HIDDEN];
float hidden_weight[TOTAL_HIDDEN][TOTAL_OUTPUT];
float out_hidden[TOTAL_HIDDEN];
float out_output[TOTAL_OUTPUT];
float delta_hidden[TOTAL_HIDDEN];
float delta_output[TOTAL_OUTPUT];
float net_hidden[TOTAL_HIDDEN];
float net_output[TOTAL_OUTPUT];

int gIDsym = 0;
char gNomb[15];

int Apren_Symbol[SYMBOL_SIZE];	// SalidaCua  
int Itetrain = 1000;//numero de entranamientos
struct s_punto {
	int x;
	int y;
	int prc;
};//***36 NeuIN	//SampPT

float target_output[TOTAL_OUTPUT][TOTAL_SYMBOLS]={
	{1,0,0,0,0,0,0,0,0,0},
	{0,1,0,0,0,0,0,0,0,0},
	{0,0,1,0,0,0,0,0,0,0},
	{0,0,0,1,0,0,0,0,0,0},
	{0,0,0,0,1,0,0,0,0,0},
	{0,0,0,0,0,1,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,1,0,0},
	{0,0,0,0,0,0,0,0,1,0},
	{0,0,0,0,0,0,0,0,0,1}
};
/*These are the bitmaps representing 0-9
  A 5x7 bitmap is used to give 35 total bits.
  Generally larger bitmaps give more accuracy
*/
int   symbols[TOTAL_SYMBOLS][SYMBOL_SIZE];

void MapOUT(char *buf[])
{
int i;
float x, y;
x = 500;
y = 100;
	glColorMask(1, 1, 1, 1);
	for(i=0; i < TOTAL_SYMBOLS; i++){
	glPushMatrix();
		if ((out_output[i]>0.0) && (out_output[i]<1.0))
			glColor3f(out_output[i], 0.0, 0.0);
		else
			glColor3f(0.2, 0.2, 0.2);
		DrawNeuronas(x, y+=25, 20);
		glColor3f(1, 1, 1);
	glPopMatrix();
	}
	//glColorMask(0, 1, 0, 1);
}

void DrawNeuronas(float x, float y, float gSzGrid)
{
   glBegin (GL_QUADS);
      glVertex3f(x, y, 0.0); 
      glVertex3f(x, y + gSzGrid, 0.0); 
      glVertex3f(x + gSzGrid, y + gSzGrid, 0.0); 
      glVertex3f(x + gSzGrid, y, 0.0); 
   glEnd();
}

void MapWeight(float gSzGrid, int out_my, struct s_punto scnGrafVx[SYMBOL_SIZE])
{
int i;

	for(i=0; i < SYMBOL_SIZE; i++){
	glPushMatrix();
		if ((out_hidden[i]>=0.0) && (out_hidden[i]<=1.0))
			glColor3f(hidden_weight[i][out_my], 0.0, 0.0);
		else
			glColor3f(0.2, 0.2, 0.2);
		DrawNeuronas(scnGrafVx[i].x-gSzGrid/2, scnGrafVx[i].y-gSzGrid/2, gSzGrid);
	glPopMatrix();
	}
	glColorMask(0, 1, 0, 1);
}

void generate_random_weights()
{
	float z;
	int i,j;
	srand(25);//Should use the timer here but I was too lazy!
	z=2.5/RAND_MAX;
	for (i=0;i<TOTAL_INPUT;++i)
		for (j=0;j<TOTAL_HIDDEN;++j)
			input_weight[i][j]=(rand()*z)-1.25;
	for (i=0;i<TOTAL_HIDDEN;++i)
		for (j=0;j<TOTAL_OUTPUT;++j)
			hidden_weight[i][j]=(rand()*z)-1.25;

}

/*These are the different phases in the training*/
/*See the acompanying text file for more info*/
void  phase_1(int arrSym[])
{
	int middlenode,inputnode;
	for (middlenode=0;middlenode<TOTAL_HIDDEN;++middlenode)
	{
		net_hidden[middlenode]=0;
		for (inputnode=0;inputnode<TOTAL_INPUT;++inputnode)
		{
			net_hidden[middlenode]+=input_weight[inputnode][middlenode]*arrSym[inputnode];
		}
		out_hidden[middlenode]=sigmoid(net_hidden[middlenode]);
	}

}

void phase_2(int symbol_index)
{
int middlenode,outputnode;
	for (outputnode=0;outputnode<TOTAL_OUTPUT;++outputnode)
	{
		net_output[outputnode]=0;
		for (middlenode=0;middlenode<TOTAL_HIDDEN;++middlenode)
		{
			net_output[outputnode]+=hidden_weight[middlenode][outputnode]*out_hidden[middlenode];
		}
		out_output[outputnode]=sigmoid(net_output[outputnode]);
		delta_output[outputnode]=out_output[outputnode]*(1-out_output[outputnode])
			*(target_output[symbol_index][outputnode]-out_output[outputnode]);
	}
}

void phase_3()
{
int outputnode,middlenode;
	float delta_sum;
	for (middlenode=0;middlenode<TOTAL_HIDDEN;++middlenode)
	{
		delta_sum=0;
		for (outputnode=0;outputnode<TOTAL_OUTPUT;++outputnode)
		{
			delta_sum+=delta_output[outputnode]*hidden_weight[middlenode][outputnode];
		}
		hidden_weight[middlenode][outputnode]+=delta_output[outputnode]*out_hidden[middlenode];
		delta_hidden[middlenode]=delta_sum*out_hidden[middlenode]*(1-out_hidden[middlenode]);
	}
}

void phase_4(int arrSym[])
{
	int middle_node,input_node;
	for (input_node=0;input_node<TOTAL_INPUT;++input_node)
	{
		for (middle_node=0;middle_node<TOTAL_HIDDEN;++middle_node)
		{
			input_weight[input_node][middle_node]+=delta_hidden[middle_node]*arrSym[input_node];
		}
	}
}

void SalvarPesos(void)
{
FILE *fp;
int i, j;

fp = fopen("data/pesos.dat","w");
//fprintf(fp,"*Pesos de input_weight: %dx%d", TOTAL_INPUT, TOTAL_HIDDEN);
	for(i=0; i<TOTAL_INPUT; i++) 
		for( j=0; j<TOTAL_HIDDEN; j++){
			fprintf(fp,"%f\n", input_weight[i][j]);
		}
//fprintf(fp,"\n*Pesos de hidden_weight: %dx%d", TOTAL_HIDDEN, TOTAL_OUTPUT);
	for(i=0; i<TOTAL_HIDDEN; i++) 
		for( j=0; j<TOTAL_OUTPUT; j++){
			fprintf(fp,"%f\n", hidden_weight[i][j]);
		}
fclose(fp);
}

void LeerPesos(void)
{
FILE *fp;
int i, j;

	fp = fopen("data/pesos.dat","a+");
	fscanf(fp,"\n");
	for(i=0; i<TOTAL_INPUT; i++) 
		for( j=0; j<TOTAL_HIDDEN; j++){
			fscanf(fp,"%f",&input_weight[i][j]);
		}
	
	for(i=0; i<TOTAL_HIDDEN; i++) 
		for( j=0; j<TOTAL_OUTPUT; j++){
			fscanf(fp,"%f",&hidden_weight[i][j]);
		}
fclose(fp);
}
/*void train2 (float gSzGrid, struct s_punto scnGrafVx[SYMBOL_SIZE])
{
int *arrSym[],i;
	

	while (LeerScans(&IDs, &arrSym) == 1){
		for (i=0; i<Itetrain; ++i){		
			if ((out_output[IDs]>0.985)&&(i>(Itetrain*0.35))) break;
			phase_1(arrSym);
			phase_2(IDs);
			phase_3();
			phase_4(arrSym);
		}
	}
}*/
void train (int IDs, int arrSym[], float gSzGrid, struct s_punto scnGrafVx[SYMBOL_SIZE])
{
int i;
	
	SalavarScan(IDs);	//para reforzar todo el entrenamiento de la red
	for (i=0; i<Itetrain; ++i)
	{		
		if ((out_output[IDs]>0.985)&&(i>(Itetrain*0.35))) break;
		phase_1(arrSym);
		phase_2(IDs);
		phase_3();
		phase_4(arrSym);
		/*glColorMask(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_DEPTH_BUFFER_BIT);
		MapWeight(gSzGrid, IDs, scnGrafVx);*/
	}
}

void train2 (int IDs, int arrSym[], float gSzGrid)
{
int i;
	
		for (i=0; i<Itetrain; ++i){		
			if ((out_output[IDs]>0.985)&&(i>(Itetrain*0.35))) break;
			phase_1(arrSym);
			phase_2(IDs);
			phase_3();
			phase_4(arrSym);
		}
}

int buscIDvx(char nomb[15])  
{
int i;
	for (i = 0;gListaSym[i][0];i++){
		if (strcmp(nomb, gListaSym[i])== 0){
			return i;
			break;
		}
	}
	return -1;// error: no debe pasar nunca!
}

void trainScans(int IDs, char lstSym[10][15], float gSzGrid)
{
FILE *fp;
char NomS[15];
int i, j, vIDs, nldsMtx;
int vextor[SYMBOL_SIZE];


	SalavarScan(IDs);	//salva el nuevo vextor
	nldsMtx = sqrt(SYMBOL_SIZE);
	fp = fopen("data/data.dat","a+");
	for (i = 0;!feof(fp);i++){	// por cada vextor en fp
		fscanf(fp,"%s ", &NomS);	// nombre del vextor
		vIDs = buscIDvx(NomS);
		for( j=0; j<SYMBOL_SIZE; j++){// cargando vextor(j)
			fscanf(fp,"%d ", &vextor[j]);
			if (((j+1) % nldsMtx == 0) && (j!=0)) 
				fscanf(fp,"\n");// creo q lee la prim de la fila j++
		}
		train2(vIDs, vextor, gSzGrid);	// entrena la red con el vextor actual
	}
	fclose(fp);
}

void  forward_pass1(int arrSym[])
{
	int middlenode,inputnode;
	for (middlenode=0;middlenode<TOTAL_HIDDEN;++middlenode)
	{
		net_hidden[middlenode]=0;
		for (inputnode=0;inputnode<TOTAL_INPUT;++inputnode)
		{
			net_hidden[middlenode]+=input_weight[inputnode][middlenode]*arrSym[inputnode];
		}
		out_hidden[middlenode]=sigmoid(net_hidden[middlenode]);
	}

}

/*
Procedure #2 for recognising a pattern
Almost the same as the phase_2 routine but uses
the test_pattern array instead
*/
void forward_pass2()
{
	int middlenode,outputnode;
	for (outputnode=0;outputnode<TOTAL_OUTPUT;++outputnode)
	{
		net_output[outputnode]=0;
		for (middlenode=0;middlenode<TOTAL_HIDDEN;++middlenode)
		{
			net_output[outputnode]+=hidden_weight[middlenode][outputnode]*out_hidden[middlenode];
		}
		out_output[outputnode]=sigmoid(net_output[outputnode]);
	}
}

void BeginTraining(void)
{

	//LeerBitmap(TOTAL_SYMBOLS, SYMBOL_SIZE);
	//iterations=GetIterationCount(hwndDlg);

	//train(Itetrain);
}


/*After sending a pattern through, this routine
  finds the highest output and tells you what
  the number is
*/
int MatchPattern(int *arrSym, float *w)
{
int i, c, mtch;
float m;
char buffer[6];
char peso[15];

	forward_pass1(arrSym);
	forward_pass2();
	for (i=0,m=0,c=0;i<TOTAL_OUTPUT;++i)
	{
		if (m<out_output[i])
		{
			m=out_output[i];// busca el maximo
			c=i;
		}
	}
	_itoa(c,buffer,10);
	*w = m;
	gcvt(m,3,peso);
	TexPantalla(peso, 10, 20);
	if (m <= 0.50){//		NO RECONOCIDO
		mtch = -1;
	}
	else{
		mtch = c;
	}
	return mtch;
}


