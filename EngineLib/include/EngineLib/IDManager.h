#pragma once
#include <queue>


namespace EngineCore {

	class IDManager {
	public:
		IDManager() = default;
		~IDManager() = default;

		bool IsIDFallback();

		/*
		* @brief returns a uniqe int id and ENGINE_INVALID_ID if no free id was found
		*/
		unsigned int GetNewUniqueIdentifier();
		void FreeUniqueIdentifier(unsigned int id);
		/*
		* @brief resets the ID manager
		* @param sets the start value of the next free id
		*/
		void Reset(unsigned int startValue = 0);

	private:
		unsigned int m_idCounter = 0;
		bool m_idFallback = false;// gets set to true when the id limit(Integer.Max) is reached.
		std::queue<unsigned int> m_freeIDs;

		unsigned int GetNewUniqueIdentifierFallback();
	};

}