#pragma once
#include <iostream>
#include "heightmap.hpp"
#include "model.hpp"

class Camera
{
public:
	Camera();
	Camera(HeightMapLoader* heightMap);
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

private:
	/// rotation in the Y axis
	float m_fRotationAngleRadian = 0.0f;
	/// horizon angle in radian
	float m_fHorizonAngleRadian = 0.0;

	/// position of the camera
	float m_fCameraX = 0.0f, 
		  m_fCameraY = 0.0f, 
		  m_fCameraZ = 0.0f;
	/// "height" of the camera
	float m_fCameraHeight = 1.f;
	/// camera's direction (vector)
	float m_fDirectionX = 0.0f,
		  m_fDirectionZ = -1.0f;
	
	/// speed of camera
	float m_fSpeed = 5.f;

	float m_fCurrTime = 0.f;

	/// movements
	bool m_bMoveForward = false,
		 m_bMoveBackward = false,
		 m_bMoveLeft = false,
		 m_bMoveRight = false;

	HeightMapLoader* m_pHeightMap;

	void move();
	float radianToAngle(float radian);
	float getElapsedTime();
};
