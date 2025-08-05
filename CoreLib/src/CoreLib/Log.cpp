#include "CoreLib\Log.h"

bool Log::m_levelError = true;
bool Log::m_levelWarning = true;
bool Log::m_levelInfo = true;
bool Log::m_levelDebug = true;

void Log::ClearLog() {
    system("cls");
}

void Log::m_print(const std::string& message) {
    std::cout << message << std::endl;
}

bool Log::IsLevelSelected(Level level) {
    switch (level) {
    case Level::levelError: return m_levelError;
    case Level::levelWarning: return m_levelWarning;
    case Level::levelInfo: return m_levelInfo;
    case Level::levelDebug: return m_levelDebug;
    default: return false;
    }
}
