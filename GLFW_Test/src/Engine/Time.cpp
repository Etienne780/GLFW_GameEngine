#include "Time.h"

namespace EngineCore {
	double Time::timeSec = 0.0;
	double Time::deltaTimeSec = 0.0;
	
	Time::Time() {}

	void Time::UpdateTime(double currentTimeSec) {
		CalculateDeltaTime(currentTimeSec);
		timeSec = currentTimeSec;
	}

	void Time::CalculateDeltaTime(double currentTimeSec) {
		deltaTimeSec = currentTimeSec - timeSec;
	}

	double Time::GetTimeSec() {
		return timeSec;
	}

	double Time::GetDeltaTimeSec() {
		return deltaTimeSec;
	}
}