#pragma once
#include "model.hpp"
#include "objectLoader.hpp"

class Tree
{
	unsigned int * m_textures;
	void loadTextures(string* fileNames, int num_of_files);
	GLuint m_dispList;
	void loadDispList();

	GLuint leafTxtr;
	GLuint barkTxtr;
	void loadTextures();
public:
	Tree();
	~Tree();
	void drawTree();
	void move();
};