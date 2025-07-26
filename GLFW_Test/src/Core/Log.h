// made by me and chatGPT
// the syntax looks so bad :( 
// this is peak C++ code
// not even Holy C can keep up

#pragma once
#include <iostream>
#include <string>
#include "FormatUtils.h"

using String = std::string;

class Log {
public:
    enum Level : short {
        levelError,
        levelWarning,
        levelInfo,
        levelDebug
    };

    static void ClearLog();

    static bool IsLevelSelected(Level level);

    // Sets what level you want to see
    template<typename... Args>
    static void SetLevel(Args&&... args) {
        static_assert((std::is_same_v<std::decay_t<Args>, Log::Level> && ...), "All arguments must be of type Log::Level.");

        m_levelError = false;
        m_levelWarning = false;
        m_levelInfo = false;
        m_levelDebug = false;

        for (auto a : { args... }) {
            switch (a) {
            case Log::levelError:
                m_levelError = true;
                break;
            case Log::levelWarning:
                m_levelWarning = true;
                break;
            case Log::levelInfo:
                m_levelInfo = true;
                break;
            case Log::levelDebug:
                m_levelDebug = true;
                break;
            }
        }
    }

    // Überladene Methoden für Formatstrings mit variadischen Argumenten
    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Error(T&& format, Args&&... args) {
        if (!m_levelError) return;
        m_print("[ERROR]: " + formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Error(Args&&... args) {
        if (!m_levelError) return;
        m_print("[ERROR]: " + format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Warn(T&& format, Args&&... args) {
        if (!m_levelWarning) return;
        m_print("[WARNING]: " + formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Warn(Args&&... args) {
        if (!m_levelWarning) return;
        m_print("[WARNING]: " + format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Info(T&& format, Args&&... args) {
        if (!m_levelInfo) return;
        m_print("[INFO]: " + formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Info(Args&&... args) {
        if (!m_levelInfo) return;
        m_print("[INFO]: " + format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Debug(T&& format, Args&&... args) {
        if (!m_levelDebug) return;
        m_print("[Debug]: " + formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Debug(Args&&... args) {
        if (!m_levelDebug) return;
        m_print("[Debug]: " + format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Print(T&& format, Args&&... args) {
        m_print(formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Print(Args&&... args) {
        m_print(format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static void Print(Level level, T&& format, Args&&... args) {
        if (!IsLevelSelected(level)) return;
        m_print(formatString(std::forward<T>(format), std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void Print(Level level, Args&&... args) {
        if (!IsLevelSelected(level)) return;
        m_print(format::joinArgs(std::forward<Args>(args)...));
    }

    template<typename T, typename... Args,
        typename = std::enable_if_t<
        std::is_convertible_v<T, String> || std::is_convertible_v<T, const char*>
        >>
    static String GetFormattedString(T&& format, Args&&... args) {
        return formatString(std::forward<T>(format), std::forward<Args>(args)...);
    }

    template<typename... Args>
    static String GetFormattedString(Args&&... args) {
        return format::joinArgs(std::forward<Args>(args)...);
    }

private:
    Log();

    static bool m_levelError;
    static bool m_levelWarning;
    static bool m_levelInfo;
    static bool m_levelDebug;

    // Basis-Version ohne Argumente
    static void m_print(const String& message);

    // String-Formatierung mit Platzhaltern
    template<typename... Args>
    static void m_print(const String& message, Args&&... args) {
        m_print(formatString(message, std::forward<Args>(args)...));
    }

    // Hilfsmethoden für formatString (rekursiv)

    static String formatString(const String& format) {
        return format;
    }

    template<typename T, typename... Args>
    static String formatString(const String& format, T&& value, Args&&... args) {
        size_t pos = format.find("{}");
        if (pos == String::npos) {
            // Kein Platzhalter mehr, String zurückgeben (überschüssige Argumente ignorieren)
            return format;
        }
        String result = format.substr(0, pos) + format::toString(std::forward<T>(value)) + formatString(format.substr(pos + 2), std::forward<Args>(args)...);
        return result;
    }
};