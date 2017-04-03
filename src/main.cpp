#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <memory.h>
#include <string>
#include "../include/buildSnowman.hpp"
#include "../include/texture.hpp"
#include "../include/object.hpp"
#include "../include/heightmap.hpp"
#include "../include/camera.hpp"
#include "../include/sky.hpp"
#include "../include/forest.hpp"
#include "../include/environment.hpp"


#define PI 3.14159265
#define RadToAngle 180/PI

// DEBUG
#include <iostream>
using namespace std;

//Global variables
//camera object
Camera* camera;
//environment
Environment* environment;
//mouse position
int mouseX = 0, mouseY = 0;
//middle of the screen
int midX=430, midY=320;
//menu
bool menu = false;
//for flying around
float setHeight = 0.f;

void drawAxis(float nullX, float nullY, float nullZ)
{
	glPushMatrix();
	glDisable(GL_TEXTURE_2D);
	glTranslatef(nullX, nullY, nullZ);
	glLineWidth(10.0f);
	glBegin(GL_LINES);	
	//x axis RED
	GLfloat diffuse_and_ambient[] = { 1,0,0,1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(1.0f, 0.f, 0.f);  glVertex3f(0.f, 0.f, 0.f);	glVertex3f(3.0f, 0.f, 0.f);
	//y axis GREEN
	diffuse_and_ambient[0] = 0;
	diffuse_and_ambient[1] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(0.0f, 1.0f, 0.f);	glVertex3f(0.f, 0.f, 0.f);	glVertex3f(0.0f, 3.0f, 0.f);
	//z axis BLUE
	diffuse_and_ambient[1] = 0;
	diffuse_and_ambient[2] = 1;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, diffuse_and_ambient);
	glColor3f(0.0f, 0.f, 1.f);	glVertex3f(0.f, 0.f, 0.f);	glVertex3f(0.0f, 0.f, 3.f);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glPopMatrix();
}

HeightMapLoader *heightMap;

int time = 0;
double calcFps() 
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	double fps = 1 / ((now - time) / 1000.0);
	time = now;
	return fps;
}

void printFps()
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix(); 
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	glColor3f(1.f, 0.f, 0.f);
	glRasterPos2f(0.6f, -0.9f);
	std::string fpsString = "fps: ";
	fpsString += std::to_string(calcFps());
	//cout << "called" << fpsString << fpsMeter() << endl;
	for (int i = 0; i < strlen(fpsString.c_str()); i++)
	{
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, fpsString[i]);
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix(); 
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();	
}

GLenum error;
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//reset transformations
	glLoadIdentity();

	camera->updateCamera();
	glTranslatef(0.f, setHeight, 0.f);

	float scale = heightMap->getScale();
	float maxHeight = heightMap->getMaxHeight();

	//Draw ground
	heightMap->drawTerrain();

	environment->update();

	//int midMapX = (heightMap->getImageWidth()*heightMap->getScale()) / 2;
	//int midMapY = (heightMap->getImageHeight()*heightMap->getScale()) / 2;
	//drawAxis(midMapX, heightMap->getUnitHeight(midMapX, midMapY) * maxHeight, midMapY);


	printFps();

	//check errors	
	error = glGetError();
	while (error != GL_NO_ERROR)
	{
		std::cout << error << std::endl;
		error = glGetError();
	}
	//std::cout << "error check done" << std::endl;

	glFlush();
	glutSwapBuffers();	
	camera->resetMovements();
}

void reshape(GLsizei width, GLsizei height)
{
	cout << "Window size: " << width << "x" << height << endl;
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70.0, (GLdouble)width / (GLdouble)height, 0.01, 10000.0);
	midX = width / 2;
	midY = height / 2;
	glMatrixMode(GL_MODELVIEW);
}

void mouseHandler(int button, int state, int x, int y)
{
	mouseX = x;
	mouseY = y;
}

void motionHandler(int x, int y)
{
	//rotation around the Y axis
	float rotX = (x - mouseX) / 200.0f;
	camera->addRotationInRadian(rotX);

	//changing horizon
	float rotY = (y - mouseY)/200.f;
	camera->changeHorizonInRadian(rotY);

	//cout<< "horizonAngle2: " << horizonAngle*(180 / PI) << endl;
	//update positions, dont let the cursor leave the window
	if (x<=(midX/3) || x>=(midX * 1.5))
	{
		mouseX = midX;
		glutWarpPointer(midX, y);
	}
	else
	{
		mouseX = x;
	}	
	if (y <= (midY / 3) || y >= (midY * 1.5))
	{
		mouseY = midY;
		glutWarpPointer(x, midY);
	}
	else
	{
		mouseY = y;
	}
}

void cleanUp() {	
	//camera->~Camera();
	//environment->~Environment();
	heightMap->~HeightMapLoader();
}

//speed of camera
float speed = 1.f;
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		camera->setForwardMovement();
		break;
	case 's':
		camera->setBackwardMovement();
		break;
	case 'd':
		camera->setRightMovement();
		break;
	case 'a':
		camera->setLeftMovement();
		break;
	case ' ':
		setHeight -= speed;
		break;
	case 'x':
		setHeight += speed;
		break;
	case 43:
		// + key
		environment->changeAmbientLight(0.1f);
		break;
	case 45:
		// - key
		environment->changeAmbientLight(-0.1f);
		break;
	case 'f':
		environment->toggleFog();
		break;
	case 'm':
		environment->toggleMoonlight();
		break;
	case 27:
		cleanUp();
		exit(0);
		break;
	}
}

void specialFunctionKeys(int key, int x, int y)
{
	if (key == 1)
	{
		if (menu)
		{
			menu = false;
			cout << "exit" << endl;
		}
		else
		{
			menu = true;
			cout << "enter" << endl;
		}
	}
}


void initialize()
{
	cout << "Init ..." << endl;
	
	glEnable(GL_TEXTURE_2D);

	//Loadheightmap
	heightMap = new HeightMapLoader("terrain6_256.png");
	//init cam, sets the current time
	camera = new Camera(heightMap);
	//set up environment
	environment = new Environment();
	environment->initialize(heightMap, camera);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_CULL_FACE);

	//Alpha functions
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.5f);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	//reset the matrix
	glLoadIdentity();
	//hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	camera->startTimer();
}

/**
* Main function
*/
int main(int argc, char* argv[])
{
	cout << "Init GLUT ..." << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	cout << "Init window ..." << endl;
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH)-(midX*2))/2, (glutGet(GLUT_SCREEN_HEIGHT) - (midY * 2)) / 2);
	glutInitWindowSize(midX*2, midY*2);

	cout << "Create window ..." << endl;
	int window = glutCreateWindow("GLUT Window");
	glutSetWindow(window);
	// set mouseX and mouseY
	mouseHandler(0, 0, midX, midY);
	glutWarpPointer(midX, midY);
	initialize();

	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	glutPassiveMotionFunc(motionHandler);
	//glutMotionFunc(motionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialFunctionKeys);
	glutIdleFunc(display);

	//loop
	glutMainLoop();

	//TODO: 
		//free heightmap
	return 0;
}
