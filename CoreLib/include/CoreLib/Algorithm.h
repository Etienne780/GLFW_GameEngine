#pragma once
#include <vector>
#include <functional>
#include <memory>
/**
* is not thread save
*/
namespace Algorithm {

	namespace Search {

		#pragma region Binary

		/**
		* @brief Gets an element by a specific ID from a sorted list using binary search
		* @tparam T The type of elements stored in the list
		* @param list Reference to a sorted vector containing elements of type T
		* @param getID Function that returns the ID of a given element
		* @param searchID The ID to search for
		* @return Pointer to the found element, or nullptr if not found
		*/
		template<typename T>
		inline T* GetBinary(std::vector<T>& list, std::function<unsigned int(const T&)> getID, unsigned int searchID) {
			if (list.empty()) 
				return nullptr;

			size_t startIndex = 0;
			size_t endIndex = list.size() - 1;

			while (startIndex <= endIndex) {
				size_t mid = startIndex + (endIndex - startIndex) / 2;
				T& element = list[mid];
				unsigned int id = getID(element);

				if (id == searchID) {
					return &element;
				}
				else if (id > searchID) {
					if (mid == 0) break;
					endIndex = mid - 1;
				}
				else {
					startIndex = mid + 1;
				}
			}

			return nullptr;
		}

		/**
		* @brief Gets an element by a specific ID from a sorted list of unique_ptr using binary search
		* @tparam T The type of the managed elements
		* @param list Reference to a sorted vector of unique_ptr<T>
		* @param getID Function that returns the ID of a given element
		* @param searchID The ID to search for
		* @return Pointer to the found element (non-owning), or nullptr if not found
		*/
		template<typename T>
		inline T* GetBinary(
			std::vector<std::unique_ptr<T>>& list,
			std::function<unsigned int(const T&)> getID,
			unsigned int searchID
		) {
			if (list.empty()) 
				return nullptr;

			size_t startIndex = 0;
			size_t endIndex = list.size() - 1;

			while (startIndex <= endIndex) {
				size_t mid = startIndex + (endIndex - startIndex) / 2;
				T& element = *list[mid];  // Dereference unique_ptr
				unsigned int id = getID(element);

				if (id == searchID) {
					return list[mid].get();  // Return raw pointer (non-owning)
				}
				else if (id > searchID) {
					if (mid == 0) break;
					endIndex = mid - 1;
				}
				else {
					startIndex = mid + 1;
				}
			}

			return nullptr;
		}

		/**
		* @brief Gets an element by a specific ID from a sorted list of shared_ptr using binary search
		* @tparam T The type of the managed elements
		* @param list Reference to a sorted vector of shared_ptr<T>
		* @param getID Function that returns the ID of a given element
		* @param searchID The ID to search for
		* @return Shared pointer to the found element, or nullptr if not found
		*/
		template<typename T>
		inline std::shared_ptr<T> GetBinary(
			std::vector<std::shared_ptr<T>>& list,
			std::function<unsigned int(const T&)> getID,
			unsigned int searchID
		) {
			if (list.empty()) 
				return nullptr;

			size_t startIndex = 0;
			size_t endIndex = list.size() - 1;

			while (startIndex <= endIndex) {
				size_t mid = startIndex + (endIndex - startIndex) / 2;
				T& element = *list[mid];  // Dereference shared_ptr
				unsigned int id = getID(element);

				if (id == searchID) {
					return list[mid];
				}
				else if (id > searchID) {
					if (mid == 0) break;
					endIndex = mid - 1;
				}
				else {
					startIndex = mid + 1;
				}
			}

			return nullptr;
		}

		#pragma endregion

		#pragma region Linear

		/**
		* @brief Gets an element that satisfies a condition from a list using linear search
		* @tparam T The type of elements stored in the list
		* @param list Reference to a vector containing elements of type T
		* @param condition Function that returns true for the element to find
		* @return Pointer to the found element, or nullptr if not found
		*/
		template<typename T>
		inline T* GetLinear(std::vector<T>& list, std::function<bool(const T&)> condition) {
			for (auto& e : list)
				if (condition(e))
					return &e;

			return nullptr;
		}

		/**
		* @brief Gets an element that satisfies a condition from a list of unique_ptr using linear search
		* @tparam T The type of the managed elements
		* @param list Reference to a vector of unique_ptr<T>
		* @param condition Function that returns true for the element to find
		* @return Pointer to the found element (non-owning), or nullptr if not found
		*/
		template<typename T>
		inline T* GetLinear(std::vector<std::unique_ptr<T>>& list, std::function<bool(const T&)> condition) {
			for (auto& e : list)
				if (condition(*e))
					return e.get();

			return nullptr;
		}

		/**
		* @brief Gets an element that satisfies a condition from a list of shared_ptr using linear search
		* @tparam T The type of the managed elements
		* @param list Reference to a vector of shared_ptr<T>
		* @param condition Function that returns true for the element to find
		* @return Shared pointer to the found element, or nullptr if not found
		*/
		template<typename T>
		inline std::shared_ptr<T> GetLinear(std::vector<std::shared_ptr<T>>& list, std::function<bool(const T&)> condition) {
			for (auto& e : list)
				if (condition(*e))
					return e;

			return nullptr;
		}

		/**
		* @brief Recursively searches a vector and its children for an element matching the given condition.
		* @tparam T Type of elements in the vector
		* @param list The vector to search through
		* @param condition Function returning true if the element matches
		* @param getChildren Function returning a reference to the children vector of an element
		* @return Pointer to the found element or nullptr if not found
		*/
		template<typename T>
		inline T* GetLinearRecursive(
			std::vector<T>& list,
			std::function<bool(const T&)> condition,
			std::function<std::vector<T>& (T&)> getChildren
		) {
			for (auto& element : list) {
				if (condition(element))
					return &element;

				auto& children = getChildren(element);
				if (!children.empty()) {
					T* found = GetLinearRecursive(children, condition, getChildren);
					if (found)
						return found;
				}
			}
			return nullptr;
		}

		template<typename T>
		inline T* GetLinearRecursive(
			std::vector<std::unique_ptr<T>>& list,
			std::function<bool(const T&)> condition,
			std::function<std::vector<std::unique_ptr<T>>& (T&)> getChildren
		) {
			for (auto& element : list) {
				if (condition(*element))
					return element.get();

				auto& children = getChildren(*element);
				if (!children.empty()) {
					T* found = GetLinearRecursive(children, condition, getChildren);
					if (found)
						return found;
				}
			}
			return nullptr;
		}

		template<typename T>
		inline std::shared_ptr<T> GetLinearRecursive(
			std::vector<std::shared_ptr<T>>& list,
			std::function<bool(const T&)> condition,
			std::function<std::vector<std::shared_ptr<T>>& (T&)> getChildren
		) {
			for (auto& element : list) {
				if (condition(*element))
					return element;

				auto& children = getChildren(element);
				if (!children.empty()) {
					std::shared_ptr<T> found = GetLinearRecursive(children, condition, getChildren);
					if (found)
						return found;
				}
			}
			return nullptr;
		}

		#pragma endregion

	}

	namespace Sorting {
		
		/**
		* @brief Sorts a list in-place using a user-defined comparison function.
		* @tparam T Type of elements stored in the vector
		* @param list Reference to the vector to be sorted
		* @param condition Function returning true if the first element should come before the second
		*/
		template<typename T>
		inline void BubbleSort(std::vector<T>& list, std::function<bool(const T& a, const T& b)> condition) {
			size_t n = list.size();
			for (size_t i = 0; i < n; ++i) {
				for (size_t j = 0; j < n - 1 - i; ++j) {
					// Swap if the condition indicates list[j] should come before list[i]
					if (!condition(list[j], list[j + 1])) {
						T temp = list[j];
						list[j] = list[j + 1];
						list[j + 1] = temp;
					}
				}
			}
		}

		/**
		* @brief Performs Bubble Sort on 'list' and stores the sorted elements in 'outList'
		* @tparam T Type of elements
		* @param list The input vector to be sorted
		* @param outList Vector that will contain the sorted elements (must be resized to list.size())
		* @param condition Comparison function: returns true if first element should come before second
		*/
		template<typename T>
		inline void BubbleSort(std::vector<T>& list, std::vector<T>& outList, std::function<bool(const T& a, const T& b)> condition) {
			outList = list;

			for (size_t i = 0; i < outList.size(); ++i) {
				for (size_t j = 0; j < outList.size() - 1 - i; ++j) {
					if (!condition(outList[j], outList[j + 1])) {
						T temp = outList[j];
						outList[j] = outList[j + 1];
						outList[j + 1] = temp;
					}
				}
			}
		}

	}

}