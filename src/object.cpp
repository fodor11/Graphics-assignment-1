#include "../include/object.hpp"

GLuint Tree::loadTexture(string fileName)
{
	TextureLoader *txtrLoader = new TextureLoader();
	txtrLoader->loadMipMappedTexture(fileName);
	m_textures.push_back(txtrLoader->textureID());
	txtrLoader->~TextureLoader();
	return m_textures.back();
}

void Tree::drawFace(Face & face) const
{

}

void Tree::loadDispList()
{
	ObjectLoader* objLoader = new ObjectLoader();
	objLoader->loadObjFile("pine1.obj");

	std::vector<vec3f> vertices = objLoader->getVertices();
	std::vector<vec3f> vertNormals = objLoader->getVertexNormals();
	std::vector<std::pair<float, float>> textureCoords = objLoader->getTextureCoords();
	std::map<std::string, std::vector<Face>> faceLists = objLoader->getFaceLists();

	//temporary
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
		for (auto map = faceLists.begin(); map != faceLists.end(); map++)
		{
			GLuint textureId = loadTexture(map->first);
			glBindTexture(GL_TEXTURE_2D, textureId);
			std::cout << "bound texture: " << map->first << endl;
			std::vector<Face> curr_faceList = map->second;
			for (int i = 0; i < curr_faceList.size(); i++)
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
					u = textureCoords[vt_index].first;
					v = textureCoords[vt_index].second;

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


Tree::Tree()
{
	loadDispList();
}

Tree::~Tree()
{
}

void Tree::drawTree()
{
	glCallList(m_dispList);
}
