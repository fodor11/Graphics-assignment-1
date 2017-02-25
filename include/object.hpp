#pragma once
#include "model.hpp"
#include "objectLoader.hpp"

class Tree
{
public:
	Tree();
	~Tree();
	void drawTree();

private:
	std::vector<GLuint> m_textures;
	GLuint loadTexture(string fileNames);
	void drawFace(Face& face) const;
	GLuint m_dispList;
	void loadDispList();

	GLuint leafTxtr;
	GLuint barkTxtr;
};