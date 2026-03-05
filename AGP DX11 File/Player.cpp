#include "Player.h"
#include "DeltaTime.h"
void Player::Awake()
{
	GameObject::Awake();
}

void Player::Start()
{
}

void Player::Update(float deltaTime)
{
	Move(DeltaTime::GetDeltaTimeUnmodified());
}

void Player::LateUpdate(float deltaTime)
{
}

void Player::Move(float deltaTime)
{
	
	float mouseYMovement = InputManager::GetInstance().GetMouseXY().y * sensitivity;
	camera->yaw += InputManager::GetInstance().GetMouseXY().x * sensitivity;



	

	if (camera->pitch > minYViewRange && camera->pitch < maxYViewRange)
	{
		camera->pitch += InputManager::GetInstance().GetMouseXY().y * sensitivity;
	}
	else if (camera->pitch >= maxYViewRange)
	{
		camera->pitch = maxYViewRange - 0.000001f;
	}
	else if (camera->pitch <= minYViewRange)
	{
		camera->pitch = minYViewRange + 0.000001f;
	}

	if (InputManager::GetInstance().GetKeyHeld().W)
	{
		camera->x += moveSpeed * sin(camera->yaw) * sin(camera->pitch) * deltaTime;
		camera->y += moveSpeed * cos(camera->pitch) * deltaTime;
		camera->z += moveSpeed * cos(camera->yaw) * sin(camera->pitch) * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyHeld().S)
	{
		camera->x -= moveSpeed * sin(camera->yaw) * sin(camera->pitch) * deltaTime;
		camera->y -= moveSpeed * cos(camera->pitch) * deltaTime;
		camera->z -= moveSpeed * cos(camera->yaw) * sin(camera->pitch) * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyHeld().D)
	{
		camera->x += moveSpeed * cos(camera->yaw) * deltaTime;
		camera->z += moveSpeed * (-sin(camera->yaw)) * deltaTime;
	}
	if (InputManager::GetInstance().GetKeyHeld().A)
	{
		camera->x -= moveSpeed * cos(camera->yaw) * deltaTime;
		camera->z -= moveSpeed * (-sin(camera->yaw)) * deltaTime;
	}
}
