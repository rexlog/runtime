### runtime
Dependency Injection for Modern C++

### basic usage
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

### with custom free method
```C++
class MyClass {
protected:
int m_int;
public:
static void freeMethod(rexlog::RuntimeStorage *self){
  rexlosg::RealRuntimeStorage<MyClass*> *storage=static_cast<rexlosg::RealRuntimeStorage<MyClass*>>(self);
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
