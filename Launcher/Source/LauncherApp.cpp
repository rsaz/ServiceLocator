#include "pch.h"
#include "ServiceLocator.h"

// Class Examples
using Locator::ServiceLocator;

#pragma region Classes Examples
class ILogger
{
public:
	virtual void Info(const std::string& message) = 0;
};

class Logger : public ILogger
{
public:
	// Inherited via ILogger
	virtual void Info(const std::string& message) override
	{
		std::cout << message << std::endl;
	}
};

class IConfiguration
{
public:
	virtual void Load() = 0;
};

class Configuration : public IConfiguration
{
public:
	void Load()
	{
		std::cout << "Loading configuration\n";
	}
};
#pragma endregion


auto main() -> int
{
	// Service Locator initialization
	auto locator = std::make_unique<ServiceLocator>();

	// Service Registration
	locator->RegisterService<Logger>();
	locator->RegisterService<Configuration>();
	
	// Check double registration
	locator->RegisterService<Logger>();

	// Request the service
	auto logger1 = locator->Get<Logger>();

	// Guarantee same instance
	auto logger2 = locator->Get<Logger>();
	logger1->Info("information");
	logger2->Info("information");

	// Check Unregister Singleton Service
	std::cout << locator->ServicesList() << std::endl;
	locator->UnregisterService<Logger>();
	std::cout << locator->ServicesList() << std::endl;

	// Clear all services (Singleton and Transients)
	locator->Clear();

	// Service Factory Creation
	locator->RegisterServiceFactory<Configuration>();
	
	// Get a new instance upon each request
	auto config1 = locator->Get<Configuration>();
	auto config2 = locator->Get<Configuration>();
	config1->Load();
	config2->Load();

	// Check Unregister Factory Services
	std::cout << locator->ServicesFactoryList() << std::endl;
	locator->UnregisterServiceFactory<Configuration>();
	std::cout << locator->ServicesFactoryList() << std::endl;

	return 0;
}