#pragma once
#include <iostream>
#include <string.h>
#include "model.hpp"

using namespace std;


class Snowman
{
public:
	Snowman(std::string name);
	~Snowman();
	void createDispList();
	void build();
private:
	GLuint m_dispList;
	string m_name;
};
