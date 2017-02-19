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
	unsigned int n_materials = 0;
	void countTokens();

	unsigned int m_current_faceList = 0;
	int getFaceListIndex(string textureAlias);

	vec3f * m_vertices;
	vec3f * m_vertNormals;
	float * m_textureCoords;
	//Pointers to Face* objects (separated by textures)
	Face ** m_faceLists;
	//the lengths of the facelists in order
	int * m_faceList_length;
	void readData();

	//read the corresponding .mtl, extracts texture aliases and filenames, 
	//initializes m_faceLists and m_faceList_length
	void readMtl(string filename);
	//string array: [Txtr Alias][Txtr Filename]
	//the first txtr belongs to the first faceList etc...
	string** m_texture_alias;

	ifstream m_file;

public:
	ObjectLoader();
	~ObjectLoader();
	void loadObjFile(char* filename);
	vec3f* getVertices();
	vec3f* getVertexNormals();
	float* getTextureCoords();

	Face** getFaceLists();
	int* getFaceListLengths();
	int getNumberOfFacelists();
	string* getTextureFileNames();
};

class Skybox
{
public:
	void drawSky(char* fileName);
};