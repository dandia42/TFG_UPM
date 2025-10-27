#pragma once
#include <GL/glut.h>
#include "Mundo.h"

Mundo mundo;

void OnDraw(void); //esta funcion sera llamada para dibujar
void OnTimer(int value); //esta funcion sera llamada cuando transcurra una temporizacion
void OnKeyboardDown(unsigned char key, int x, int y); //cuando se pulse una tecla	

int main(int argc, char* argv[])
{	
	//Inicializar el gestor de ventanas GLUT
	//y crear la ventana
	glutInit(&argc, argv);
	glutInitWindowSize(1200,900);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glEnable(GLUT_DOUBLE);
	glutCreateWindow("Mundo");

	mundo.InitGL();

	//Registrar los callbacks
	glutDisplayFunc(OnDraw);
	//glutMouseFunc(OnRaton);

	glutTimerFunc(25, OnTimer, 0);//le decimos que dentro de 25ms llame 1 vez a la funcion OnTimer()
	glutKeyboardFunc(OnKeyboardDown);
	glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

	mundo.Init();

	//pasarle el control a GLUT,que llamara a los callbacks
	glutMainLoop();

	return 0;
}

void OnDraw(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	mundo.OnDraw();
	
	glutSwapBuffers();
}
void OnTimer(int value)
{
	mundo.OnTimer();
	glutTimerFunc(25, OnTimer, 0);
	glutPostRedisplay();
}
void OnKeyboardDown(unsigned char key, int x, int y)
{
	mundo.OnKeyboardDown(key, x, y);
	glutPostRedisplay();
}
