#include "DeltaTime.h"

std::chrono::time_point<std::chrono::high_resolution_clock> DeltaTime::startTime;
std::chrono::time_point<std::chrono::high_resolution_clock> DeltaTime::lastTime;
float DeltaTime::timeModifier = 1.0f;
float DeltaTime::deltaTime = 0.0f;

DeltaTime::DeltaTime()
{
	startTime = std::chrono::high_resolution_clock::now();
	timeModifier = 1.0f;
}


DeltaTime::~DeltaTime()
{
}



void DeltaTime::UpdateTime()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - lastTime).count() / 1000000.0f;
	
	lastTime = currentTime;
}