#pragma once

namespace lh {
namespace util {
	
	template <typename T>
	class Singleton {
	public:
		static T* instance() {
			if (__instance == nullptr)
				__instance = new T();

			return __instance;
		}

	private:
		Singleton() {}
		Singleton(const Singleton<T>&);
		Singleton<T>& operator=(const Singleton<T>&);
		~Singleton() {}

	private:
		static T* __instance;
	};

	template <typename T>
	T* Singleton<T>::__instance = nullptr;
}
}