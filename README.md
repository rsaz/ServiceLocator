## Service Locator
Very fast, header-only C++ Service Locator Pattern library

### What is the Service Locator Pattern
The Service Locator Pattern is a design pattern that encapsulates the processes involved in obtaining a service with a strong abstraction layer. This pattern uses a central registry know as the "Service Locator" to request either Singleton instances or make use of a factory to generate instances of a particular type. This pattern simplifies component-based application where all applications dependencies are cleanly listed at the beggining of the application boostrap.

### Motivation
To simplify the classes relationships and dependencies, defining a single point of class registration and request. To reduce software development complexity

### Advantages
- Simplifies the use of singleton and static objects
- Centralize the use of objects in one class called locator
- The code is easy to test

### Install
##### Copy the includes [folder](https://github.com/rsaz/ServiceLocator/tree/master/includes) to your build tree and use a C++ compilter >= 11
```

$ git clone https://github.com/rsaz/ServiceLocator.git

```
### Platforms
- Windows (msvc 2013+, cygwin)

### Package Manager

### Features
- Register service ```RegisterService<type>()```
- Unregister service ```UnregisterService<type>()```
- Request service ```Get<type>()```
- Request service list ```ServicesList()```
- Register service factory ```RegisterServiceFactory<type>()```
- Unregister service factory ```UnregisterServiceFactory<type>()```
- Request service factory ```Get<type>()```
- Request service factory list ```ServicesFactoryList()```

### Usage examples
```c++
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
	locator->ServicesList();
	locator->UnregisterService<Logger>();
	locator->ServicesList();

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
	locator->ServicesFactoryList();
	locator->UnregisterServiceFactory<Configuration>();
	locator->ServicesFactoryList();

	return 0;
}
```
