#pragma once
#include "sky.hpp"
#include "object.hpp"
#include "forest.hpp"
#include "camera.hpp"
#include "dynamicFog.hpp"
#include "rain.hpp"

class Environment
{
public:
	Environment();
	~Environment();
	/// reads and prepares objects and textures (sky, forest, trees, terrain)
	void initialize(HeightMapLoader* heightMap, Camera* camera);
	/// draws everything
	void update();
	/// adds the value to the overall lightness
	void changeAmbientLight(float value);
	/// toggles moonlight on and off
	void toggleMoonlight();
	/// toggles fog on and off
	void toggleFog();
private:
	DynamicFog *m_fog;
	Rain *rain;
	Sky *pSky;
	Tree *tree1;
	std::string tree1FileName = "pine1";
	Tree *tree2;
	std::string tree2FileName = "pine2";
	Tree *tree3;
	std::string tree3FileName = "pine3";
	Forest* forest;
	std::string forestFileName = "population.png";

	float visualRange = 6.5f;

	float light_tmp = 0.1;
	GLfloat lmodel_ambient[4] = { light_tmp, light_tmp, light_tmp, 1.0 };

	/// pointers to foreign objects
	HeightMapLoader* heightmap;
	Camera* m_pCamera;

	float calcDistanceToCamera(vec3f position);
	void drawTree(Tree* tree, float distanceToCamera);
	Tree* determineTree(std::string treeId);
};