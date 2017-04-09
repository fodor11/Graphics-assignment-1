#include "../include/camera.hpp"

Camera::Camera(HeightMapLoader* heightMap)
{
#ifndef __linux__
	QueryPerformanceFrequency(&m_liFrequency);
#endif

	m_pHeightMap = heightMap;
	m_fCameraX = (heightMap->getImageWidth()*heightMap->getScale()) / 2;
	m_fCameraZ = (heightMap->getImageHeight()*heightMap->getScale()) / 2;
}

Camera::~Camera(){}

void Camera::updateCamera()
{
	move();

	//set horizon (+change from radius to angle)
	glRotatef(radianToAngle(m_fHorizonAngleRadian), 1, 0, 0);
	
	m_fCameraY = m_pHeightMap->getHeight(m_fCameraX, m_fCameraZ) + m_fCameraHeight;
	//set the camera
	gluLookAt(m_fCameraX, m_fCameraY, m_fCameraZ,                              //camera position
		m_fCameraX + m_fDirectionX, m_fCameraY, m_fCameraZ + m_fDirectionZ,    //look at point
		0.0f, 0.1f, 0.0f);                                                     //up vector
}

void Camera::resetMovements()
{
	m_bMoveBackward = false;
	m_bMoveForward = false;
	m_bMoveLeft = false;
	m_bMoveRight = false;
}

void Camera::setForwardMovement()
{
	m_bMoveForward = true;
}

void Camera::setBackwardMovement()
{
	m_bMoveBackward = true;
}

void Camera::setLeftMovement()
{
	m_bMoveLeft = true;
}

void Camera::setRightMovement()
{
	m_bMoveRight = true;
}

void Camera::addRotationInRadian(float addRotationAngle)
{
	m_fRotationAngleRadian += addRotationAngle;
	m_fDirectionX = cos(m_fRotationAngleRadian);
	m_fDirectionZ = sin(m_fRotationAngleRadian);
	if (m_fRotationAngleRadian > (M_PI * 2) || m_fRotationAngleRadian < -(M_PI * 2))
	{
		m_fRotationAngleRadian = fmodf(m_fRotationAngleRadian, M_PI * 2);
	}
}

void Camera::changeHorizonInRadian(float addHorizonAngle)
{
	m_fHorizonAngleRadian += addHorizonAngle;
	if (m_fHorizonAngleRadian < -M_PI_2)
	{
		m_fHorizonAngleRadian = -M_PI_2;
	}
	else if (m_fHorizonAngleRadian > M_PI_2)
	{
		m_fHorizonAngleRadian = M_PI_2;
	}
}

float Camera::getX() const
{
	return m_fCameraX;
}

float Camera::getY() const
{
	return m_fCameraY;
}

float Camera::getZ() const
{
	return m_fCameraZ;
}

float Camera::getCameraHeight() const
{
	return m_fCameraHeight;
}

float Camera::getElapsedTime() const
{
	return m_fElapsedTime;
}

void Camera::setObstacles(std::map<std::string, std::vector<vec3f>>* obstaclePositions)
{
	m_pObstaclePositions = obstaclePositions;
}

void Camera::startTimer()
{
#ifndef __linux__
	QueryPerformanceCounter(&m_liCurrTime);
#endif
}

void Camera::move()
{
	//float prevTime = m_fElapsedTime;
	//// smoothes movement in case of unstable fps
	//m_fElapsedTime = (prevTime + calcElapsedTime()) * 0.5f;
	m_fElapsedTime = calcElapsedTime();
	m_fPrevCameraX = m_fCameraX;
	m_fPrevCameraZ = m_fCameraZ;

	if (m_bMoveForward)
	{
		m_fCameraX += m_fDirectionX * m_fSpeed * m_fElapsedTime;
		m_fCameraZ += m_fDirectionZ * m_fSpeed * m_fElapsedTime;
	}
	if (m_bMoveBackward)
	{
		m_fCameraX -= m_fDirectionX * m_fSpeed * m_fElapsedTime;
		m_fCameraZ -= m_fDirectionZ * m_fSpeed * m_fElapsedTime;
	}
	if (m_bMoveRight)
	{
		m_fCameraX += cos(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * m_fElapsedTime;
		m_fCameraZ += sin(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * m_fElapsedTime;
	}
	if (m_bMoveLeft)
	{
		m_fCameraX -= cos(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * m_fElapsedTime;
		m_fCameraZ -= sin(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * m_fElapsedTime;
	}
	if (checkCollisions())
	{
		m_fCameraX = m_fPrevCameraX;
		m_fCameraZ = m_fPrevCameraZ;
	}
}
float Camera::calc2Ddistance(float point1x, float point1y, float point2x, float point2y)
{
	return sqrtf( powf(point2x - point1x, 2.0f) + powf(point2y - point1y, 2.0f));
}
bool Camera::checkCollisions()
{
	if (m_pObstaclePositions != nullptr)
	{
		for (auto obstacle = m_pObstaclePositions->begin(); obstacle != m_pObstaclePositions->end(); obstacle++)
		{
			std::vector<vec3f>&  positions = obstacle->second;
			for (vec3f& position : positions)
			{
				if (calc2Ddistance(m_fCameraX, m_fCameraZ, position.x(), position.z()) < m_fObstacleRadius)
				{
					return true;
				}
			}
		}
	}
	return false;
}

float Camera::radianToAngle(float radian)
{
	return radian * (180 / M_PI);
}

float Camera::calcElapsedTime()
{
#ifndef __linux__
	m_liPrevTime = m_liCurrTime;
	QueryPerformanceCounter(&m_liCurrTime);
	float elapsed = (m_liCurrTime.QuadPart - m_liPrevTime.QuadPart) * 100.f / m_liFrequency.QuadPart;
#else
    static int prevTime = glutGet(GLUT_ELAPSED_TIME);
    int currentTime = glutGet(GLUT_ELAPSED_TIME);
    float elapsed = (float)(currentTime - prevTime) / 1000.0;
    prevTime = currentTime;
#endif
	if (elapsed < 0.01)
	{
		std::cout << "elapsed time < 0.01" << std::endl;
	}
	return elapsed;
}
