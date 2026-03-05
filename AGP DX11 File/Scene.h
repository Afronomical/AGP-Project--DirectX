#pragma once
#include "GameObject.h"
#include "PointLight.h"
#include "Skybox.h"
#include "text2D.h"
#include "DeltaTime.h"
class Scene
{
private:
	bool isFullscreen = false; //Is the window in fullscreen mode
	bool vsyncOn = false;	//Limit fps to the refresh rate of the monitor
	bool focusMouse = true; //is the mouse focused on this window alone
	float FOV = 100.0f; //Field of View for the camera

public:
	
	Scene(std::string nameOfSkyboxMesh, bool _startFullscreen, bool _vsyncOn)
		: skyboxName(nameOfSkyboxMesh),
		isFullscreen(_startFullscreen),
		vsyncOn(_vsyncOn)
	{
	}

	
#pragma once Settings
	bool GetFullScreen() 
	{
		return isFullscreen;
	};
	void SetFullScreen(bool fullscreen) 
	{
		isFullscreen = fullscreen;
	}
	void SetVSync(bool vsync)
	{
		vsyncOn = vsync;
	}
	bool GetVSync()
	{
		return vsyncOn;
	}
	void Draw();
	//Skybox
	std::string skyboxName = "DebugSkybox"; //Default skybox mesh name

	bool isFirstFrame = true; //used to tell if this is the starting frame or not

	void Play();

	std::vector<GameObject*> gameObjects; //List of all game objects in the scene
	std::vector<PointLight*> pointLights; // List of all point Lights in the scene
	
	Camera* GetCamera() //Get this scene's camera from an external class
	{
		return camera; 
	} 
	//Clean Scene
	void CleanScene();

	//UIElements
	Text2D* UI;   //the UI class that handles 2D text rendering

	XMVECTOR* ambientLightCol = new XMVECTOR { 0.5f, 0.5f, 0.5f, 1.0f }; //Ambient light colour
	XMVECTOR* directionalLightDir = new XMVECTOR{ 0.0f, 1.0f, -1.0f, 1.0f }; //Direction of the directional light
	XMVECTOR *directionalLightCol = new XMVECTOR { 0.1f, 0.1f, 0.1f, 1.0f }; //Colour of the directional light
protected:

	virtual void Awake();
	//Start()
	virtual void Start();
	//Update()
	virtual void Update();
	//LateUpdate()
	virtual void LateUpdate();

	Camera* camera = new Camera;
};