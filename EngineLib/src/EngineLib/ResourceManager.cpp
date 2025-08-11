#include "EngineLib\ResourceManager.h"

namespace EngineCore {

    ResourceManager& ResourceManager::Instance() {
        static ResourceManager instance;
        return instance;
    }

}