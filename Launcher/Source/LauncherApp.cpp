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

	// Service Registration
	locator.get()->RegisterService<ILogger>(new Logger());
	// Check double registration
	locator.get()->RegisterService<ILogger>(new Logger());

	// Request the service
	auto logger1 = locator.get()->Get<ILogger>();
	// Guarantee same instance
	auto logger2 = locator.get()->Get<ILogger>();
	logger1.get()->Info("information");
	logger2.get()->Info("information");

	locator.get()->Clear();

	// Service Factory Creation
	locator.get()->RegisterServiceFactory<IConfiguration>([]() { return std::make_shared<Configuration>(); });
	auto config1 = locator.get()->Get<IConfiguration>();
	auto config2 = locator.get()->Get<IConfiguration>();
	config1.get()->Load();
	config2.get()->Load();

	return 0;
}