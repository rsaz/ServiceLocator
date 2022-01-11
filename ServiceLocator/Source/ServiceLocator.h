#pragma once
#include "pch.h"

namespace Locator
{
	class ServiceLocator
	{
	public:
		inline ServiceLocator() : instances(), servicesFactory() {}
		inline ~ServiceLocator() { Clear(); }

		/// <summary> Method to clear all instances and type containers
		/// <para> This method will unregister all services from singleton services and services factory
		/// </summary>
		inline void Clear()
		{
			instances.clear();
			instancesTypes.clear();

			servicesFactory.clear();
			servicesFactoryTypes.clear();
		};

		/// <summary> Method to Register a Singleton Service
		/// <para>This method creates a hash to be associated with the the type registed in the unordered map.</para> 
		/// <seealso cref="std::unordered_map"/>
		/// <para>A Singleton object is provided every request, the same instance.</para> 
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="instance"></param>
		template<typename T>
		inline bool RegisterService()
		{
			int hash = static_cast<int>(typeid(T).hash_code());
			if (instances.find(hash) == instances.end())
			{
				instances.emplace(hash, new T);
				instancesTypes.push_back(typeid(T).name());
				
				return true;
			}
			
			return false;
		};

		/// <summary> Method to Unregister a Singleton Service
		/// <para>This method identifies if the type hash generated exist in the instances container.</para>
		/// <para>The <paramref name="T"/> is the class service that will be unregistered</para>
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		inline bool UnregisterService()
		{
			auto hash = static_cast<int>(typeid(T).hash_code());
			if (instances.find(hash) != instances.end())
			{
				instances.erase(hash);

				auto it = std::find_if(instancesTypes.begin(), 
					instancesTypes.end(), [&](const std::string& t) {return (t == typeid(T).name()); });
				if (&it != nullptr) instancesTypes.erase(it);

				return true;
			}

			return false;
		}

		/// <summary> Method to Register Service Factory
		/// <para>This method creates a hash to be associated with the the type registed in the unordered map.</para>
		/// <para>A Transient object is provided every request, a new instance.</para> 
		/// <seealso cref="std::unordered_map"/>
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <param name="instance"></param>
		template<typename T>
		inline bool RegisterServiceFactory()
		{
			std::function<std::shared_ptr<T>()> factory = []() { return std::make_shared<T>(); };
			
			int hash = static_cast<int>(typeid(T).hash_code());
			if (servicesFactory.find(hash) == servicesFactory.end())
			{
				servicesFactory.emplace(hash, factory);
				servicesFactoryTypes.push_back(typeid(T).name());

				return true;
			}

			return false;
		}

		/// <summary> Method to Unregister a Factory Service
		/// <para>This method identifies if the type hash generated exist in the servicesFactory container.</para>
		/// <para>The <paramref name="T"/> is the class service that will be unregistered</para>
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<typename T>
		inline bool UnregisterServiceFactory()
		{
			auto hash = static_cast<int>(typeid(T).hash_code());
			if (servicesFactory.find(hash) != servicesFactory.end())
			{
				servicesFactory.erase(hash);

				auto it = std::find_if(servicesFactoryTypes.begin(),
					servicesFactoryTypes.end(), [&](const std::string& t) {return (t == typeid(T).name()); });
				if (&it != nullptr) servicesFactoryTypes.erase(it);

				return true;
			}

			return false;
		}

		/// <summary> Method to Request a Service
		/// <para>This method provides the instance requested based on the type.</para>
		/// <para>Types, Singletons or Transients are stored in different containers.</para>
		/// </summary>
		/// <typeparam name="T"></typeparam>
		/// <returns>The instance requested</returns>
		template<typename T>
		inline std::shared_ptr<T> Get() const
		{
			try
			{
				int hash = static_cast<int>(typeid(T).hash_code());
				auto itr1 = instances.find(hash);
				if (itr1 != instances.end())
					return std::static_pointer_cast<T>(itr1->second);

				auto itr2 = servicesFactory.find(hash);
				if (itr2 != servicesFactory.end())
					return std::static_pointer_cast<T>(itr2->second());
			}
			catch (const std::exception& ex)
			{
				std::cout << ex.what() << std::endl;
			}
			return nullptr;
		}

		/// <summary>Method returns a list of Singleton Services Registered</summary>
		inline std::string ServicesList()
		{
			if (instances.empty())
			{
				instancesTypes.clear();
				return {};
			}
			
			std::stringstream ss;
			ss << "  " << " | " << "Registered Singletion Services" << " |" << "\n";
			for (auto& service : instancesTypes) ss << "->" << " [ " << service << " ]" << "\n";

			return ss.str();
		}

		/// <summary>Method returns a list of Factory Services Registered</summary>
		inline std::string ServicesFactoryList()
		{
			if (servicesFactory.empty())
			{
				servicesFactoryTypes.clear();
				return {};
			}

			std::stringstream ss;
			ss << "  " << " | " << "Registered Factory Services" << " |" << "\n";
			for (auto& service : servicesFactoryTypes) ss << "->" << " [ " << service << " ]" << "\n";

			return ss.str();
		}

	private:
		std::unordered_map<int, std::shared_ptr<void>> instances;
		std::vector<std::string> instancesTypes;

		std::unordered_map<int, std::function<std::shared_ptr<void>()>> servicesFactory;
		std::vector<std::string> servicesFactoryTypes;
	};
}