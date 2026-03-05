#pragma once
#include "GameObject.h"
class Planet : public GameObject
{

private:
	float orbitRadius = 10.0f; //Distance from the target object
	float orbitSpeed = 0.5f; //Speed of orbiting around the target object
	float rotationSpeed = 0.0;

	GameObject* target; //the object this orbits around, e.g. a sun or star

public:
	Planet(GameObject* orbitTarget, std::string planetMeshName, float orbitDistance, float orbitingSpeed, float rotationSpeed,  float scale)
		: target(orbitTarget), planetName(planetMeshName), GameObject(planetMeshName)
	{
		orbitRadius = orbitDistance;
		orbitSpeed = orbitingSpeed;
		this->rotationSpeed = rotationSpeed;
		transform->scl = { scale, scale, scale };
	}
	std::string planetName;
	void Awake() override;
	void Start() override;
	void Update(float deltaTime) override;
	void LateUpdate(float deltaTime) override;
	void SetTarget(GameObject* targetObject) { target = targetObject; }
	void Orbit(float deltaTime);
	void Rotate(float deltaTime);
	
};

