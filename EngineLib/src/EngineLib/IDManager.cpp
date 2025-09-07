#include <unordered_set>
#include <queue>
#include <CoreLib/Log.h>

#include "EngineLib/EngineTypes.h"
#include "EngineLib/IDManager.h"

namespace EngineCore {

	bool IDManager::IsIDFallback() {
		return m_idFallback;
	}

	unsigned int IDManager::GetNewUniqueIdentifier() {
		unsigned int id;

		if (m_idFallback) {
			id = GetNewUniqueIdentifierFallback();
		}
		else {
			if (m_idCounter != ENGINE_INVALID_ID) {
				id = m_idCounter++;
			}
			else {
				id = GetNewUniqueIdentifierFallback();
			}
		}

		return id;
	}

	void IDManager::FreeUniqueIdentifier(unsigned int id) {
		m_freeIDs.push(id);
	}

	unsigned int IDManager::GetNewUniqueIdentifierFallback() {
		if (!m_idFallback) {
			m_idFallback = true;
			Log::Warn("IDManager: Max ID limit reached, using fallback IDs from free pool");
		}

		if (!m_freeIDs.empty()) {
			auto id = m_freeIDs.front();
			m_freeIDs.pop();
			return id;
		}

		Log::Warn("IDManager: Cant find any free IDs, free pool is empty!");
		return ENGINE_INVALID_ID;
	}

	void IDManager::Reset(unsigned int startValue) {
		std::queue<unsigned int> empty;
		std::swap(m_freeIDs, empty);
		m_idCounter = startValue;
		m_idFallback = false;
	}

}