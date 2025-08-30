#include "EngineLib/Profiler.h"
#include "EngineLib/Time.h"

namespace EngineCore {

    void Profiler::Start(int fps) {
        if (Time::GetTime() < 2)
            return;

        if (m_fpsList.empty()) {
            m_highestFPS = fps;
            m_lowestFPS = fps;
        }

        if (fps > m_highestFPS) m_highestFPS = fps;
        if (fps < m_lowestFPS) m_lowestFPS = fps;

        if (m_maxSampleCount > 0 && m_fpsList.size() >= m_maxSampleCount) {
            m_fpsList.erase(m_fpsList.begin());
        }

        m_fpsList.push_back(fps);
    }

    void Profiler::SectionStart(const std::string& sectionName) {
        m_currentSection = sectionName;
        m_currentStartTime = Time::GetTimeDouble();
    }

    void Profiler::SectionEnd() {
        double delta = Time::GetTimeDouble() - m_currentStartTime;

        auto& obj = m_sections[m_currentSection];

        if (obj.m_deltaTimes.empty()) {
            obj.m_index = indexCount++;
            obj.m_longestTime = delta;
            obj.m_shortestTime = delta;
        }
        else {
            if (delta > obj.m_longestTime) obj.m_longestTime = delta;
            if (delta < obj.m_shortestTime) obj.m_shortestTime = delta;
        }

        if (m_maxSampleCount > 0 && obj.m_deltaTimes.size() >= m_maxSampleCount) {
            obj.m_deltaTimes.erase(obj.m_deltaTimes.begin());
        }

        obj.m_deltaTimes.push_back(delta);

        m_currentSection.clear();
        m_currentStartTime = 0;
    }


    std::vector<Profiler::Section> Profiler::GetSectionData() {
        std::vector<Profiler::Section> sections;
        sections.reserve(m_sections.size());

        for (auto& [name, obj] : m_sections) {
            Section s;

            s.name = name;
            s.m_longestTime = obj.m_longestTime;
            s.m_shortestTime = obj.m_shortestTime;
            s.m_deltaTimes = obj.m_deltaTimes;

            double sum = 0.0;
            for (auto& delta : obj.m_deltaTimes) {
                sum += delta;
            }

            if (!obj.m_deltaTimes.empty())
                s.average = sum / static_cast<double>(obj.m_deltaTimes.size());
            else
                s.average = 0.0;

            sections.push_back(s);
        }

        return sections;
    }

    std::string Profiler::GetSectionDataString() {
        std::string result;
        auto sections = GetSectionData();
        result += "Highest FPS: " + std::to_string(m_highestFPS) + " \n";
        result += "Lowest FPS: " + std::to_string(m_lowestFPS) + " \n";
        result += "Average FPS: " + std::to_string(GetAverageFPS()) + " \n";
        result += "\n";

        for (const auto& s : sections) {
            result += "Section: " + s.name + "\n";
            result += "  Longest Time: " + std::to_string(s.m_longestTime) + " s\n";
            result += "  Shortest Time: " + std::to_string(s.m_shortestTime) + " s\n";
            result += "  Average Time: " + std::to_string(s.average) + " s\n";
            result += "  Samples: " + std::to_string(s.m_deltaTimes.size()) + "\n";
            result += "\n";
        }

        return result;
    }

    size_t Profiler::GetMaxSampleCount() {
        return m_maxSampleCount;
    }

    float Profiler::GetAverageFPS() {
        if (m_fpsList.empty()) return 0.0f;
        int fpsSum = 0;
        for (int fps : m_fpsList) fpsSum += fps;
        return static_cast<float>(fpsSum) / m_fpsList.size();
    }

    void Profiler::SetMaxSampleCount(size_t sampleCount) {
        m_maxSampleCount = sampleCount;
    }
}