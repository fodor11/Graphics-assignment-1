#include "..\include\objectLoader.hpp"

void Skybox::drawSky(char * fileName)
{

}

void ObjectLoader::countTokens()
{
	m_num_v = 0;
	m_num_vt = 0;
	m_num_vn = 0;
	m_num_f = 0;
	string line;
	if (m_file.is_open())
	{
		cout << "Counting tokens in .obj file" << endl;
		while (getline(m_file, line))
		{
			if (line.compare(0,2,"v ")==0)
			{
				m_num_v++;
			}
			else if (line.compare(0, 2, "vt") == 0)
			{
				m_num_vt++;
			}
			else if (line.compare(0, 2, "vn") == 0)
			{
				m_num_vn++;
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
				m_num_f++;
			}
		}
	}
	cout << "v: " << m_num_v << "   vt: " << m_num_vt << "   vn: " << m_num_vn << "   f: " << m_num_f << endl;
	m_vertices = new vec3f[m_num_v];
	m_vertNormals = new vec3f[m_num_vn];
	m_textureCoords = new float[m_num_vt, 2];
	m_faces = new Face[m_num_f];
}

void parse3floats(vec3f& vector, string line)
{
	string::size_type nextFloat = 2; //jumping the vn/v
	for (int i = 0; i < 3; i++)
	{
		line = line.substr(nextFloat);
		vector[i] = stof(line, &nextFloat);
	}
}
void parse2floats(float* vector, string line)
{
	string::size_type nextFloat = 2; //jumping the vt
	for (int i = 0; i < 2; i++)
	{
		line = line.substr(nextFloat);
		vector[i] = stof(line, &nextFloat);
	}
}
void parseFace(Face& face, string line)
{
	string::size_type nextInt = 1;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			line = line.substr(nextInt+1);	//+1 for jumping '/' or ' '
			face[i][j] = stoi(line, &nextInt);
		}
	}
}
void ObjectLoader::readData()
{
	//clear EOF flag and return to the beginning
	m_file.clear();
	m_file.seekg(0, ios::beg);

	m_num_v = 0;
	m_num_vt = 0;
	m_num_vn = 0;
	m_num_f = 0;

	string line;
	if (m_file.is_open())
	{
		cout << "Parsing Data" << endl;
		while (getline(m_file, line))
		{
			if (line.compare(0, 2, "v ") == 0)
			{
				parse3floats(m_vertices[m_num_v], line);
				m_num_v++;
			}
			else if (line.compare(0, 2, "vt") == 0)
			{
				parse2floats(&m_textureCoords[m_num_vt,0], line);
				m_num_vt++;
			}
			else if (line.compare(0, 2, "vn") == 0)
			{
				parse3floats(m_vertNormals[m_num_vn], line);
				m_num_vn++;
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
				parseFace(m_faces[m_num_f], line);
				m_num_f++;
			}
		}
	}
}

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
	delete[] m_vertices;
	delete[] m_textureCoords;
	delete[] m_vertNormals;
	delete[] m_faces;
}

void ObjectLoader::loadObjFile(char * filename)
{
	m_file.open(filename);
	//do stuff
	countTokens();
	readData();

	m_file.close();
}

vec3f * ObjectLoader::getVertices()
{
	return m_vertices;
}

FacePoint::FacePoint()
{
	m_v = 0;
	m_vt = 0;
	m_vn = 0;
}

FacePoint::~FacePoint()
{
}

int & FacePoint::operator[](int index)
{
	switch (index)
	{
	case 0:
		return m_v;
		break;
	case 1:
		return m_vt;
		break;
	case 2:
		return m_vn;
		break;
	}
}

int & FacePoint::getV()
{
	return m_v;
}

int & FacePoint::getVt()
{
	return m_vt;
}

int & FacePoint::getVn()
{
	return m_vn;
}

Face::Face()
{
	m_facePoint = new FacePoint[3];
}

Face::~Face()
{
	delete[] m_facePoint;
}

FacePoint & Face::operator[](int index)
{
	if (index>=0 && index <=2)
	{
		return m_facePoint[index];
	}
}
