#include <CoreLib/Log.h>

#include "EngineLib/Profiler.h"
#include <sstream>
#include <iomanip>

namespace EngineCore {

    void Profiler::Start(int fps) {
        // Initialize start time once
        if (m_fpsList.empty()) {
            m_profilerStartTime = Clock::now();
            m_highestFPS = fps;
            m_lowestFPS = fps;
        }

        // Only start recording after ~2 seconds uptime
        const auto elapsed = std::chrono::duration<double>(Clock::now() - m_profilerStartTime).count();
        if (elapsed < 2.0)
            return;

        if (fps > m_highestFPS) m_highestFPS = fps;
        if (fps < m_lowestFPS) m_lowestFPS = fps;

        if (m_maxSampleCount > 0 && m_fpsList.size() >= m_maxSampleCount) {
            m_fpsList.erase(m_fpsList.begin());
        }

        m_fpsList.push_back(fps);
    }

    void Profiler::SectionStart(const std::string& sectionName) {
        ActiveSection section;
        section.index = indexCount++;
        section.name = sectionName;
        section.startTime = Clock::now();
        m_sectionStack.push_back(section);
    }

    void Profiler::SectionEnd() {
        if (m_sectionStack.empty()) {
            Log::Error("Profiler: SectionEnd() called without a matching SectionStart()!");
            return;
        }

        const auto endTime = Clock::now();
        const ActiveSection section = m_sectionStack.back();
        m_sectionStack.pop_back();

        const double delta = std::chrono::duration<double>(endTime - section.startTime).count();
        auto& obj = m_sections[section.name];

        if (obj.m_deltaTimes.empty()) {
            obj.m_index = section.index;
            obj.m_indentLevel = static_cast<int>(m_sectionStack.size());
            obj.m_longestTime = delta;
            obj.m_shortestTime = delta;
        }
        else {
            if (delta > obj.m_longestTime) obj.m_longestTime = delta;
            if (delta < obj.m_shortestTime) obj.m_shortestTime = delta;
        }

        if (m_maxSampleCount > 0 && obj.m_deltaTimes.size() >= m_maxSampleCount)
            obj.m_deltaTimes.erase(obj.m_deltaTimes.begin());

        obj.m_deltaTimes.push_back(delta);
    }

    std::vector<Profiler::Section> Profiler::GetSectionData() {
        std::vector<Profiler::Section> sections;
        sections.reserve(m_sections.size());

        for (auto& [name, obj] : m_sections) {
            Section s;
            s.m_name = name;
            s.longestTime = obj.m_longestTime;
            s.shortestTime = obj.m_shortestTime;
            s.deltaTimes = obj.m_deltaTimes;
            s.indentLevel = obj.m_indentLevel;

            double sum = 0.0;
            for (auto& delta : obj.m_deltaTimes)
                sum += delta;

            s.average = !obj.m_deltaTimes.empty() ? sum / obj.m_deltaTimes.size() : 0.0;
            sections.push_back(s);
        }

        // Sort sections by insertion order (m_index)
        std::sort(sections.begin(), sections.end(),
            [&](const Section& a, const Section& b) {
                return m_sections[a.m_name].m_index < m_sections[b.m_name].m_index;
            });

        return sections;
    }


    std::string Profiler::GetSectionDataString() {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(6);
        ss << "Highest FPS: " << m_highestFPS << "\n";
        ss << "Lowest FPS: " << m_lowestFPS << "\n";
        ss << "Average FPS: " << GetAverageFPS() << "\n\n";

        size_t count = 0;
        std::vector<Profiler::Section> sections = GetSectionData();
        for (const auto& s : sections) {
            std::string indent = "";
            for (int i = 0; i < s.indentLevel; i++) {
                indent = "|  ";
            }

            if (count < sections.size() && count > 0)
                ss << indent << "|  " << "\n";
            ss << indent << ((s.indentLevel != 0) ? "|-" : "") << "Section: " << s.m_name << "\n";
            ss << indent << "|- Longest Time:  " << s.longestTime << "s\n";
            ss << indent << "|- Shortest Time: " << s.shortestTime << "s\n";
            ss << indent << "|- Average Time:  " << s.average << "s\n";
            ss << indent << "|- Samples:       " << s.deltaTimes.size() << "\n";

            count++;
        }

        return ss.str();
    }

    size_t Profiler::GetMaxSampleCount() {
        return m_maxSampleCount;
    }

    float Profiler::GetAverageFPS() {
        if (m_fpsList.empty()) return 0.0f;
        int fpsSum = 0;
        for (int fps : m_fpsList) fpsSum += fps;
        return static_cast<float>(fpsSum) / static_cast<float>(m_fpsList.size());
    }

    void Profiler::SetMaxSampleCount(size_t sampleCount) {
        m_maxSampleCount = sampleCount;
    }

}