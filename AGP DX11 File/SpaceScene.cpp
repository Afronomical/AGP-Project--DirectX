#include "SpaceScene.h"

void SpaceScene::Awake()
{
	gameObjects.push_back(player);
	gameObjects.push_back(sun);
	gameObjects.push_back(earth);
	gameObjects.push_back(mars);
	gameObjects.push_back(neptune);
	gameObjects.push_back(venus);
	gameObjects.push_back(moon);
	gameObjects.push_back(mercury);
	gameObjects.push_back(jupiter);
	gameObjects.push_back(saturn);
	gameObjects.push_back(uranus);


	

	pointLights.push_back(sunLight);
	Scene::Awake();
}

void SpaceScene::Start()
{
	sun->transform->pos = { 0.0f, -10.0f, 10.0f };
	ambientLightCol = new XMVECTOR{ 0.1f, 0.1f, 0.1f, 1.0f };
	Scene::Start();
}

void SpaceScene::Update()
{
	sun->transform->pos.x += 0.5f * DeltaTime::GetDeltaTime();

	sunLight->position = { sun->transform->pos.x, sun->transform->pos.y, sun->transform->pos.z };
	
	ChangeTimeScale();
	SettingsUpdate();
	Scene::Update();
}








void SpaceScene::UpdateFPSText()
{
	if (showFPS)
	{
		static int frameCount = 0;
		static float timeAccumulator = 0.0f;
		static int framesLastSecond = 0;

		frameCount++;
		timeAccumulator += DeltaTime::GetDeltaTimeUnmodified();

		if (timeAccumulator >= 1.0f) {
			framesLastSecond = frameCount;
			frameCount = 0;
			timeAccumulator -= 1.0f;
		}

		std::string fpsCounter = std::to_string(framesLastSecond) + "FPS";
		UI->AddText(fpsCounter, -1.0f, 0.5f, 0.04f);
	}
	
}

void SpaceScene::ChangeTimeScale()
{
	if (InputManager::GetInstance().GetKeyHeld().E)
	{
		DeltaTime::SetTimeModifier(DeltaTime::GetTimeModifier() + 0.1f);

	}
	if (InputManager::GetInstance().GetKeyHeld().Q)
	{
		if (DeltaTime::GetTimeModifier() > 0.1f)
		{
			DeltaTime::SetTimeModifier(DeltaTime::GetTimeModifier() - 0.1f);
		}
		else
		{
			DeltaTime::SetTimeModifier(0.1f);
		}
	}
}

void SpaceScene::SettingsUpdate()
{
	if (InputManager::GetInstance().GetKeyPressed().F11)
	{
		SetFullScreen(!GetFullScreen());
	}
	if (InputManager::GetInstance().GetKeyPressed().F9)
	{
		SetVSync(!GetVSync());
	}
	if (InputManager::GetInstance().GetKeyPressed().M)
	{
		showSettings = !showSettings;
	}
	if (InputManager::GetInstance().GetKeyPressed().F10)
	{
		showFPS = !showFPS;
	}
	if (InputManager::GetInstance().GetKeyPressed().Space)
	{
		isDirectionalLightOn = !isDirectionalLightOn;
	}
	LightingSettings();
	SettingsUIText();
}

void SpaceScene::SettingsUIText()
{

	UI->AddText("M toggle help", -1.0f, 1, 0.05f);
	UpdateFPSText();

	if (showSettings) 
	{
		UI->AddText("F11 Fullscreen", -1.0f, 0.85f, 0.04f);

				if (GetFullScreen())
				{
					UI->AddText("Fullscreen", -1.0f, 0.8f, 0.04f);
				}
				else
				{
					UI->AddText("Windowed", -1.0f, 0.8f, 0.04f);
				}


		UI->AddText("F9 Toggle Vsync", -1.0f, 0.7f, 0.04f);

				if (GetVSync())
				{
					UI->AddText("VsyncON", -1.0f, 0.65f, 0.04f);
				}
				else
				{
					UI->AddText("VsyncOFF", -1.0f, 0.65f, 0.04f);
				}


		UI->AddText("F10 Show FPS", -1.0f, 0.55f, 0.04f);
		UI->AddText("Q slow time", -1.0f, 0.45f, 0.04f);
		UI->AddText("E speed up time", -1.0f, 0.40f, 0.04f);
		UI->AddText("Time Scale: " + std::to_string(DeltaTime::GetTimeModifier()), -1.0f, 0.35f, 0.04f);

		UI->AddText("SPACE toggle directional light", -1.0f, 0.3f, 0.04f);
		if (isDirectionalLightOn)
		{
			UI->AddText("Directional Light: ON", -1.0f, 0.25f, 0.04f);
		}
		else
		{
			UI->AddText("Directional Light: OFF", -1.0f, 0.25f, 0.04f);
		}
	}
		UI->AddText("WASD to move", 0.3f, 1.0, 0.04f);
	

	

}

void SpaceScene::LightingSettings()
{
	if (isDirectionalLightOn)
	{
		
		directionalLightCol = new XMVECTOR{ 1.0f, 0.0f, 0.5f, 1.0f };
	}
	else
	{
		directionalLightCol = new XMVECTOR{ 0.0f, 0.0f, 0.0f, 1.0f };
	}
}


