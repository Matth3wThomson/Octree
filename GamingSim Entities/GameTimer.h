#pragma once
#include <Windows.h>

/**
	A timer class for obtaining the time between getTime calls.
*/
class GameTimer
{
public:
	GameTimer(void){
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		lastFrame = start;
	};
	~GameTimer(void){ };

	float GetTime(){
		LARGE_INTEGER t;
		QueryPerformanceCounter(&t);

		float f = (float) ((t.QuadPart - lastFrame.QuadPart) * 1000.0f / frequency.QuadPart);
		lastFrame = t;

		return f;
	}
protected:
	LARGE_INTEGER frequency, lastFrame, start;
};

