#pragma once
#include <gl\glut.h>
#include <iostream>
#include <string>
#include <fstream>
#include "model.hpp"

class vec3f; 

class FacePoint
{
	int m_v;	//0
	int m_vt;	//1
	int m_vn;	//2

public:
	FacePoint();
	~FacePoint();
	int& operator[](int index);
	int & getV();
	int & getVt();
	int & getVn();
};

class Face
{
	FacePoint * m_facePoint;
public:
	Face();
	~Face();
	FacePoint& operator[](int index);
	Face& operator=(const Face& other);
};

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
	Face * m_faces;
	void readData();

	ifstream m_file;

public:
	ObjectLoader();
	~ObjectLoader();
	void loadObjFile(char* filename);
	vec3f* getVertices();
	vec3f* getVertexNormals();
	float* getTextureCoords();
	Face* getFaces();
	int getNumberOfFaces();
};

class Skybox
{
public:
	void drawSky(char* fileName);
};