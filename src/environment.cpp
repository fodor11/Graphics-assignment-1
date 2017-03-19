#include "../include/environment.hpp"

Environment::Environment()
{
}

void Environment::initialize(HeightMapLoader* heightMap, Camera* camera)
{
	m_pCamera = camera;
	heightmap = heightMap;

	pSky = new Sky();
	pSky->initialize();

	forest = new Forest(forestFileName);
	forest->initialize(heightmap);

	tree1 = new Tree(tree1FileName);
	tree2 = new Tree(tree2FileName);
	tree3 = new Tree(tree3FileName);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	//set up fog
	GLfloat fogColor[4] = { 0.12f, 0.12f, 0.12f, 1.0f };
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_DENSITY, 0.03f);
	glFogf(GL_FOG_START, 1.0f);
	glFogf(GL_FOG_END, 150.0f);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
}

void Environment::update()
{
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	pSky->updateSky(m_pCamera->getX(), m_pCamera->getY(), m_pCamera->getZ(), m_pCamera->getElapsedTime());

	std::map<std::string, std::vector<vec3f>>& positions = *forest->getPositionsMap();
	std::string tmpTreeId;
	vec3f tmpPosition;
	for (auto map = positions.begin(); map != positions.end(); map++)
	{
		tmpTreeId = map->first;
		std::vector<vec3f>& actualPositions = map->second;
		for (int j = 0; j < actualPositions.size(); j++)
		{
			tmpPosition = actualPositions[j];
			glPushMatrix();
			glTranslatef(tmpPosition[0], tmpPosition[1], tmpPosition[2]);
			drawTree(determineTree(tmpTreeId), calcDistanceToCamera(tmpPosition));
			glPopMatrix();
		}
	}
}

void Environment::changeAmbientLight(float value)
{
	light_tmp += value;
	if (light_tmp > 1.0f)
	{
		light_tmp = 1.0f;
	}
	else if (light_tmp < 0.0f)
	{
		light_tmp = 0.0f;
	}
	lmodel_ambient[0] = light_tmp;
	lmodel_ambient[1] = light_tmp;
	lmodel_ambient[2] = light_tmp;
}

void Environment::toggleFog()
{
	if (m_bFog)
	{
		glDisable(GL_FOG);
		m_bFog = false;
	}
	else
	{
		glEnable(GL_FOG);
		m_bFog = true;
	}
}

void Environment::toggleMoonlight()
{
	pSky->toggleMoonLight();
}

float Environment::calcDistanceToCamera(vec3f position)
{
	vec3f camPos(m_pCamera->getX(), m_pCamera->getY(), m_pCamera->getZ());
	vec3f dist = camPos - position;
	return dist.length();
}

void Environment::drawTree(Tree* tree, float distanceToCamera)
{
	if (distanceToCamera < visualRange)
	{
		tree->drawTree();
	}
	else
	{
		tree->drawBillBoard();
	}
}

Tree * Environment::determineTree(std::string treeId)
{
	if (strcmp(treeId.c_str(), "tree1") == 0)
	{
		return tree1;
	}
	else if (strcmp(treeId.c_str(), "tree2") == 0)
	{
		return tree2;
	}
	else
	{
		return tree3;
	}
}
