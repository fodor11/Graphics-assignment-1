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
}

void parse3floats(vec3f& vector, string line)
{
	string::size_type nextFloat;
	line = line.substr(2);
	vector[0] = stof(line, &nextFloat);

	line = line.substr(nextFloat);
	vector[1] = stof(line, &nextFloat);

	line = line.substr(nextFloat);
	vector[2] = stof(line);
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
				m_num_vt++;
			}
			else if (line.compare(0, 2, "vn") == 0)
			{
				cout << line << endl;
				parse3floats(m_vertNormals[m_num_vn], line);
				cout << string(m_vertNormals[m_num_vn]).c_str() << endl;
				m_num_vn++;
			}
			else if (line.compare(0, 2, "f ") == 0)
			{
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
	delete m_vertices;
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
