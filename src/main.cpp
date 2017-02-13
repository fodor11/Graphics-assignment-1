#include <GL/glut.h>
#include <SOIL/SOIL.h>
#include <math.h>
#include <memory.h>
#include "../include/buildSnowman.hpp"
#include "../include/texture.hpp"

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

unsigned int groundTxtrNr = 0;
HeightMapLoader *heightMap;
void drawGround()
{
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	GLfloat ambient_and_diffuse[] = { 1, 1, 1, 1.0 };
	GLfloat floor_specular[] = { 0, 0, 0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, floor_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
	//glBindTexture(GL_TEXTURE_2D, groundTxtrNr);
	//float textureScale = heightMap->getTextureScale();

	float scale = heightMap->getScale();
	float maxHeight = heightMap->getMaxHeight();

	vec3f normal;
	float height = 0.f;
	vec3f color;
	float image_width = heightMap->getImageWidth() - 1;
	float image_height = heightMap->getImageHeight() - 1;
	for (float i = 0; i < (image_width*scale); i+=scale)
	{
		glBegin(GL_TRIANGLE_STRIP); //vertices: 012 213 234 435 ....
		for (float j = 0; j < (image_height*scale); j+=scale)
		{
			//glTexCoord2f(i/ image_width, j/ image_height);
			//height = heightMap->getHeight((i + scale) / scale, j / scale);
			color = heightMap->getColor((i + scale) / scale, j / scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = heightMap->getNormal((i + scale) / scale, j / scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i+ scale, 
						heightMap->getHeight((i + scale) / scale, j / scale)*maxHeight,
						j);

			//glTexCoord2f(i / image_width, (j+scale) / image_height);
			//height = heightMap->getHeight(i / scale, j / scale);
			color = heightMap->getColor(i / scale, j / scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = heightMap->getNormal(i / scale, j / scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i, 
						heightMap->getHeight(i / scale, j / scale) * maxHeight, 
						j);

			//glTexCoord2f((i+scale) / image_width, j / image_height);
			//height = heightMap->getHeight((i + scale) / scale, j / scale);
			color = heightMap->getColor((i + scale) / scale, (j + scale) / scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = heightMap->getNormal((i + scale) / scale, (j + scale) / scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i+ scale,
					   heightMap->getHeight((i + scale) / scale, (j + scale) / scale) * maxHeight, 
					   j+scale);

			//glTexCoord2f((i + scale) / image_width, (j + scale) / image_height);
			//height = heightMap->getHeight((i + scale) / scale, j / scale);
			color = heightMap->getColor(i / scale, (j + scale) / scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = heightMap->getNormal(i / scale, (j + scale) / scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i, 
						heightMap->getHeight(i / scale, (j + scale) / scale) * maxHeight, 
						j+scale);
		}
		glEnd();
	}

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
}

int time = 0;
void fpsMeter() 
{
	int now = glutGet(GLUT_ELAPSED_TIME);
	cout << 1/((now-time)/1000.0) << endl;
	time = now;
}

Snowman sm("Olaf");
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
	drawGround();

	drawSkybox();

	int midMapX = (heightMap->getImageWidth()*heightMap->getScale()) / 2;
	int midMapY = (heightMap->getImageHeight()*heightMap->getScale()) / 2;
	//draw snowmen
	for (int i = midMapX -30; i < midMapX + 30; i+=10)
	{
		for (int j = midMapY -30; j < midMapY + 30; j+=10)
		{
			glPushMatrix();
			glTranslatef(i, heightMap->getHeight(i / scale, j / scale) * maxHeight, j);
			sm.build();
			glPopMatrix();
		}
	}

	//reset position of the lightsource
	//glLightfv(GL_LIGHT1, GL_POSITION, moon_position);
	glLightfv(GL_LIGHT1, GL_POSITION, moon_position);
	drawAxis();
	glFlush();
	glutSwapBuffers();
	fpsMeter();
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
		//cout << "angle: " << (angle)*(180 / PI) << endl;
		break;
	case 's':
		cameraX -= lx*speed;
		cameraZ -= lz*speed;
		//cout << "angle: " << -(angle)*(180 / PI) << endl;
		break;
	case 'd':
		//cout << "angle: " << (angle + (PI / 2))*(180 / PI) << endl;
		cameraX += cos(angle + (PI / 2))*speed;
		cameraZ += sin(angle + (PI / 2))*speed;
		
		break;
	case 'a':
		//cout << "angle: " << -(angle + (PI / 2))*(180 / PI) << endl;
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
	//cout << "x: " << cameraX << " z: " << cameraZ  <<" height: "<<heightMap->getHeight(cameraX,cameraZ)*20<< endl;
	//glutPostRedisplay();
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

	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_CULL_FACE);
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
	//vector testing
	//vec3f p1(0, 0, 0);
	//vec3f p2(0, 1, 0);
	//vec3f p3(1, 0, 0);
	//vec3f v1 = p2 - p1;
	//vec3f v2 = p3 - p1;
	//vec3f normal = v2*v1;
	//if (normal==vec3f(0,0,1))
	//{
	//	cout << string(v1).c_str() << " X " << string(v2).c_str() << " == " << string(normal).c_str() << endl;
	//}
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
