#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace EngineCore {
	
	class Profiler {
	public:
		struct Section {
			std::string name;
			double m_longestTime;
			double m_shortestTime;
			double average;
			std::vector<double> m_deltaTimes;
		};

		static void Start(int fps);
		static void SectionStart(const std::string& sectionName);
		static void SectionEnd();

		static std::vector<Section> GetSectionData();
		static std::string GetSectionDataString();
		static size_t GetMaxSampleCount();
		static float GetAverageFPS();

		static void SetMaxSampleCount(size_t sampleCount);

	private:
		struct SectionObj {
			unsigned int m_index = 0;
			double m_longestTime = 0;
			double m_shortestTime = 0;
			std::vector<double> m_deltaTimes;
		};

		static inline int m_highestFPS = 0;
		static inline int m_lowestFPS = 0;
		static inline std::vector<int> m_fpsList;

		static inline size_t m_maxSampleCount = 500;
		static inline unsigned int indexCount = 0;
		static inline std::string m_currentSection;
		static inline double m_currentStartTime;
		static inline std::unordered_map<std::string, SectionObj> m_sections;
	};

}