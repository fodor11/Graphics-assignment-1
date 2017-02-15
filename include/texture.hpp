#pragma once
#include <memory>
#include <gl\glut.h>
#include <soil\SOIL.h>
#include "model.hpp"

typedef unsigned char Pixel[3];
class vec3f;

class TextureLoader
{
private:
	unsigned int m_textureNr;
	char* m_fileName;
	int m_width = 1;
	int m_height = 1;
	Pixel* m_pixel;

public:
	TextureLoader();
	void loadMipMappedTexture(char* filename);
	void loadTexture(char* filename);
	unsigned int textureID();

	void loadImage(char* filename);
	vec3f getPixelColor(float moisture, float height);
};

class HeightMapLoader
{
private:
	unsigned char* m_image;
	char* m_fileName;
	int m_height;
	int m_width;
	int m_channels;
	GLuint m_groundDispList;

	float m_highestPoint=0.4;
	float m_lowestPoint=0.4;
	float m_wettestPoint = 0.4;
	float m_dryestPoint = 0.4;

	float *m_heightValues;
	vec3f *m_vertexNormals;
	float *m_moisture;

	vec3f *m_colors;

	float m_scale = 1.f;
	float m_maxHeight = 90 * m_scale;

	void getMoistValues();
	void getHeightValues();
	void calcVertexNormals();
	vec3f getNormal(int x, int z);
	vec3f getColor(int x, int z);
	void determineColors();
	void createDisplayList();

	void LoadingGreyScaleImage();
	bool existingCoord(int x, int z);
public:
	HeightMapLoader(char* fileName);
	void drawTerrain();
	~HeightMapLoader();
	int getImageWidth();
	int getImageHeight();
	float getHeight(int x, int z);	
	float getScale();
	float getMaxHeight();
};