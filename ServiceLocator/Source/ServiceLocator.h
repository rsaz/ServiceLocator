#pragma once
#include "pch.h"

namespace Locator
{
	class ServiceLocator
	{
	public:
		inline ServiceLocator() : instances(), servicesFactory() {}
		inline ~ServiceLocator() { Clear(); }

		inline void Clear()
		{
			instances.clear();
			servicesFactory.clear();
		};

		template<typename T>
		inline void RegisterService(T* instance = new T())
		{
			int hash = typeid(T).hash_code();
			if (instances.find(hash) == instances.end())
				instances.emplace(hash, instance);
			else
				std::cout << typeid(T).name() << " already registered!" << std::endl;
		};

		template<typename T>
		inline void RegisterServiceFactory(std::function<std::shared_ptr<T>()> factory = []() { return std::make_shared<T>(); })
		{
			int hash = typeid(T).hash_code();
			if (servicesFactory.find(hash) == servicesFactory.end())
				servicesFactory.emplace(hash, factory);
		}

		template<typename T>
		inline std::shared_ptr<T> Get() const
		{
			int hash = typeid(T).hash_code();
			auto itr1 = instances.find(hash);
			if (itr1 != instances.end())
				return std::static_pointer_cast<T>(itr1->second);

			auto itr2 = servicesFactory.find(hash);
			if (itr2 != servicesFactory.end())
				return std::static_pointer_cast<T>(itr2->second());

			return nullptr;
		}

	private:
		std::unordered_map<int, std::shared_ptr<void>> instances;
		std::unordered_map<int, std::function<std::shared_ptr<void>()>> servicesFactory;
	};
}