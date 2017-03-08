#define _USE_MATH_DEFINES
#include <math.h>
#include "..\include\camera.hpp"

Camera::Camera()
{
}

Camera::Camera(HeightMapLoader* heightMap)
{
	m_fCurrTime = glutGet(GLUT_ELAPSED_TIME);
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
	
	float maxHeight = m_pHeightMap->getMaxHeight();
	m_fCameraY = m_pHeightMap->getHeight(m_fCameraX, m_fCameraZ) * maxHeight + m_fCameraHeight;
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

void Camera::move()
{
	float elapsedTime = getElapsedTime();
	if (m_bMoveForward)
	{
		m_fCameraX += m_fDirectionX * m_fSpeed * elapsedTime;
		m_fCameraZ += m_fDirectionZ * m_fSpeed * elapsedTime;
	}
	if (m_bMoveBackward)
	{
		m_fCameraX -= m_fDirectionX * m_fSpeed * elapsedTime;
		m_fCameraZ -= m_fDirectionZ * m_fSpeed * elapsedTime;
	}
	if (m_bMoveRight)
	{
		m_fCameraX += cos(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * elapsedTime;
		m_fCameraZ += sin(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * elapsedTime;
	}
	if (m_bMoveLeft)
	{
		m_fCameraX -= cos(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * elapsedTime;
		m_fCameraZ -= sin(m_fRotationAngleRadian + M_PI_2) * m_fSpeed * elapsedTime;
	}
}

float Camera::radianToAngle(float radian)
{
	return radian * (180 / M_PI);
}

float Camera::getElapsedTime()
{
	float now = glutGet(GLUT_ELAPSED_TIME);
	float elapsed = (now - m_fCurrTime) * 0.001f;
	m_fCurrTime = now;
	return elapsed;
}
