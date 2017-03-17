#include "../include/sky.hpp"

Sky::Sky()
{
}

void Sky::initialize()
{
	//create skysphere
	m_pSkySphere = gluNewQuadric();
	gluQuadricTexture(m_pSkySphere, GL_TRUE);

	//set up moon (sphere + light)
	m_pMoonSphere = gluNewQuadric();
	gluQuadricTexture(m_pMoonSphere, GL_TRUE);
	GLfloat moonLight_ambient[] = { 1,1,1,1.0 };
	GLfloat moonLight_diffuse[] = { 1,1,1,1.0 };
	GLfloat moonLight_specular[] = { 1,1,1,1.0 };
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition);
	glLightfv(GL_LIGHT1, GL_AMBIENT, moonLight_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, moonLight_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, moonLight_specular);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHTING);

	loadTextures();
}

void Sky::updateSky(float cameraX, float cameraY, float cameraZ)
{
	glPushMatrix();
	glTranslatef(cameraX, cameraY, cameraZ);
	moveMoon();
	drawMoon();
	drawSky();
	//reset position of the lightsource
	glLightfv(GL_LIGHT1, GL_POSITION, m_LightPosition);
	glPopMatrix();
}

void Sky::loadTextures()
{
	cout << "Loading sky textures..." << endl;
	TextureLoader *txtrLoaderObj = new TextureLoader();
	//sky
	txtrLoaderObj->loadTexture("sky5.png");
	m_uiSkyTextureId = txtrLoaderObj->textureID();
	//moon
	txtrLoaderObj->loadMipMappedTexture("moon2.jpg");
	m_uiMoonTextureId = txtrLoaderObj->textureID();
	delete txtrLoaderObj;
}

void Sky::drawMoon()
{//moon
	glBindTexture(GL_TEXTURE_2D, m_uiMoonTextureId);
	glPushMatrix();
	glTranslatef(m_LightPosition[0], m_LightPosition[1], m_LightPosition[2]);
	glRotatef(50, 0, 1, 0);
	glRotatef(-35.f, 1.f, 0.f, 0.f);
	glRotatef(45.f, 0.f, 0.f, 1.f);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_MoonMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_MoonMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, m_Shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_MoonEmissive);
	gluSphere(m_pMoonSphere, 80.0f, 36, 72);
	glPopMatrix();

}

void Sky::drawSky()
{
	//sky
	glBindTexture(GL_TEXTURE_2D, m_uiSkyTextureId);
	glPushMatrix();
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, m_SkyMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_SkyMaterial);
	glMaterialfv(GL_FRONT, GL_SHININESS, m_Shininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, m_SkyEmissive);
	glRotatef(45, 0, 0, 1);
	gluQuadricOrientation(m_pSkySphere, GLU_INSIDE);
	gluSphere(m_pSkySphere, 1000.0, 36, 72);
	glPopMatrix();
}

void Sky::moveMoon()
{

}
