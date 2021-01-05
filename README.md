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

### For MSVC users
When you encount Link error of 1169, according to [MSDN](https://docs.microsoft.com/en-us/cpp/error-messages/tool-errors/linker-tools-error-lnk1169?view=msvc-160)

LNK 1169, one or more multiply defined symbols found

The build failed due to multiple definitions of one or more symbols. This error is preceded by error LNK2005.

The /FORCE or /FORCE:MULTIPLE option overrides this error.

for instance, if you're working on CMakeLists.txt. Then add following line:
```cmake
SET(CMAKE_EXE_LINKER_FLAGS  "/FORCE")
```
