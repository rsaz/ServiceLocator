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
	locator->RegisterService<ILogger>(new Logger());
	locator->RegisterService<IConfiguration>(new Configuration());
	
	// Check double registration
	locator->RegisterService<ILogger>(new Logger());

	// Request the service
	auto logger1 = locator->Get<ILogger>();

	// Guarantee same instance
	auto logger2 = locator->Get<ILogger>();
	logger1->Info("information");
	logger2->Info("information");

	// Check Unregister Singleton Service
	locator->ServicesList();
	locator->UnregisterService<ILogger>();
	locator->ServicesList();

	// Clear all services (Singleton and Transients)
	locator->Clear();

	// Service Factory Creation
	locator->RegisterServiceFactory<IConfiguration>([]() { return std::make_shared<Configuration>(); });
	
	// Get a new instance upon each request
	auto config1 = locator->Get<IConfiguration>();
	auto config2 = locator->Get<IConfiguration>();
	config1->Load();
	config2->Load();

	// Check Unregister Factory Services
	locator->ServicesFactoryList();
	locator->UnregisterServiceFactory<IConfiguration>();
	locator->ServicesFactoryList();

	return 0;
}