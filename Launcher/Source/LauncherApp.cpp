#include "pch.h"
#include "ServiceLocator.h"

using Locator::ServiceLocator;

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

int main()
{
	// Service Locator initialization
	auto locator = std::make_unique<ServiceLocator>();

	//// Service Registration
	//locator->RegisterService<ILogger>(new Logger());
	//locator->RegisterService<IConfiguration>(new Configuration());
	//// Check double registration
	////locator->RegisterService<ILogger>(new Logger());

	//locator->ServicesList();
	//locator->UnregisterService<ILogger>();
	//locator->ServicesList();

	//// Request the service
	//auto logger1 = locator->Get<ILogger>();
	//// Guarantee same instance
	//auto logger2 = locator->Get<ILogger>();
	//logger1->Info("information");
	//logger2->Info("information");

	//locator->Clear();

	//// Service Factory Creation -> Option to create the raw pointer
	locator->RegisterServiceFactory<IConfiguration>([]() { return std::make_shared<Configuration>(); });
	auto config1 = locator->Get<IConfiguration>();
	auto config2 = locator->Get<IConfiguration>();
	config1->Load();
	config2->Load();

	locator->ServicesFactoryList();

	locator->UnregisterServiceFactory<IConfiguration>();

	locator->ServicesFactoryList();

	return 0;
}