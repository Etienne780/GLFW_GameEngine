#pragma once

namespace EngineCore {
	class Time {
	public:
		static void UpdateTime(double currentTimeSec);

		static double GetTimeSec();
		static double GetDeltaTimeSec();

	private:
		Time();
		static void CalculateDeltaTime(double currentTimeSec);

		static double timeSec;
		static double deltaTimeSec;
	};
}