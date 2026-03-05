#pragma once
#include "Scene.h"
#include "Cube.h"

class SceneDemo : public Scene
{
public:
	SceneDemo() : Scene("DebugSkybox", false, true) {} 
protected:


    void Awake() override;

    void Start() override;

    void Update()override;

    float speed = 10;

    void CameraLogic();

    Cube* cube = new Cube();
    Cube* cube2 = new Cube();
    Cube* floor = new Cube();
    GameObject* sphere = new GameObject("Sphere");
    GameObject* lightPoint = new GameObject("Sphere");
    

	PointLight* magentaLight = new PointLight
    {
		{ 0.0f, 1.0f, -1.0f }, // position
		{ 0.0f, 0.8f, 0.65f, 1.0f }, // colour
		10.0f, // strength
		true // active
	};
	

    void UpdateFPSText();
    int frameCount = 0;
	float timeAccumulator = 0.0f;
    int framesLastSecond = 0;

};

