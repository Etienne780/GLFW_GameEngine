#pragma once

namespace EngineCore {
	class Time {
	public:
		Time();
		void UpdateTime(double currentTimeSec);

		double GetTimeSec() const;
		double GetDeltaTimeSec() const;

	private:
		void CalculateDeltaTime(double currentTimeSec);

		double timeSec = 0.0;
		double deltaTimeSec = 0.0;
	};
}