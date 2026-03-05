#include "SceneDemo.h"
#include <iostream>
#include "InputManager.h"
#include "RendererD3D.h"

void SceneDemo::Awake()
{

	gameObjects.push_back(cube);
	gameObjects.push_back(cube2);
	gameObjects.push_back(floor);
	//gameObjects.push_back(sphere);
	gameObjects.push_back(lightPoint);
	lightPoint->transform->scl = { 0.1f, 0.1f, 0.1f };

	pointLights.push_back(magentaLight);

	


	Scene::Awake();
}

void SceneDemo::Start()
{
	std::cout << "Overidden Scene Start Call" << std::endl;
	cube2->transform->pos.x = 5.0f;
	cube2->transform->scl.y = 2.0f;

	floor->transform->pos.y = -1.0f;
	floor->transform->scl.x = 10.0f;
	floor->transform->scl.z = 10.0f;
	floor->transform->scl.y = 0.1f;

	lightPoint->transform->pos.y = 5;
	Scene::Start();
}

void SceneDemo::Update()
{
	UpdateFPSText();

	CameraLogic();
	cube2->transform->rot.x += 0.01f;
	cube2->transform->rot.y += 0.01f;
	lightPoint->transform->pos.x = sin(DeltaTime::GetTime()) * 5.0f;
	lightPoint->transform->pos.z = cos(DeltaTime::GetTime()) * 5.0f;
	magentaLight->position = { lightPoint->transform->pos.x, lightPoint->transform->pos.y, lightPoint->transform->pos.z };
	
	Scene::Update();
}

void SceneDemo::CameraLogic()
{
	float sensitivity = XM_2PI * 0.00025f;
	float mouseYMovement = InputManager::GetInstance().GetMouseXY().y * sensitivity;
	camera->yaw += InputManager::GetInstance().GetMouseXY().x * sensitivity;
	
	
	
	float minYViewRange = 0.1f;
	float maxYViewRange = 3.0f;

	if (camera->pitch > minYViewRange && camera->pitch < maxYViewRange)
	{
		camera->pitch += InputManager::GetInstance().GetMouseXY().y * sensitivity;
	}
	else if(camera->pitch >= maxYViewRange)
	{
		camera->pitch = maxYViewRange - 0.000001f;
	}
	else if(camera->pitch <= minYViewRange)
	{
		camera->pitch = minYViewRange + 0.000001f;
	}

	if (InputManager::GetInstance().GetKeyHeld().W)
	{
		camera->x += speed * sin(camera->yaw) * sin(camera->pitch) * DeltaTime::GetDeltaTime();
		camera->y += speed * cos(camera->pitch) * DeltaTime::GetDeltaTime();
		camera->z += speed * cos(camera->yaw) * sin(camera->pitch) * DeltaTime::GetDeltaTime();
	}
	if (InputManager::GetInstance().GetKeyHeld().S)
	{
		camera->x -= speed * sin(camera->yaw) * sin(camera->pitch) * DeltaTime::GetDeltaTime();
		camera->y -= speed * cos(camera->pitch) * DeltaTime::GetDeltaTime();
		camera->z -= speed * cos(camera->yaw) * sin(camera->pitch) * DeltaTime::GetDeltaTime();
	}
	if (InputManager::GetInstance().GetKeyHeld().D)
	{
		camera->x += speed * cos(camera->yaw) * DeltaTime::GetDeltaTime();
		camera->z += speed * (-sin(camera->yaw)) * DeltaTime::GetDeltaTime();
	}
	if (InputManager::GetInstance().GetKeyHeld().A)
	{
		camera->x -= speed * cos(camera->yaw) * DeltaTime::GetDeltaTime();
		camera->z -= speed * (-sin(camera->yaw)) * DeltaTime::GetDeltaTime();
	}
}

void SceneDemo::UpdateFPSText()
{
	static int frameCount = 0;
	static float timeAccumulator = 0.0f;
	static int framesLastSecond = 0;

	frameCount++;
	timeAccumulator += DeltaTime::GetDeltaTime();

	if (timeAccumulator >= 1.0f) {
		framesLastSecond = frameCount;
		frameCount = 0;
		timeAccumulator -= 1.0f;
	}
	
	std::string fpsCounter = std::to_string(framesLastSecond) + "FPS";
	UI->AddText(fpsCounter, -0.7f, 0.5f, 0.05f);

}
