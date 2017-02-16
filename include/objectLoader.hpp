#pragma once
#include <gl\glut.h>
#include <iostream>
#include <string>
#include <fstream>
#include "model.hpp"

class vec3f; 

class ObjectLoader
{
	unsigned int m_num_v = 0;
	unsigned int m_num_vt = 0;
	unsigned int m_num_vn = 0;
	unsigned int m_num_f = 0;
	void countTokens();

	vec3f * m_vertices;
	vec3f * m_vertNormals;
	float * m_textureCoords;
	void readData();

	ifstream m_file;

public:
	ObjectLoader();
	~ObjectLoader();
	void loadObjFile(char* filename);
	vec3f* getVertices();
};

class Skybox
{
public:
	void drawSky(char* fileName);
};