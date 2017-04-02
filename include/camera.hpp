#pragma once
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <math.h>
#include <iostream>
#ifndef __linux__
#include <Windows.h>
#endif
#ifdef __linux__
#define LARGE_INTEGER long
#endif
#include "heightmap.hpp"
#include "model.hpp"

class Camera
{
public:
	/// starts timer, gets heightmap, sets the middle of the map
	Camera(HeightMapLoader* heightMap);
	/// empty
	~Camera();
	/// moves the camera
	void updateCamera();
	/// sets the boolean movement members to false
	void resetMovements();
	/// sets the m_bMoveForward to true
	void setForwardMovement();
	/// sets the m_bMoveBackward to true
	void setBackwardMovement();
	/// sets the m_bMoveLeft to true
	void setLeftMovement();
	/// sets the m_bMoveRight to true
	void setRightMovement();
	/// adds the rotation on the XZ plane to the current rotation
	void addRotationInRadian(float addRotationAngle);
	/// adds the horizontal angle to the current horizontal angle
	void changeHorizonInRadian(float addHorizonAngle);
	/// returns the X coordinate of the camera position
	float getX() const;
	/// returns the Y coordinate of the camera position
	float getY() const;
	/// returns the Z coordinate of the camera position
	float getZ() const;
	/// returns the height of the camera
	float getCameraHeight() const;
	/// returns elapsed time
	float getElapsedTime() const;

	void startTimer();
private:
	/// rotation in the Y axis
	float m_fRotationAngleRadian = -M_PI_2;
	/// horizon angle in radian
	float m_fHorizonAngleRadian = 0.0f;

	/// position of the camera
	float m_fCameraX = 0.0f, 
		  m_fCameraY = 1.0f, 
		  m_fCameraZ = 0.0f;
	/// "height" of the camera
	float m_fCameraHeight = 1.f;
	/// camera's direction (vector)
	float m_fDirectionX = 0.0f,
		  m_fDirectionZ = -1.0f;
	
	/// speed of camera
	float m_fSpeed = 0.05f;

	/// movements
	bool m_bMoveForward = false,
		 m_bMoveBackward = false,
		 m_bMoveLeft = false,
		 m_bMoveRight = false;

	HeightMapLoader* m_pHeightMap;

	void move();
	float radianToAngle(float radian);

	LARGE_INTEGER m_liPrevTime, m_liCurrTime, m_liFrequency;
	float m_fElapsedTime = 0.0f;
	float calcElapsedTime();
};
