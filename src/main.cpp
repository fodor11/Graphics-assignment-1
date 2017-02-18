#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <memory.h>
#include <string>
#include "../include/buildSnowman.hpp"
#include "../include/texture.hpp"
#include "..//include/object.hpp"

#define PI 3.14159265
#define RadToAngle 180/PI

// DEBUG
#include <iostream>
using namespace std;

//Global variables
//rotation in the Y axis
float angle = 0.0f;
//position of the camera
float cameraX = 0.0f, cameraY = 0.0f, cameraZ = 0.0f;
//"height" of the camera
float cameraHeight = 2.f;
//camera's direction (vector)
float lx = 0.0f, lz = -1.0f;
//horizon angle in radian
float horizonAngle = 0.0;
//mouse position
int mouseX = 0, mouseY = 0;
//middle of the screen
int midX=430, midY=320;
//menu
bool menu = false;

void drawAxis()
{
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
}

unsigned int skytxtrNr = 0;
GLUquadric *skySphere;
unsigned int moontxtrNr = 0;
GLUquadric *moon;
GLfloat moon_position[] = { -500, 450, 120 , 1};
void drawSkybox()
{
	//moon
	glBindTexture(GL_TEXTURE_2D, moontxtrNr);
	glPushMatrix();	
		glTranslatef(moon_position[0], moon_position[1], moon_position[2]);
		glRotatef(50, 0, 1, 0);
		glRotatef(-35.f, 1.f, 0.f, 0.f);
		glRotatef(45.f, 0.f, 0.f, 1.f);
		GLfloat moon_emissive[] = { 1,1,0.75f,1.0 };
		GLfloat moon_material[] = { 0,0,0,1 };
		GLfloat shininess[] = { 0 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, moon_material);
		glMaterialfv(GL_FRONT, GL_SPECULAR, moon_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, moon_emissive);
		gluSphere(moon, 80.0f, 36, 72);
	glPopMatrix();

	//sky
	glBindTexture(GL_TEXTURE_2D, skytxtrNr);
	glPushMatrix();
		GLfloat sky_material[] = { 1,1,1,1 };
		GLfloat sky_emissive[] = { 0,0,0,1 };
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, sky_material);
		glMaterialfv(GL_FRONT, GL_SPECULAR, sky_material);
		glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, sky_emissive);
		glRotatef(45, 0, 0, 1);
		gluQuadricOrientation(skySphere, GLU_INSIDE);
		gluSphere(skySphere, 1000.0, 36, 72);
	glPopMatrix();
}

HeightMapLoader *heightMap;
Tree *tree1;

int time = 0;
double fpsMeter() 
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	double fps = 1 / ((now - time) / 1000.0);
	time = now;
	return fps;
}

void printFPS()
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
	fpsString += std::to_string(fpsMeter());
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

Snowman sm("Olaf");
GLenum error;
void display()
{
	//cout << "display" << endl;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//reset transformations
	glLoadIdentity();

	//set horizon (+change from radius to angle)
	glRotatef(horizonAngle*RadToAngle, 1, 0, 0);
	//set camera height
	float scale = heightMap->getScale();
	float maxHeight = heightMap->getMaxHeight();
	//cameraY = heightMap->getHeight(cameraX / scale, cameraZ / scale) * maxHeight + cameraHeight;
	//set the camera
	gluLookAt(	cameraX, cameraY, cameraZ,					//camera position
				cameraX + lx, cameraY, cameraZ + lz,		//look at point
				0.0f, 0.1f, 0.0f);							//up vector
	
	
	//Draw ground
	heightMap->drawTerrain();

	drawSkybox();

	int midMapX = (heightMap->getImageWidth()*heightMap->getScale()) / 2;
	int midMapY = (heightMap->getImageHeight()*heightMap->getScale()) / 2;
	//draw Trees
	for (int i = midMapX -30; i < midMapX + 30; i+=6)
	{
		for (int j = midMapY -30; j < midMapY + 30; j+=6)
		{
			glPushMatrix();
			glTranslatef(i, heightMap->getHeight(i / scale, j / scale) * maxHeight, j);
			tree1->drawTree();
			glPopMatrix();
		}
	}
	//glPushMatrix();
	//glTranslatef(midMapX, heightMap->getHeight(midMapX / scale, midMapX / scale) * maxHeight, midMapX);
	//tree1->drawTree();
	//glPopMatrix();

	//reset position of the lightsource
	//glLightfv(GL_LIGHT1, GL_POSITION, moon_position);
	glLightfv(GL_LIGHT1, GL_POSITION, moon_position);
	drawAxis();
	printFPS();

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
	angle += rotX;
	lx = cos(angle);
	lz = sin(angle);
	if (angle > (PI * 2) || angle < -(PI * 2))
	{
		angle = fmodf(angle, (PI * 2));
	}
	//cout << "angle: " << angle*(180 / PI) << endl;

	//changing horizon
	float rotY = (y - mouseY)/200.f;
	horizonAngle += rotY;
	if (horizonAngle < -(PI / 2))
	{
		horizonAngle = -(PI / 2);
	}
	if (horizonAngle > (PI / 2))
	{
		horizonAngle = (PI / 2);
	}
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

//speed of camera
float speed = 1.f;
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		cameraX += lx*speed;
		cameraZ += lz*speed;
		break;
	case 's':
		cameraX -= lx*speed;
		cameraZ -= lz*speed;
		break;
	case 'd':
		cameraX += cos(angle + (PI / 2))*speed;
		cameraZ += sin(angle + (PI / 2))*speed;
		break;
	case 'a':
		cameraX -= cos(angle + (PI / 2))*speed;
		cameraZ -= sin(angle + (PI / 2))*speed;
		break;
	case ' ':
		cameraY += speed;
		break;
	case 'x':
		cameraY -= speed;
		break;
	case 27:
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
	//create skysphere
	skySphere = gluNewQuadric();
	gluQuadricTexture(skySphere, GL_TRUE);

	//snowman display list
	sm.createDispList();

	//set up moon (sphere + light)
	moon = gluNewQuadric();
	gluQuadricTexture(moon, GL_TRUE);
	GLfloat moonLight_ambient[] = { 1,1,1,1.0 };
	GLfloat moonLight_diffuse[] = { 1,1,1,1.0 };
	GLfloat moonLight_specular[] = { 1,1,1,1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, moon_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, moonLight_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, moonLight_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, moonLight_specular);
	glEnable(GL_LIGHT1);
	//set up a light source
	//glLightfv(GL_LIGHT0, GL_POSITION, light_position);	
	//glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	//Load textures
	cout << "Loading textures..." << endl;
	TextureLoader *txtrLoaderObj = new TextureLoader();
	//sky
	txtrLoaderObj->loadTexture("sky5.png");
	skytxtrNr = txtrLoaderObj->textureID();
	//delete skyBoxTxtr;
	//moon
	txtrLoaderObj->loadMipMappedTexture("moon2.jpg");
	moontxtrNr = txtrLoaderObj->textureID();
	//ground
	//txtrLoaderObj->loadMipMappedTexture("allTerrainTexture.png");
	//groundTxtrNr = txtrLoaderObj->textureID();

	delete txtrLoaderObj;
	glEnable(GL_TEXTURE_2D);

	//Loadheightmap
	heightMap = new HeightMapLoader("terrain6_256.png");
	cameraX = (heightMap->getImageWidth()*heightMap->getScale()) / 2;
	cameraZ = (heightMap->getImageHeight()*heightMap->getScale()) / 2;

	//Load 3D models
	tree1 = new Tree();

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	//glEnable(GL_CULL_FACE);
	glClearColor(0.0, 0.0, 0.0, 0.0);

	//Light model setup
	float light_tmp = 0.1;
	GLfloat lmodel_ambient[] = { light_tmp, light_tmp, light_tmp, 1.0 }; //amount of light anywhere
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glMatrixMode(GL_PROJECTION);

	//reset the matrix
	glLoadIdentity();
	//hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);

	//glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
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
	glutWarpPointer(midX, midY);
	initialize();

	//callbacks
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(mouseHandler);
	//glutPassiveMotionFunc(motionHandler);
	glutMotionFunc(motionHandler);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialFunctionKeys);
	glutIdleFunc(display);

	//loop
	glutMainLoop();

	//TODO: 
		//free heightmap
	return 0;
}
