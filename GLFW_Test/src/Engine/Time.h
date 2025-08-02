#pragma once

namespace EngineCore {
	class Engine;

	class Time {
		friend class Engine;

	public:
		// Gets the time since the programm started in seconds
		static float GetTime();
		// Gets the time diff between current frame and last frame time in seconds
		static float GetDeltaTime();

		// Gets the time since the programm started in seconds
		static double GetTimeDouble();
		// Gets the time diff between current frame and last frame time in seconds
		static double GetDeltaTimeDouble();

	private:
		Time();
		static void UpdateTime(double currentTimeSec);
		static void CalculateDeltaTime(double currentTimeSec);

		static double m_timeSec;
		static double m_deltaTimeSec;
	};
}