#pragma once
#include <gl\glut.h>
#include "texture.hpp"

class Sky {
public:
	Sky();
	~Sky();
	/// init spheres, light
	void initialize();
	void updateSky(float cameraX, float cameraY, float cameraZ);
private:
	unsigned int m_uiSkyTextureId = 0;
	GLUquadric *m_pSkySphere;

	unsigned int m_uiMoonTextureId = 0;
	GLUquadric *m_pMoonSphere;
	float m_fMoonSpeed = 0.001f;
	float m_fMoonCurvePosition = 0.0f;

	GLfloat m_LightPosition[4] = { -500.f, 450.f, 120.f , 1.f };

	GLfloat m_MoonEmissive[4] = { 1.f, 1.f, 0.75f, 1.0f };
	GLfloat m_MoonMaterial[4] = { 0.f, 0.f, 0.f, 1.f };
	GLfloat m_Shininess[1] = { 0.f };

	GLfloat m_SkyMaterial[4] = { 1.f, 1.f, 1.f, 1.f };
	GLfloat m_SkyEmissive[4] = { 0.f, 0.f, 0.f, 1.f };

	void loadTextures();
	void drawMoon();
	void drawSky();
	void moveMoon();
};