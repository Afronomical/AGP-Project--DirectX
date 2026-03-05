#pragma once
#include "GameObject.h"
#include "MeshManager.h"
#include "MaterialManager.h"

// A GameObject that renders a cube mesh
class Cube : public GameObject
{
public:
    Cube()
	{
    };
    ~Cube()
    {
    };

    //void Start() override;
    void Update(float deltaTime) override;
    

private:
    
};
