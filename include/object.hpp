#pragma once
#include "model.hpp"
#include "objectLoader.hpp"

class Tree
{
public:
	Tree(string fileName);
	~Tree();
	void drawTree();
	void drawBillBoard();

private:
	string m_fileName;
	float m_scale;
	//the last texture is thebillboard image
	std::vector<GLuint> m_textures;
	GLuint loadTexture(string fileNames);

	void drawFace(Face& face) const;
	GLuint m_objectDispList;
	void loadObjectDispList();

	GLuint m_billBoardDispList;
	void loadBillBoardDispList();

};