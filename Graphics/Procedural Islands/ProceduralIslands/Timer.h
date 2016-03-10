#ifndef TIMER_H
#define TIMER_H

class Timer
{
	unsigned int elapsedTime;
	unsigned int startTime;
	unsigned int durration;

	bool loopState;
	bool pauseState;

	unsigned int GetProgramTime();
	unsigned int GetTotalTime();

public:
	Timer(bool looping, float interval);

	void PauseTimer();

	void UnpauseTimer();

	float GetElapsed();

	float GetAlpha();

	float TimeLeft();

	bool IsPassed();

	bool IsPaused();

	void Reset();
};

#endif