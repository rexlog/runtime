### runtime
Dependency Injection for Modern C++

### usage
```C++
#include <runtime.hpp>

rexlog::Runtime *runtime = rexlog::Runtime::getInstance();

//Setter: can store any type of data;
std::string serverName="My HTTP Server";
runtime->set("serverName", serverName);
runtime->set("serverPort", static_cast<int>(8080));

//Getter: template method
std::cout <<"The server name is"<<runtime->get<std::string>("serverName")<<std::endl;
std::cout <<"The server port is"<<runtime->get<int>("serverPort")<<std::endl;
```
