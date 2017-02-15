#include "../include/buildSnowman.hpp"
#include <gl\glut.h>


Snowman::Snowman(std::string name)
{
	m_name = name;
}

Snowman::~Snowman()
{
	//cout << m_name.c_str() << " melted" << endl;
}

void Snowman::createDispList()
{
	m_dispList = glGenLists(1);
	glNewList(m_dispList, GL_COMPILE);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	GLfloat mat_specular[] = { 1.0,1.0,1.0,1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat ambient_and_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0 };
	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient_and_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	// Draw Body
	glTranslatef(0.0f, 0.75f, 0.0f);
	glutSolidSphere(0.75f, 20, 20);

	// Draw Head
	glTranslatef(0.0f, 1.0f, 0.0f);
	glutSolidSphere(0.25f, 20, 20);

	// Draw Eyes
	//glDisable(GL_COLOR_MATERIAL);
	ambient_and_diffuse[0] = 0;
	ambient_and_diffuse[1] = 0;
	ambient_and_diffuse[2] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
	glPushMatrix();
	//glColor3f(0.0f, 0.0f, 0.0f);
	glTranslatef(0.05f, 0.10f, 0.18f);
	glutSolidSphere(0.05f, 10, 10);
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glutSolidSphere(0.05f, 10, 10);
	glPopMatrix();

	// Draw Nose
	ambient_and_diffuse[0] = 1;
	ambient_and_diffuse[1] = 0;
	ambient_and_diffuse[2] = 0;
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
	//glColor3f(1.0f, 0.5f, 0.5f);
	glutSolidCone(0.08f, 0.5f, 10, 2);
	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEndList();
}

void Snowman::build()
{
	glCallList(m_dispList);
	//cout << m_name.c_str() << " built\n";
}


