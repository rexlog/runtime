### runtime
Dependency Injection for Modern C++

### Integrate runtime to your project
Create a cpp file with following codes:
```C++
#include <Runtime.hpp>

REXLOG_RUNTIME_IMPLEMENTION;
```

### Basic usage
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

### With custom free method
```C++
class MyClass {
protected:
int m_int;
public:
static void freeMethod(rexlog::RuntimeStorage *self){
  rexlog::RealRuntimeStorage<MyClass*> *storage=static_cast<rexlog::RealRuntimeStorage<MyClass*>>(self);
  delete storage->data;
}
public:
MyClass(){
  this->m_int=new int;
}
~MyClass(){
  delete this-> m_int;
}
}

rexlog::Runtime *runtime = rexlog::Runtime::getInstance();
runtime->set("myObj", new MyClass, MyClass::freeMethod);

```

### Use Lamba as custom free method
```C++
class MyClass {
protected:
int m_int;
public:
MyClass(){
  this->m_int=new int;
}
~MyClass(){
  delete this-> m_int;
}
}

rexlog::Runtime *runtime = rexlog::Runtime::getInstance();
runtime->set("myObj", new MyClass, [&](rexlog::RuntimeStorage *self){
  rexlog::RealRuntimeStorage<MyClass*> *storage=static_cast<rexlog::RealRuntimeStorage<MyClass*>>(self);
  delete storage->data;
});

```

