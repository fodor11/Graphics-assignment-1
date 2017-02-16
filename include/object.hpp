#pragma once
#include "model.hpp"

class Tree
{
	GLuint dispList;
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