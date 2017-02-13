#include "../include/texture.hpp"
#include <iostream>

/*
* Texture loader
*/
void TextureLoader::loadTexture(char* filename)
{
	m_fileName = filename;
	m_textureNr = 0;
	m_textureNr = SOIL_load_OGL_texture(m_fileName, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	if (m_textureNr == 0)	
	{
		std::cout << m_fileName << " could not be loaded!" << std::endl;
	}
	else
		std::cout << m_fileName << " loaded!" << std::endl;

	glBindTexture(GL_TEXTURE_2D, m_textureNr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //when the shown picture is streched
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //when the shown picture is smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //coordinate outside the range of 0 to 1 (s,t = x,y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void TextureLoader::loadImage(char * filename)
{
	m_fileName = filename;
	m_pixel = (Pixel*)SOIL_load_image(m_fileName, &m_width, &m_height, 0, SOIL_LOAD_RGB);
	if (m_pixel == NULL)
	{
		std::cout << m_fileName << " could not be loaded!" << std::endl;
	}
	else
		std::cout << m_fileName << " loaded!" << std::endl;
}

vec3f TextureLoader::getPixelColor(float moisture, float height)
{
	if (m_pixel != NULL && moisture >= 0 && height >= 0 && moisture <= 1.00001 && height <= 1.00001)
	{
		int x = m_width*moisture;
		int y = m_height*(1-height);
		int index = y*m_width + x;
		float r = m_pixel[index][0];
		float g = m_pixel[index][1];
		float b = m_pixel[index][2];
		return vec3f(r / 255, g / 255, b / 255);
	}
	else return vec3f(1, 0, 0);
}

unsigned int TextureLoader::textureID()
{
	return m_textureNr;
}

TextureLoader::TextureLoader()
{
}

void TextureLoader::loadMipMappedTexture(char * filename)
{
	m_fileName = filename;
	//get the texture id
	glGenTextures(1, &m_textureNr);
	glBindTexture(GL_TEXTURE_2D, m_textureNr);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	unsigned char* imageData;
	imageData = SOIL_load_image(m_fileName, &m_width, &m_height, 0, SOIL_LOAD_RGB);
	if (imageData==NULL)
	{
		std::cout << m_fileName << " could not be loaded!" << std::endl;
	}
	else
		std::cout << m_fileName << " loaded!" << std::endl;

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //when the shown picture is streched
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //when the shown picture is smaller
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //coordinate outside the range of 0 to 1 (s,t = x,y)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, imageData);

	//GLenum error;
	//error = glGetError();
	//while (error != GL_NO_ERROR)
	//{
	//std::cout << error << std::endl;
	//error = glGetError();
	//}
	//std::cout << "error check done" << std::endl;
	SOIL_free_image_data(imageData);
}

/*
* Height map loader
*/
void HeightMapLoader::getHeightValues()
{
	m_heightValues = new float[m_width*m_height];
	
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_heightValues[i] = pow((float)m_image[i]/255.0f,2);
		if (m_heightValues[i]>m_highestPoint)
		{
			m_highestPoint = m_heightValues[i];
		}
		else if (m_heightValues[i]<m_lowestPoint)
		{
			m_lowestPoint = m_heightValues[i];
		}
	}
	//std::cout << m_width << "x" << m_height << std::endl;
	//std::cout << getHeight(150, 150) << std::endl;
	//std::cout << getHeight(m_width-1, m_height-1) << "==" << m_heightValues[(m_height-1)*(m_width - 1) +(m_width - 1)] << std::endl;
}

void HeightMapLoader::determineColors()
{
	float lightness;
	m_colors = new vec3f[m_width*m_height];

	//vec3f green(0.420f, 0.557f, 0.137f);
	//vec3f darkGreen(0.333f, 0.420f, 0.184f);
	//vec3f grey(0.212f, 0.212f, 0.212f);
	//vec3f darkGrey(0.152f, 0.152f, 0.152f);
	//vec3f white(0.5f, 0.5f, 0.4f);
	//vec3f notSoWhite(0.35f, 0.35f, 0.3f);

	TextureLoader *biome = new TextureLoader();
	biome->loadImage("biome-smooth.png");

	for (int i = 0; i < m_width*m_height; i++)
	{
		lightness = (m_heightValues[i] + 0.05)*2;
		if (lightness > 0.5)
		{
			lightness = 0.5;
		}
		float realtiveHeight = (m_heightValues[i] - m_lowestPoint) / (m_highestPoint - m_lowestPoint);
		float relativeMoisture = (m_moisture[i] - m_dryestPoint) / (m_wettestPoint - m_dryestPoint);
		m_colors[i] = biome->getPixelColor(relativeMoisture, realtiveHeight)*lightness;
		//lightness = (m_heightValues[i] + 0.05) * 2;
		//if (m_heightValues[i] < 0.09) //Green grass
		//{
		//	if (m_moisture[i] > 0.7)
		//	{
		//		m_colors[i] = darkGreen * lightness;
		//	}
		//	else
		//		m_colors[i] = green * lightness;
		//}
		//else if (m_heightValues[i] < 0.15) //Darker grass
		//{
		//	if (m_moisture[i] > 0.5)
		//	{
		//		m_colors[i] = darkGreen * lightness;
		//	}
		//	else
		//		m_colors[i] = grey* lightness;
		//}
		//else if (m_heightValues[i] < 0.25) //Grey rock + grass
		//{
		//	if (m_moisture[i] > 0.5)
		//	{
		//		m_colors[i] = darkGreen * lightness;
		//	}
		//	else
		//		m_colors[i] = grey*lightness;
		//}
		//else if (m_heightValues[i] < 0.35) //Grey rock + darker grey
		//{
		//	if (m_moisture[i] > 0.55)
		//	{
		//		m_colors[i] = darkGrey * lightness;
		//	}
		//	else
		//		m_colors[i] = grey*lightness;
		//}
		//else if (m_heightValues[i] < 0.35) //Grey rock + white snow
		//{
		//	if (m_moisture[i] > 0.4)
		//	{
		//		m_colors[i] = white * lightness;
		//	}
		//	else
		//		m_colors[i] = grey*lightness;
		//}
		//else //White snow + darker snow
		//{
		//	if (m_moisture[i] > 0.6)
		//	{
		//		m_colors[i] = notSoWhite * lightness;
		//	}
		//	else
		//		m_colors[i] = white * lightness;
		//}
	}
}

void HeightMapLoader::getMoistValues()
{
	m_fileName = "moisture.png";
	LoadingGreyScaleImage();
	m_moisture = new float[m_width*m_height];
	for (int i = 0; i < m_width*m_height; i++)
	{
		m_moisture[i] = (float)m_image[i] / 255.0f;
		if (m_moisture[i]>m_wettestPoint)
		{
			m_wettestPoint = m_moisture[i];
		}
		else if (m_moisture[i]<m_dryestPoint)
		{
			m_dryestPoint = m_moisture[i];
		}
	}
}

void HeightMapLoader::LoadingGreyScaleImage()
{
	m_image = SOIL_load_image(m_fileName, &m_width, &m_height, &m_channels, SOIL_LOAD_L);
	if (m_image == NULL)
	{
		std::cout << m_fileName << " could not be loaded!" << std::endl;
	}
	else
	{
		std::cout << m_fileName << " loaded! resolution: " << m_width << "x" << m_height << std::endl;
	}
}

bool HeightMapLoader::existingCoord(int x, int z)
{
	if (x>=0 && z>=0 && x<m_width && z<m_height)
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
	m_vertexNormals = new vec3f[m_width*m_height];
	vec3f p0(0, 0, 0);
	vec3f p1(0, 0, 0);
	vec3f p2(0, 0, 0);
	int triangles;
	for (int i = 0; i < m_width; i++)
	{
		for (int j = 0; j < m_height; j++)
		{
			vec3f& actualNormal = m_vertexNormals[i, j];
			actualNormal = vec3f(0, 0, 0);
			p0 = vec3f(i, getHeight(i, j), j);
			triangles = 0;
			//second
			if (existingCoord(i - 1, j))
			{
				if (existingCoord(i - 1, j+1))
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

HeightMapLoader::HeightMapLoader(char* fileName)
{
	//moisture
	getMoistValues();
	SOIL_free_image_data(m_image);
	//height
	m_fileName = fileName;
	LoadingGreyScaleImage();
	getHeightValues();
	calcVertexNormals();
	SOIL_free_image_data(m_image);
	//color
	determineColors();
}

HeightMapLoader::~HeightMapLoader()
{
	delete  m_image;
	delete m_fileName;
	delete  m_vertexNormals;
	delete  m_heightValues;
}

int HeightMapLoader::getImageWidth()
{
	return m_width;
}

int HeightMapLoader::getImageHeight()
{
	return m_height;
}

float HeightMapLoader::getHeight(int x, int z)
{
	//std::cout << "x,z: " << x << ", " << z << std::endl;
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
	{
		return m_heightValues[z*m_width + x];
	}	
	else
	{
		return 1.0f;
	}
}

vec3f HeightMapLoader::getNormal(int x, int z)
{
	if (x < m_width && z < m_height && x >= 0 && z >= 0)
	{
		return m_vertexNormals[x,z];
	}
	else
	{
		return vec3f(0, 1, 0);
	}
}

vec3f HeightMapLoader::getColor(int x, int z)
{
	return m_colors[z*m_width + x];
}

float HeightMapLoader::getScale()
{
	return m_scale;
}

float HeightMapLoader::getMaxHeight()
{
	return m_maxHeight;
}

float HeightMapLoader::getTextureScale()
{
	return m_textureScale;
}

