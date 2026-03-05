#pragma once
#include "GameObject.h"
#include "Camera.h"
#include "InputManager.h"
class Player : public GameObject
{

private:
	float moveSpeed = 10.0f; // Speed of the player movement
	Camera* camera;
	float sensitivity = XM_2PI * 0.00025f; // camerra sensitivity

	float minYViewRange = 0.1f; //minimum view angle (how high up you can look)
	float maxYViewRange = 3.0f; // maximum view angle (how low down you can look)
public:
	Player(Camera* cam) : camera(cam), GameObject() { SetVisibility(false); }
	~Player() {}
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;

	void Move(float deltaTime);
};

