#pragma once

namespace EngineCore {
	class Engine;

	class Time {
		friend class Engine;

	public:
		// Gets the time since the programm started
		static double GetTimeSec();
		// Gets the time diff between current frame and last frame time
		static double GetDeltaTimeSec();

	private:
		Time();
		static void UpdateTime(double currentTimeSec);
		static void CalculateDeltaTime(double currentTimeSec);

		static double timeSec;
		static double deltaTimeSec;
	};
}