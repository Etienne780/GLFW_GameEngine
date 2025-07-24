#include "Time.h"

namespace EngineCore {
	Time::Time() {}

	void Time::UpdateTime(double currentTimeSec) {
		CalculateDeltaTime(currentTimeSec);
		timeSec = currentTimeSec;
	}

	void Time::CalculateDeltaTime(double currentTimeSec) {
		deltaTimeSec = currentTimeSec - timeSec;
	}

	double Time::GetTimeSec() const {
		return timeSec;
	}

	double Time::GetDeltaTimeSec() const {
		return deltaTimeSec;
	}
}