#pragma once
#include "model.hpp"
#include "objectLoader.hpp"

class Tree
{
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