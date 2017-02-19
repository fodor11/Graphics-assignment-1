#include "..\include\objectLoader.hpp"

void Skybox::drawSky(char * fileName)
{

}

//read .mtl
void ObjectLoader::readMtl(string filename)
{
	cout << "Parsing .mtl...\t";
	ifstream mtlFile;
	mtlFile.open(filename);
	n_materials = 0;
	string line;
	//counting
	if (mtlFile.is_open())
	{
		while (getline(mtlFile, line))
		{
			if (line.compare(0, 7, "newmtl ") == 0)
			{
				n_materials++;
			}
		}
	}
	m_texture_alias = new string*[n_materials];

	//parsing
	mtlFile.clear();
	mtlFile.seekg(0, ios::beg);
	n_materials = 0;

	if (mtlFile.is_open())
	{
		while (getline(mtlFile, line))
		{
			if (line.compare(0, 7, "newmtl ") == 0)
			{
				m_texture_alias[n_materials] = new string[2];
				m_texture_alias[n_materials][0] = line.substr(7);
			}
			else if (line.compare(0, 7, "map_Kd ") == 0)
			{
				m_texture_alias[n_materials][1] = line.substr(7);
				n_materials++;
			}
		}
		//cout << m_texture_alias[0][0] << ": " << m_texture_alias[0][1] << endl;
		//cout << m_texture_alias[1][0] << ": " << m_texture_alias[1][1] << endl;
	}
	
	mtlFile.close();
	m_faceLists = new Face*[n_materials];
	m_faceList_length = new int[n_materials];
	cout << " Done." << endl;
}

int ObjectLoader::getFaceListIndex(string textureAlias)
{
	for (int i = 0; i < n_materials; i++)
	{
		if (m_texture_alias[i][0].compare(textureAlias) == 0)
		{
			return i;
		}
	}
}

void ObjectLoader::countTokens()
{
	m_num_v = 0;
	m_num_vt = 0;
	m_num_vn = 0;
	m_current_faceList = 0;
	
	string line;
	if (m_file.is_open())
	{
		cout << "Counting tokens in .obj file" << endl;
		while (getline(m_file, line))
		{
			if (line.compare(0, 7, "mtllib ") == 0)
			{
				readMtl(line.substr(7));
				for (int i = 0; i < n_materials; i++)
				{
					m_faceList_length[i] = 0;
				}
			}
			else if (line.compare(0,2,"v ")==0)
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
			else if (line.compare(0, 7, "usemtl ") == 0)
			{
				m_current_faceList = getFaceListIndex(line.substr(7));
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
				m_faceList_length[m_current_faceList]++;
			}
		}
	}
	cout << "v: " << m_num_v << "   vt: " << m_num_vt << "   vn: " << m_num_vn << endl;
	for (int i = 0; i < n_materials; i++)
	{
		cout << "facelist[" << i << "]: " << m_faceList_length[i] << endl;
	}
	m_vertices = new vec3f[m_num_v];
	m_vertNormals = new vec3f[m_num_vn];
	m_textureCoords = new float[m_num_vt*2];
	for (int i = 0; i < n_materials; i++)
	{
		m_faceLists[i] = new Face[m_faceList_length[i]];
	}
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
	m_current_faceList = 0;
	for (int i = 0; i < n_materials; i++)
	{
		m_faceList_length[i] = 0;
	}

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
				parse2floats(&m_textureCoords[m_num_vt*2], line);
				m_num_vt++;
			}
			else if (line.compare(0, 2, "vn") == 0)
			{
				parse3floats(m_vertNormals[m_num_vn], line);
				m_num_vn++;
			}
			else if (line.compare(0, 7, "usemtl ") == 0)
			{
				m_current_faceList = getFaceListIndex(line.substr(7));
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
				parseFace(m_faceLists[m_current_faceList][m_faceList_length[m_current_faceList]], line);
				m_faceList_length[m_current_faceList]++;
			}
		}
	}
}

ObjectLoader::ObjectLoader()
{
}

ObjectLoader::~ObjectLoader()
{
	cout << "~ObjectLoader():" << endl;
	delete[] m_vertices;
	delete[] m_textureCoords;
	delete[] m_vertNormals;
	delete[] m_texture_alias;
	for (int i = 0; i < n_materials; i++)
	{
		delete[] m_faceLists[i];
		//delete[] m_texture_alias[i];
	}
	delete[] m_faceLists;
	//delete[] m_texture_alias;
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

vec3f * ObjectLoader::getVertexNormals()
{
	return m_vertNormals;
}

float * ObjectLoader::getTextureCoords()
{
	return m_textureCoords;
}

Face ** ObjectLoader::getFaceLists()
{
	return m_faceLists;
}

int * ObjectLoader::getFaceListLengths()
{
	return m_faceList_length;
}

int ObjectLoader::getNumberOfFacelists()
{
	return n_materials;
}

string * ObjectLoader::getTextureFileNames()
{
	string* ret = new string[n_materials];
	for (int i = 0; i < n_materials; i++)
	{
		ret[i] = m_texture_alias[i][1];
	}
	return ret;
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

Face & Face::operator=(const Face & other)
{
	delete[] m_facePoint;
	m_facePoint = new FacePoint[3];
	for (int i = 0; i < 3; i++)
	{	
		for (int j = 0; j < 3; j++)
		{
			m_facePoint[i][j] = other.m_facePoint[i][j];
		}
	}
	return *this;
}
								