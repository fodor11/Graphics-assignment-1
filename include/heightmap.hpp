#pragma once
#include <memory>
#include <gl\glut.h>
#include <soil\SOIL.h>
#include <string.h>
#include "model.hpp"

typedef unsigned char Pixel[3];
class vec3f;

class HeightMapLoader
{
public:
	HeightMapLoader(std::string fileName);
	void drawTerrain() const;
	~HeightMapLoader();
	int getImageWidth() const;
	int getImageHeight() const;
	float getHeight(float x, float z);
	float getScale() const;
	float getMaxHeight() const;

private:
	unsigned char* m_pImage;
	std::string m_fileName;
	int m_height;
	int m_width;
	int m_channels;
	GLuint m_groundDispList;

	float m_highestPoint=0.4;
	float m_lowestPoint=0.4;
	float m_wettestPoint = 0.4;
	float m_dryestPoint = 0.4;

	float *m_pHeightValues;
	vec3f *m_pVertexNormals;
	float *m_pMoisture;

	vec3f *m_pColors;

	float m_scale = 1.f;
	float m_maxHeight = 90 * m_scale;

	void getMoistValues();
	void getHeightValues();
	void calcVertexNormals();
	vec3f getNormal(int x, int z) const;
	vec3f getColor(int x, int z) const;
	void determineColors();
	void createDisplayList();
	float linearInterpolation(float pointA, float pointB, float weightB);

	void LoadingGreyScaleImage();
	bool existingCoord(int x, int z);
};