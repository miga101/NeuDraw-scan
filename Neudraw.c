#include <GL/glut.h>
#include <string.h>
#include "engneu.h"

#define ERROR_w			(0.95)// ERROR ACEPTADO

int mxX = 0;
int mnX = 32000;
int mxY = 0;
int mnY = 32000;
int pt = 0;
int TOTAL_SYMBOLS = 10;//		REPETIDA EN ENGNEU.C

//char gListaSym[10][15];

int vextor[144];	//***36 NeuIN Sqr(6)
int NeuIN = 144;	//***36 NeuIN
int ptSamp = 400;	//Puntos a samplear desde la pantalla
float SideMtrx;		
float ptBaseX, ptBaseY;
int nldsMtx, gNasc, gWmy_Ids;
int gmausbtlf = 0;
float gSzGrid;
float gWmy_w;


int gIDs = -1;
//char gNombS[15];

struct s_punto {
	int x;
	int y;
	int prc;
} scan[400], scnGrafVx[144];//***36 NeuIN	//SampPT


unsigned char	BlendBrush[7][7] =
{
  { 0xff, 0x00, 0xff, 0x00, 0xff, 0x08, 0xff},
  { 0xff, 0x00, 0xff, 0x08, 0xff, 0x10, 0xff },
  { 0xff, 0x08, 0xff, 0x10, 0xff, 0x20, 0xff },
  { 0xff, 0x10, 0xff, 0x20, 0xff, 0x40, 0xff },
  { 0xff, 0x08, 0xff, 0x10, 0xff, 0x20, 0xff },
  { 0xff, 0x00, 0xff, 0x08, 0xff, 0x10, 0xff },
  { 0xff, 0x00, 0xff, 0x00, 0xff, 0x08, 0xff},
};

int MtxOut[10] = {0,0,0,0,0,0,0,0,0,0};

void clrPantalla(void)
{
	glColorMask(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT);
}

void reshape(int w, int h)
{
  glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, w, h, 0.0, -1.0, 1.0);
   clrPantalla();
}

void DrawXY(int mousex,	int mousey)	
{
  glRasterPos2i(mousex, mousey);
  glDrawPixels(5, 5, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, BlendBrush[0]);
  scan[pt].x = mousex;
  scan[pt].y = mousey;
  scan[pt].prc = 0;
  pt++;
  glFinish();
}

void Pt_MinMax (int *X1, int *Y1, int *X2, int *Y2)
{
int i;

mxX = 0;
mnX = 32000;
mxY = 0;
mnY = 32000;
	for (i=0;i<pt;++i)
	{
		if (mxX<scan[i].x)
			mxX = scan[i].x;
		if (mnX>scan[i].x)
			mnX = scan[i].x;
		if (mxY<scan[i].y)
			mxY = scan[i].y;
		if (mnY>scan[i].y)
			mnY = scan[i].y;
	}
	*X1 = mnX;
	*Y1 = mnY;
	*X2 = mxX;
	*Y2 = mxY;
}

void TexPantalla(char *buf, int x, int y)
{
int len, i;
	glRasterPos2i(x, y);
	len = strlen(buf);
	for (i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, buf[i]);
}

void SalvarLstSymb()
{
FILE *fp;
int i;

	fp = fopen("data/lsSym.dat","w");
	for (i = 0;gListaSym[i][0];i++)
		fprintf(fp, "%s\n", gListaSym[i]);
	fclose(fp);
}

void LeerLstSymb()
{
FILE *fp;
int i;
	fp = fopen("data/lsSym.dat","a+");
	for (i = 0;!feof(fp);i++)
		fscanf(fp,"%s",&gListaSym[i]);
	fclose(fp);
	gIDs = i-2;
	//if (gIDs <= 0) gIDs = -1;
}

void SalavarScan(int id)
{
int i;
FILE *fp;

	fp = fopen("data/data.dat","a+");
	//fprintf(fp,"salida: %s", gListaSym[id]);
	fprintf(fp,"%s", gListaSym[id]);
	fprintf(fp,"\n");
	for( i=0; i<NeuIN; i++){
		fprintf(fp,"%d ",vextor[i]);
		if (((i+1) % nldsMtx == 0) && (i!=0)) fprintf(fp,"\n");
	}
	fclose(fp);
}

void scanVextor(void)
{
float Rng1x, Rng2x, Rng1y, Rng2y;
int p, i, j, encX, encY;
char op[1];

	nldsMtx = sqrt(NeuIN);
	gSzGrid = SideMtrx/nldsMtx;
	for(i=0;i<NeuIN;i++)
		vextor[i]=0;

	for (p=0;p < pt;p++) {
		Rng1x = ptBaseX;
		Rng2x = ptBaseX + gSzGrid;
		Rng1y = ptBaseY;
		Rng2y = ptBaseY + gSzGrid;
		for(i=0,encX=0; ((i < nldsMtx) && !(encX)); i++){
			if (((scan[p].x >= Rng1x) && (scan[p].x <= Rng2x))  && (!scan[p].prc))
				encX = 1;
			if (i >= nldsMtx) {
				Rng1x = ptBaseX;
				Rng2x = ptBaseX + gSzGrid;
			}else {
				Rng1x = Rng2x;
				Rng2x = Rng2x + gSzGrid;
			}
		}
		if (i==(nldsMtx + 1)){
			printf("\n ERROR: fuera de rango X");
			gets(op);
			exit(0);
		}

		for(j=0,encY=0; ((j < nldsMtx) && !(encY)); j++){
			if (((scan[p].y >= Rng1y) && (scan[p].y <= Rng2y))  && (!scan[p].prc))
				encY = 1;
			if (j >= nldsMtx) {
				Rng1y = ptBaseY;
				Rng2y = ptBaseY + gSzGrid;
			}else {
				Rng1y = Rng2y;
				Rng2y = Rng2y + gSzGrid;
			}
		}
		if (j==(nldsMtx + 1)){
			printf("\n ERROR: fuera de rango J");
			gets(op);
			exit(0);
		}

		vextor[(i-1) + ((j-1) * nldsMtx)] = 1;
		scan[p].prc = 1;
	}
}

void scanGraf(void)
{
float Rng1x, Rng2x, Rng1y, Rng2y;
int p, i, j;

	nldsMtx = sqrt(NeuIN);
	Rng1y = ptBaseY;
	Rng2y = ptBaseY + gSzGrid;
	p = 0;
		for(j=0; j < nldsMtx; j++){
			for(i=0; i < nldsMtx; i++){

				if (i == 0) {
					Rng1x = ptBaseX;
					Rng2x = ptBaseX + gSzGrid;
				}else {
					Rng1x = Rng2x;
					Rng2x = Rng2x + gSzGrid;
				}

				scnGrafVx[p].x = ((Rng2x - Rng1x)/2) + Rng1x;
				scnGrafVx[p].y = ((Rng2y - Rng1y)/2) + Rng1y;
				if (((scan[p].y >= Rng1y) && (scan[p].y <= Rng2y))  && ((scan[p].x >= Rng1x) && (scan[p].x <= Rng2x)))
					scnGrafVx[p].prc = 1;
				p++;

			}
			Rng1y = Rng2y;
			Rng2y = Rng2y + gSzGrid;
		}
}

void init(void)
{
   //glEnable (GL_BLEND);
   //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   glShadeModel (GL_FLAT);
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glColorMask(0, 0, 0, 1);
}

void DrawNeurona(float x, float y)
{
   glBegin (GL_QUADS);
      glColor4f(1.0, 1.0, 1.0, 0.0);
      glVertex3f(x, y, 0.0); 
      glVertex3f(x, y + gSzGrid, 0.0); 
      glVertex3f(x + gSzGrid, y + gSzGrid, 0.0); 
      glVertex3f(x + gSzGrid, y, 0.0); 
   glEnd();
}

void MapScan(void)
{
int i;
	for(i=0; i < NeuIN; i++){
	glPushMatrix();
		if (vextor[i])
			glColorMask(1, 0, 0, 1);
		else
			glColorMask(0, 0, 1, 1);
		DrawNeurona(scnGrafVx[i].x-gSzGrid/2, scnGrafVx[i].y-gSzGrid/2);
	glPopMatrix();
	}
	glColorMask(0, 1, 0, 1);
}

void marco(x1,y1,x2,y2)
{
float pmX, pmY, dX, dY, p1x, p1y, p2x, p2y;

	pmX = (x1 + x2)/2;
	pmY = (y1 + y2)/2;
	dX = x2 - x1;
	dY = y2 - y1;
	if (dX > dY){//	p1 = superior izq, p2 = inf derecho
		p1y = pmY - dX/2;
		p2y = pmY + dX/2;
		p1x = x1;
		p2x = x2;
		SideMtrx = dX;
	}else{
		p1x = pmX - dY/2;
		p2x = pmX + dY/2;
		p1y = y1;
		p2y = y2;
		SideMtrx = dY;
	}
	ptBaseX = p1x;
	ptBaseY = p1y;

	glPushMatrix();
	 glColorMask(0, 1, 0, 1);
	glBegin(GL_LINE_STRIP);
		glVertex2f(p1x,p1y);
		glVertex2f(p2x,p1y);
		glVertex2f(p2x,p2y);
		glVertex2f(p1x,p2y);
		glVertex2f(p1x,p1y);
	glEnd();
	glPopMatrix();
scanVextor();
scanGraf();
}

void keyboard(char key)
{
char nom[10];
int i, j;

  switch (key) {
  case 'n':
		gIDs++;
		clrPantalla();
		TexPantalla("Escriba el Nombre del Symbolo:...", 10, 470);
		printf("\n Nombre?: ");
		gets(gListaSym[gIDs]);
		clrPantalla();
		TexPantalla("Entrenando!...", 10, 470);
		//train(gIDs, vextor, gSzGrid, scnGrafVx);
		trainScans(gIDs, gListaSym, gSzGrid);
		SalvarPesos();
		clrPantalla();
    break;
  case 'a':
		glClear(GL_COLOR_BUFFER_BIT);
		TexPantalla("Seleccione de la lista:...", 10, 470);
		printf("\n lista: ");
		for (i = 0;gListaSym[i][0];i++)
			printf("\n %d: %s", i, gListaSym[i]);
		printf("\n ID = ");
		gets(nom);
		clrPantalla();
		TexPantalla("Entrenando!...", 10, 470);
		//gIDs = atoi(nom);//??????????
		//train2(gSzGrid, scnGrafVx);
		trainScans(atoi(nom), gListaSym, gSzGrid);
		SalvarPesos();
		clrPantalla();
    break;
  case 'c':
		clrPantalla();
		if (gWmy_w < ERROR_w){
			TexPantalla("Reforzando!...", 10, 20);
			//train(gWmy_Ids, vextor, gSzGrid, scnGrafVx);
			trainScans(gIDs, gListaSym, gSzGrid);
			SalvarPesos();
			clrPantalla();
		}
		MapWeight(gSzGrid, gWmy_Ids, scnGrafVx);
     break;
  case 's':
		TexPantalla("Salvando Pesos!...", 10, 20);
		SalvarPesos();
		printf("\n	- Pesos Salvados. ");
		clrPantalla();
     break;
  case 'b':
		clrPantalla();
     break;
  case 'm':
	  //MapOUT();
     break;
  case 'w':
	  clrPantalla();
	  MapWeight(gSzGrid, gWmy_Ids, scnGrafVx);
     break;
  case 27:
	for (i = 0;gListaSym[i][0];i++) j++;
	SalvarLstSymb();
	SalvarPesos();
    exit(0);
    break;
  }
}

void motion(int x, int y)
{
	if (gmausbtlf == 1){
		glColorMask(1, 1, 1, 1);
		DrawXY(x, y);
	}
}

void mouse(int button, int state, int x, int y)
{
int x1,y1,x2,y2, i;
char nom[60];

	if ((button == GLUT_RIGHT_BUTTON) && (pt>1)) {
		gmausbtlf = 0;
		Pt_MinMax(&x1,&y1,&x2,&y2);
		marco(x1,y1,x2,y2);
		MapScan();
		pt = 0;
		if ((gWmy_Ids = MatchPattern(vextor, &gWmy_w)) == -1){// NO reconocido
			if (gIDs == -1)	generate_random_weights();
			TexPantalla("NO Reconocido; Nuevo o Asociarlo? (n,a):", 10, 470);
		}else{//lo encontro
			strcpy(nom, gListaSym[gWmy_Ids]);
			strcat(nom, " - Rsp. correcta, otra Asociacion o Nuevo?(c,a,n):");	
			TexPantalla(nom, 10, 470);
		}
		MapOUT(gListaSym);
		y = 117;
		for (i = 0;gListaSym[i][0];i++){
			TexPantalla(gListaSym[i], 535, y+=25);
		}

		gmausbtlf = 0;
	}else
	if (button == GLUT_MIDDLE_BUTTON) {
		pt = 0;
		gmausbtlf = 0;
		clrPantalla();
		/*if (gWmy_w < ERROR_w){
			TexPantalla("Reforzando Entrenamiento!...", 10, 20);
			train(gWmy_Ids, vextor, gSzGrid, scnGrafVx);
			SalvarPesos();
			clrPantalla();
		}*/
	}else
		if (button == GLUT_LEFT_BUTTON) {
			gmausbtlf = 1;
	}	
}

void display(void)
{
	//glColorMask(GL_TRUE, GL_FALSE, GL_FALSE, GL_TRUE);
	//DrawXY(LOWORD(lParam), HIWORD(lParam));
}

int main(int argc, char** argv)
{
  glutInit(&argc, argv);
  glutInitWindowSize(640, 480);
  glutCreateWindow("Codigo BASE");
  init();
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc(mouse);  
  glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
  glutKeyboardFunc(keyboard);
  glutMotionFunc(motion);
  LeerLstSymb();
  LeerPesos();
  glutMainLoop();
  return 0;
}



