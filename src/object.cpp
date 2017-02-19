#include "../include/object.hpp"

void Tree::loadTextures(string * fileNames, int num_of_files)
{
	m_textures = new unsigned int[num_of_files];
	TextureLoader *txtrLoader = new TextureLoader();
	for (int i = 0; i < num_of_files; i++)
	{
		char* fileName = new char[fileNames[i].length()];
		strcpy(fileName, fileNames[i].c_str());
		txtrLoader->loadMipMappedTexture(fileName);
		m_textures[i] = txtrLoader->textureID();
		//delete[] fileName;
	}
	txtrLoader->~TextureLoader();
}

void Tree::loadDispList()
{
	ObjectLoader* objLoader = new ObjectLoader();
	objLoader->loadObjFile("pine1.obj");

	vec3f * vertices = objLoader->getVertices();
	vec3f * vertNormals = objLoader->getVertexNormals();
	float * textureCoords = objLoader->getTextureCoords();

	Face ** faceLists = objLoader->getFaceLists();
	int * faceListsLengths = objLoader->getFaceListLengths();
	int num_facelists = objLoader->getNumberOfFacelists();

	loadTextures(objLoader->getTextureFileNames(), num_facelists);

	//temporary
	Face* curr_faceList;
	Face face;
	int v_index, vt_index, vn_index;
	vec3f vertex, vertexNormal;
	float u, v;

	m_dispList = glGenLists(1);
	glNewList(m_dispList, GL_COMPILE);
	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
		glRotatef(-90, 1, 0, 0);
		float scale = 0.05;
		glScalef(scale, scale, scale);
		for (int k = 0; k < num_facelists; k++)
		{
			glBindTexture(GL_TEXTURE_2D, m_textures[k]);
			cout << "bound texture: " << m_textures[k] << endl;
			curr_faceList = faceLists[k];
			for (int i = 0; i < faceListsLengths[k]; i++)
			{
				face = curr_faceList[i];
				glBegin(GL_TRIANGLES);
				for (int j = 0; j < 3; j++)
				{
					//get indexes
					v_index = face[j][0] - 1;
					vt_index = face[j][1] - 1;
					vn_index = face[j][2] - 1;

					//get values
					vertex = vertices[v_index];
					vertexNormal = vertNormals[vn_index];
					u = textureCoords[vt_index*2 + 0];
					v = textureCoords[vt_index*2 + 1];

					glTexCoord2f(u, 1 - v);
					glNormal3f(vertexNormal[0], vertexNormal[1], vertexNormal[2]);
					glVertex3f(vertex[0], vertex[1], vertex[2]);
				}
				glEnd();
			}
		}	
	glPopMatrix();
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

Tree::~Tree()
{
	delete[] m_textures;
}

void Tree::drawTree()
{
	glCallList(m_dispList);
}
