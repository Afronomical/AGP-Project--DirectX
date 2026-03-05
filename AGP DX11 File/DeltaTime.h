#pragma once
#include <chrono>
#include <iostream>

using namespace std::chrono;


class DeltaTime
{
private:
	typedef system_clock Time;
	static float deltaTime;
	static float timeModifier;
public:
	DeltaTime();
	~DeltaTime();

	DeltaTime(DeltaTime& obj) = delete;


	static std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
	static std::chrono::time_point<std::chrono::high_resolution_clock> lastTime;
	static float GetTime() 
	{
		static float time = 0.0f;
		time += deltaTime;

		return time;
	}
	static float GetDeltaTime()
	{
		if (deltaTime > 0)
		{
			return deltaTime * timeModifier;
		}
		else
		{
			return 0.0f;
		}
	}
	static float GetDeltaTimeUnmodified()
	{
		if (deltaTime > 0)
		{
			return deltaTime;
		}
		else
		{
			return 0.0f;
		}
	}
	static float GetFPS()
	{
		if (deltaTime > 0)
		{
			return 1000.0f / deltaTime;
		}
	}
	static void SetTimeModifier(float modifier)
	{
		if (modifier > 0)
		{
			timeModifier = modifier;
		}
		else
		{
			std::cerr << "Error: Time modifier must be greater than 0." << std::endl;
		}
	}static void SetTimeModifierUnrestricted(float modifier)
	{
		if (modifier > 0)
		{
			timeModifier = modifier;
		}
	}
	static float GetTimeModifier()
	{
		return timeModifier;
	}
	static void UpdateTime();
	//float GetTime();
};

