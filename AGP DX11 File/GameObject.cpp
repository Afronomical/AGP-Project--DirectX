#include "GameObject.h"
#include <iostream>
#include "RendererD3D.h"
#include "Window.h"
#include "MeshManager.h"
#include "MaterialManager.h"
void GameObject::Update(float deltaTime)
{

}

void GameObject::Awake()
{
	gameObjectCBuffer = new ConstantBuffer<CBUFFER0>(RendererD3D::GetInstance()->GetDevice());
	mesh = MeshManager::GetInstance()->GetMesh(meshName);
}

void GameObject::Start()
{
}


void GameObject::Clean()
{
}

void GameObject::LateUpdate(float deltaTime)
{
}

void GameObject::Draw(ID3D11DeviceContext* devCon, CBUFFER0 cbufData)
{

	gameObjectCBuffer->Update(devCon, cbufData);


	RendererD3D::GetInstance()->DrawMesh(mesh, gameObjectCBuffer->GetBuffer(), &cbufData, sizeof(CBUFFER0));
}

