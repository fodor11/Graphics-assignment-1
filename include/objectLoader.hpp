#pragma once
#include <gl\glut.h>
#include <iostream>
#include <string>
#include <fstream>
#include "model.hpp"
#include <vector>
#include <map>

class vec3f; 

class FacePoint
{
public:
	FacePoint();
	~FacePoint();
	int& operator[](int index);
	int & getVertex();
	int & getVertexTexture();
	int & getVertexNormal();

private:
	int m_vertex;	//0
	int m_vertexTexture;	//1
	int m_vertexNormal;	//2
};

class Face
{
public:
	Face();
	Face(const Face& other);
	~Face();
	FacePoint& operator[](int index);
	Face& operator=(const Face& other);
	Face& operator=(Face& other);
	string printFace();
private:
	FacePoint * m_pFacePoint;
};

class ObjectLoader
{
public:
	ObjectLoader();
	~ObjectLoader();
	void loadObjFile(std::string filename);
	std::vector<vec3f> getVertices() const;
	std::vector<vec3f> getVertexNormals() const;
	std::vector<std::pair<float, float>> getTextureCoords() const;
	std::map<std::string, std::vector<Face>> getFaceLists() const;

private:
	unsigned int m_num_vertices = 0;
	unsigned int m_num_vertexTextures = 0;
	unsigned int m_num_vertexNormals = 0;
	unsigned int m_num_materials = 0;
	void countTokens();

	unsigned int m_current_faceList = 0;
	std::string getTextureFileName(const std::string& textureAlias) const;
	int getFaceListIndex(const std::string& textureAlias) const;

	std::vector<vec3f> m_vertices;
	std::vector<vec3f> m_vertexNormals;
	std::vector<std::pair<float, float>> m_textureCoords;
	std::map<std::string, std::vector<Face>> m_faceLists;


	//the lengths of the facelists in order
	int * m_faceList_lengths;
	void readData();

	//read the corresponding .mtl, extracts texture aliases and filenames, 
	//initializes m_faceLists and m_faceList_length
	void readMtlFile(string filename);
	//string array: [Txtr Alias][Txtr Filename]
	//the first txtr belongs to the first faceList etc...
	std::vector<std::pair<std::string, std::string>> m_texture_aliases;

	ifstream m_file;
};

class Skybox
{
public:
	void drawSky(char* fileName);
};