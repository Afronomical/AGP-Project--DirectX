#include "Scene.h"
#include <iostream>
#include "RendererD3D.h"


void Scene::Draw()
{
}

void Scene::Play()
{

	// If it's the first frame of the game call both awake and start for all GOs
	if (isFirstFrame) 
	{
		Awake();
		Start();
		isFirstFrame = false;

	}
	else  //Otherwise call all of these lovely update functions
	{
		Update();
		LateUpdate();
	}

}

void Scene::Awake()
{
	UI = new Text2D("font2.png", RendererD3D::GetInstance()->GetDevice(), RendererD3D::GetInstance()->GetDeviceContext());

	std::cout << "Scene AWAKE" << std::endl;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject)
		{
			gameObject->Awake();
		}
	}
}

void Scene::Start()
{

	std::cout << "Scene START " << std::endl;

	for (auto& gameObject : gameObjects)
	{
		if (gameObject)
		{
			gameObject->Start();

		}
	}
}

void Scene::Update()
{
	//std::cout << "Scene UPDATE called and empty" << std::endl;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject)
		{
			gameObject->Update(DeltaTime::GetDeltaTime());

		}
	}
}

void Scene::LateUpdate()
{
	//std::cout << "Scene LATEUPDATE called and empty" << std::endl;
	for (auto& gameObject : gameObjects)
	{
		if (gameObject)
		{
			gameObject->LateUpdate(DeltaTime::GetDeltaTime());

		}
	}
}

void Scene::CleanScene()
{

	for (auto& gameObject : gameObjects)
	{
		if (gameObject)
		{
			gameObject->Clean();
			delete gameObject;
			gameObject = nullptr;
		}
	}


	std::cout << "Scene Cleaned" << std::endl;
	isFirstFrame = true;
}


