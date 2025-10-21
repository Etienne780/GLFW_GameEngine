#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <chrono>
#include <stack>

namespace EngineCore {

    class Profiler {
    public:
        struct Section {
            std::string m_name;
            double longestTime = 0;
            double shortestTime = 0;
            double average = 0;
            int indentLevel = 0;
            std::vector<double> deltaTimes;
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
        using Clock = std::chrono::high_resolution_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        struct SectionObj {
            unsigned int m_index = 0;
            int m_indentLevel = 0;
            double m_longestTime = 0;
            double m_shortestTime = 0;
            std::vector<double> m_deltaTimes;
        };

        static inline int m_highestFPS = 0;
        static inline int m_lowestFPS = 0;
        static inline std::vector<int> m_fpsList;

        static inline size_t m_maxSampleCount = 500;
        static inline unsigned int indexCount = 0;

        struct ActiveSection {
            unsigned int index = 0;
            std::string name;
            TimePoint startTime;
        };

        static inline std::vector<ActiveSection> m_sectionStack;
        static inline std::unordered_map<std::string, SectionObj> m_sections;

        static inline TimePoint m_profilerStartTime;
    };

}