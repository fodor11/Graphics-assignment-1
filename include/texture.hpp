#pragma once
#include <memory>
#include <gl\glut.h>
#include <soil\SOIL.h>
#include <string.h>
#include "model.hpp"

typedef unsigned char Pixel[3];
class vec3f;

class TextureLoader
{

public:
	TextureLoader();
	~TextureLoader();
	void loadMipMappedTexture(std::string filename);
	void loadTexture(std::string filename);
	unsigned int textureID() const;

	void loadImage(std::string filename);
	vec3f getPixelColor(float moisture, float height);

private:
	unsigned int m_textureId;
	std::string m_fileName;
	int m_width = 1;
	int m_height = 1;
	Pixel* m_pixel;
};