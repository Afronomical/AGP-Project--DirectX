#pragma once
#include "Scene.h"
#include "Planet.h"
#include "Player.h"
class SpaceScene : public Scene
{

	
public:
	SpaceScene() : Scene("Space", false, true) {} 

	void Awake() override;

	void Start() override;

	void Update()override;

	Player* player = new Player(camera);

	Planet* sun = new Planet(nullptr, "Sun", 0, 0, 0.1f,2);

	Planet* mercury = new Planet(sun, "Mercury", 10, 0.1f, 0.2f, 0.1f);


	Planet* venus = new Planet(sun, "Venus", 15, 0.03f, 0.7, 0.2);

	Planet* earth = new Planet(sun, "Earth", 20, 0.025f, 1, 0.25 );

	Planet* moon = new Planet(earth, "Moon", 2, 0.5f, 0.6f, 0.08f);

	Planet* mars = new Planet(sun, "Mars",	 25, 0.06f, 0.5, 0.2);

	Planet* jupiter = new Planet(sun, "Jupiter", 30, 0.01f, 0.5f, 0.8f);

	Planet* saturn = new Planet(sun, "Saturn", 40, 0.008f, 0.4f, 0.75f);

	Planet* neptune = new Planet(sun, "Neptune", 55, 0.05f, 0.7, 0.55f);

	Planet* uranus = new Planet(sun, "Uranus", 46, 0.02f, 0.3f, 0.6f);


	bool isDirectionalLightOn = false;

	void UpdateFPSText();
	int frameCount = 0;
	float timeAccumulator = 0.0f;
	int framesLastSecond = 0;

	PointLight* sunLight = new PointLight
	{
		{ sun->transform->pos.x, sun->transform->pos.y, sun->transform->pos.z }, // position
		{ 1.0f, 1.0f, 0.6f, 1.0f }, // colour
		500.0f, // strength
		true // active
	};
	void ChangeTimeScale();
	bool showSettings = false;
	bool showFPS = false;
	void SettingsUpdate();
	void SettingsUIText();

	void LightingSettings();

	void SetDirectionalLight(bool isOn);
	
};

