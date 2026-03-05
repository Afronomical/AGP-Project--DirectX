#pragma once

#include "Mesh.h"
#include "Transform.h"
#include <memory>
#include "ConstantBuffer.h"


class GameObject
{
private:

	bool isActive = true;
	bool isVisible = true;
public:

	bool IsActive()  { return isActive; }
	void SetActive(bool active) { isActive = active; }
	bool IsVisible() { return isVisible; }
	void SetVisibility(bool visible) { isVisible = visible; }



	Mesh* mesh = nullptr;
	std::string meshName = "Standard";

	

	ConstantBuffer<CBUFFER0>* gameObjectCBuffer = nullptr;

	GameObject() {};
	GameObject(std::string _meshName):meshName(_meshName) {};
	Transform* transform = new Transform;
	
	virtual void Awake();
	virtual void Start();

	virtual void LateUpdate(float deltaTime);
	virtual void Update(float deltaTime);

	virtual void Draw(ID3D11DeviceContext* devCon, CBUFFER0 cbufData);
	virtual void Clean();
	
};

