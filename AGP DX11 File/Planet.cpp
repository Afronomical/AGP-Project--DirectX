#include "Planet.h"

void Planet::Awake()
{
	GameObject::Awake();

}

void Planet::Start()
{
}

void Planet::Update(float deltaTime)
{
	Orbit(deltaTime);
	Rotate(deltaTime);
}

void Planet::LateUpdate(float deltaTime)
{
}

void Planet::Orbit(float deltaTime)
{
	static float time ;	
	time += deltaTime; // Increment time by deltaTime
	if (target)
	{
		transform->pos.x = target->transform->pos.x + orbitRadius * sin(time * orbitSpeed);
		transform->pos.z = target->transform->pos.z + orbitRadius * cos(time * orbitSpeed);
		transform->pos.y = target->transform->pos.y; // Keep the same height as the target
	}
	
}

void Planet::Rotate(float deltaTime)
{
	transform->rot.y += rotationSpeed * deltaTime;
	if (transform->rot.y > XM_2PI) transform->rot.y -= XM_2PI;
}
