#pragma once
#include <unordered_map>
#include <typeindex>
#include <string>

#include "CoreLib\Log.h"

struct GLFWwindow;
struct GLFWmonitor;

class UI {
public:
    enum UIParamName;

	static void Setup(GLFWwindow* window, GLFWmonitor* primaryMonitor);
	static void Draw();
	static void Shutdown();

    template<typename T>
    static void AddParam(UIParamName paramName, T* value) {
        params.emplace(paramName, UIParam(value));
    }

    template<typename T>
    static bool TryGetParam(UIParamName paramName, T* value) {
        auto it = params.find(paramName);
        if (it != params.end()) {
            if (it->second.type == std::type_index(typeid(T))) {
                *value = *static_cast<T*>(it->second.ptr);
                return true;
            }
            else {
                Log::Warn("Wrong type '{}' should be '{}'", typeid(T).name(), it->second.type.name());
                return false;
            }
        }

        Log::Warn("Parameter (ID: {}) doesn't exist! ", static_cast<int>(paramName));
        return false;
    }

private:
    struct UIParam {
        void* ptr;             // Pointer to data
        std::type_index type;  // Type info

        template<typename T>
        UIParam(T* value)
            : ptr(static_cast<void*>(value)),
            type(std::type_index(typeid(T))) {
        }
    };

    static std::unordered_map<UIParamName, UIParam> params;
};