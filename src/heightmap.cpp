#include "../include/texture.hpp"
#include "../include/heightmap.hpp"
#include <iostream>

/*
* Height map loader
*/
void HeightMapLoader::getHeightValues()
{
	m_pHeightValues = new float[m_width * m_height];

	for (int i = 0; i < m_width * m_height; i++)
	{
		m_pHeightValues[i] = pow((float)m_pImage[i] / 255.0f, 2);
		if (m_pHeightValues[i] > m_highestPoint)
		{
			m_highestPoint = m_pHeightValues[i];
		}
		else if (m_pHeightValues[i] < m_lowestPoint)
		{
			m_lowestPoint = m_pHeightValues[i];
		}
	}
}

void HeightMapLoader::determineColors()
{
	float lightness;
	m_pColors = new vec3f[m_width * m_height];

	TextureLoader *biome = new TextureLoader();
	biome->loadImage("biome-smooth.png");

	for (int i = 0; i < m_width * m_height; i++)
	{
		lightness = (m_pHeightValues[i] + 0.05) * 2;
		if (lightness > 0.5)
		{
			lightness = 0.5;
		}
		float realtiveHeight = (m_pHeightValues[i] - m_lowestPoint) / (m_highestPoint - m_lowestPoint);
		float relativeMoisture = (m_pMoisture[i] - m_dryestPoint) / (m_wettestPoint - m_dryestPoint);
		m_pColors[i] = biome->getPixelColor(relativeMoisture, realtiveHeight)*lightness;
	}
}

void HeightMapLoader::createDisplayList()
{
	m_groundDispList = glGenLists(1);
	glNewList(m_groundDispList, GL_COMPILE);

	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	GLfloat ambient_and_diffuse[] = { 1, 1, 1, 1.0 };
	GLfloat floor_specular[] = { 0, 0, 0, 1.0 };
	glMaterialfv(GL_FRONT, GL_SPECULAR, floor_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);

	vec3f normal;
	float height = 0.f;
	vec3f color;
	for (float i = 0; i < ((m_width - 1)*m_scale); i += m_scale)
	{
		glBegin(GL_TRIANGLE_STRIP); //vertices: 012 213 234 435 ....
		for (float j = 0; j < ((m_height - 1)*m_scale); j += m_scale)
		{
			color = getColor((i + m_scale) / m_scale, j / m_scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = getNormal((i + m_scale) / m_scale, j / m_scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i + m_scale,
				getHeight((i + m_scale) / m_scale, j / m_scale)*m_maxHeight,
				j);

			color = getColor(i / m_scale, j / m_scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = getNormal(i / m_scale, j / m_scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i,
				getHeight(i / m_scale, j / m_scale) * m_maxHeight,
				j);

			color = getColor((i + m_scale) / m_scale, (j + m_scale) / m_scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = getNormal((i + m_scale) / m_scale, (j + m_scale) / m_scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i + m_scale,
				getHeight((i + m_scale) / m_scale, (j + m_scale) / m_scale) * m_maxHeight,
				j + m_scale);

			color = getColor(i / m_scale, (j + m_scale) / m_scale);
			ambient_and_diffuse[0] = color.x();
			ambient_and_diffuse[1] = color.y();
			ambient_and_diffuse[2] = color.z();
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ambient_and_diffuse);
			normal = getNormal(i / m_scale, (j + m_scale) / m_scale);
			glNormal3f(normal.x(), normal.y(), normal.z());
			glVertex3f(i,
				getHeight(i / m_scale, (j + m_scale) / m_scale) * m_maxHeight,
				j + m_scale);
		}
		glEnd();
	}

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glEndList();
}

void HeightMapLoader::getMoistValues()
{
	m_fileName = "moisture.png";
	LoadingGreyScaleImage();
	m_pMoisture = new float[m_width*m_height];
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_pMoisture[i] = (float)m_pImage[i] / 255.0f;
		if (m_pMoisture[i] > m_wettestPoint)
		{
			m_wettestPoint = m_pMoisture[i];
		}
		else if (m_pMoisture[i] < m_dryestPoint)
		{
			m_dryestPoint = m_pMoisture[i];
		}
	}
}

void HeightMapLoader::LoadingGreyScaleImage()
{
	m_pImage = SOIL_load_image(m_fileName.c_str(), &m_width, &m_height, &m_channels, SOIL_LOAD_L);
	if (m_pImage == nullptr)
	{
		std::cout << m_fileName.c_str() << " could not be loaded!" << std::endl;
	}
	else
	{
		std::cout << m_fileName.c_str() << " loaded! resolution: " << m_width << "x" << m_height << std::endl;
	}
}

bool HeightMapLoader::existingCoord(int x, int z)
{
	if (x >= 0 && z >= 0 && x < m_width && z < m_height)
	{
		return true;
	}
	else
	{
		return false;
	}
}
vec3f calcNormalFromPoints(vec3f& p0, vec3f& p1, vec3f& p2)
{
	vec3f v1 = p1 - p0;
	vec3f v2 = p2 - p0;
	return v1*v2;
}
void HeightMapLoader::calcVertexNormals()
{
	std::cout << "Generating normals...\t\t";
	m_pVertexNormals = new vec3f[m_width*m_height];
	vec3f p0(0, 0, 0);
	vec3f p1(0, 0, 0);
	vec3f p2(0, 0, 0);
	int triangles;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			vec3f& actualNormal = m_pVertexNormals[i, j];
			actualNormal = vec3f(0, 0, 0);
			p0 = vec3f(i, getHeight(i, j), j);
			triangles = 0;
			//second
			if (existingCoord(i - 1, j))
			{
				if (existingCoord(i - 1, j + 1))
				{
					p1 = vec3f(i - 1, getHeight(i - 1, j), j);
					p2 = vec3f(i - 1, getHeight(i - 1, j + 1), j + 1);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//third - if 3rd exists, then 4th exists too
			if (existingCoord(i - 1, j + 1))
			{
				p1 = vec3f(i - 1, getHeight(i - 1, j + 1), j + 1);
				p2 = vec3f(i, getHeight(i, j + 1), j + 1);
				triangles++;
				actualNormal += calcNormalFromPoints(p0, p1, p2);
				//check if 5th exists => p1=4th, p2=5th
				if (existingCoord(i + 1, j))
				{
					p1 = vec3f(i, getHeight(i, j + 1), j + 1);
					p2 = vec3f(i + 1, getHeight(i + 1, j), j);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//fifth
			if (existingCoord(i + 1, j))
			{
				if (existingCoord(i + 1, j - 1))
				{
					p1 = vec3f(i + 1, getHeight(i + 1, j), j);
					p2 = vec3f(i + 1, getHeight(i + 1, j - 1), j - 1);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}
			//sixth => if 6th exists, then 1st exists too
			if (existingCoord(i + 1, j - 1))
			{
				p1 = vec3f(i + 1, getHeight(i + 1, j - 1), j - 1);
				p2 = vec3f(i, getHeight(i, j - 1), j - 1);
				triangles++;
				actualNormal += calcNormalFromPoints(p0, p1, p2);
				//check if 2nd exists => p1=1st p2=2nd
				if (existingCoord(i - 1, j))
				{
					p1 = vec3f(i, getHeight(i, j - 1), j - 1);
					p2 = vec3f(i - 1, getHeight(i - 1, j), j);
					triangles++;
					actualNormal += calcNormalFromPoints(p0, p1, p2);
				}
			}

			actualNormal /= triangles;
			actualNormal.normalize();
		}
	}
	std::cout << "Done\n";
}

HeightMapLoader::HeightMapLoader(std::string fileName)
{
	//moisture
	getMoistValues();
	SOIL_free_image_data(m_pImage);
	//height
	m_fileName = fileName;
	LoadingGreyScaleImage();
	getHeightValues();
	calcVertexNormals();
	SOIL_free_image_data(m_pImage);
	//color
	determineColors();
	//display list
	createDisplayList();
}

void HeightMapLoader::drawTerrain() const
{
	glCallList(m_groundDispList);
}

HeightMapLoader::~HeightMapLoader()
{
	delete  m_pImage;
	delete  m_pVertexNormals;
	delete  m_pHeightValues;
}

int HeightMapLoader::getImageWidth() const
{
	return m_width;
}

int HeightMapLoader::getImageHeight() const
{
	return m_height;
}

float HeightMapLoader::getHeight(int x, int z) const
{
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
	{
		return m_pHeightValues[z*m_width + x];
	}
	else
	{
		return 1.0f;
	}
}

vec3f HeightMapLoader::getNormal(int x, int z) const
{
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
	{
		return m_pVertexNormals[x, z];
	}
	else
	{
		return vec3f(0, 1, 0);
	}
}

vec3f HeightMapLoader::getColor(int x, int z) const
{
	return m_pColors[z*m_width + x];
}

float HeightMapLoader::getScale() const
{
	return m_scale;
}

float HeightMapLoader::getMaxHeight() const
{
	return m_maxHeight;
}

