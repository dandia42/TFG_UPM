#pragma once
#include <GL/glut.h>

#include "world.h"

World world;

void onDraw(void);                                     // draw method
void onTimer(int value);                               // timer method
void onKeyboardDown(unsigned char key, int x, int y);  // method when a key is pressed

int main(int argc, char* argv[]) {
  // Initialize GLUT window manager and creates window
  glutInit(&argc, argv);
  glutInitWindowSize(1200, 900);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glEnable(GLUT_DOUBLE);
  glutCreateWindow("world");

  world.initGL();

  // Callbacks registry
  glutDisplayFunc(onDraw);
  // glutMouseFunc(OnRaton);

  glutTimerFunc(25, onTimer, 0);  // Calls onTimer() each 25ms
  glutKeyboardFunc(onKeyboardDown);
  glutSetCursor(GLUT_CURSOR_FULL_CROSSHAIR);

  world.init();

  // GLUT control to manage callbacks
  glutMainLoop();

  return 0;
}

void onDraw(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  world.onDraw();

  glutSwapBuffers();
}
void onTimer(int value) {
  world.onTimer();
  glutTimerFunc(25, onTimer, 0);
  glutPostRedisplay();
}
void onKeyboardDown(unsigned char key, int x, int y) {
  world.onKeyboardDown(key, x, y);
  glutPostRedisplay();
}
