#include "Time.h"

namespace EngineCore {
	double Time::m_timeSec = 0.0;
	double Time::m_deltaTimeSec = 0.0;
	
	Time::Time() {}

	void Time::UpdateTime(double currentTimeSec) {
		CalculateDeltaTime(currentTimeSec);
		m_timeSec = currentTimeSec;
	}

	void Time::CalculateDeltaTime(double currentTimeSec) {
		m_deltaTimeSec = currentTimeSec - m_timeSec;
	}

	float Time::GetTime() {
		return static_cast<float>(m_timeSec);
	}

	float Time::GetDeltaTime() {
		return static_cast<float>(m_deltaTimeSec);
	}

	double Time::GetTimeDouble() {
		return m_timeSec;
	}

	double Time::GetDeltaTimeDouble() {
		return m_deltaTimeSec;
	}
}