#pragma once
#include <unordered_map>
#include <typeindex>
#include <string>

#include "CoreLib\Log.h"

struct GLFWwindow;
struct GLFWmonitor;

class UI {
public:
	static void Setup(GLFWwindow* window, GLFWmonitor* primaryMonitor);
	static void Draw();
	static void Shutdown();

    template<typename T>
    static void AddParam(const std::string& name, T* value) {
        params.emplace(name, UIParam(value));
    }

    template<typename T>
    static bool TryGetParam(const std::string& name, T** value) {
        auto it = params.find(name);
        if (it != params.end()) {
            if (it->second.type == std::type_index(typeid(T))) {
                *value = static_cast<T*>(it->second.ptr);
                return true;
            }
            else {
                Log::Warn("Wrong type '{}' should be '{}'", typeid(T).name(), it->second.type.name());
                return false;
            }
        }

        Log::Warn("Parameter '{}' not found!", name);
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

    static std::unordered_map<std::string, UIParam> params;
};