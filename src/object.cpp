#include "../include/object.hpp"

void Tree::loadDispList()
{
	ObjectLoader* objLoader = new ObjectLoader();
	objLoader->loadObjFile("pine1.obj");
	vec3f * vertices = objLoader->getVertices();
	vec3f * vertNormals = objLoader->getVertexNormals();
	float * textureCoords = objLoader->getTextureCoords();
	Face * faces = objLoader->getFaces();

	Face face;
	int v_index, vt_index, vn_index;
	vec3f vertex, vertexNormal;
	float u, v;

	m_dispList = glGenLists(1);
	glNewList(m_dispList, GL_COMPILE);

	glDisable(GL_TEXTURE_2D);
	GLfloat mat_specular[] = { 0.5,0.5,0.5,1.0 };
	GLfloat ambient_and_diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0 };
	GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1 };
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ambient_and_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);

	glRotatef(-90, 1, 0, 0);
	float scale = 0.05;
	glScalef(scale, scale, scale);
	for (int i = 0; i < objLoader->getNumberOfFaces(); i++)
	{
		face = faces[i];
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; j++)
		{
			//get indexes
			v_index = face[j][0]-1;
			vt_index = face[j][1]-1;
			vn_index = face[j][2]-1;

			//get values
			vertex = vertices[v_index];
			vertexNormal = vertNormals[vn_index];
			u = textureCoords[vt_index, 0];
			v = textureCoords[vt_index, 1];

			glNormal3f(vertexNormal[0], vertexNormal[1], vertexNormal[2]);
			glVertex3f(vertex[0], vertex[1], vertex[2]);
		}
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
	glEndList();

	objLoader->~ObjectLoader();
}

void Tree::loadTextures()
{
}

Tree::Tree()
{
	loadTextures();
	loadDispList();
}

void Tree::drawTree()
{
	glCallList(m_dispList);
}
