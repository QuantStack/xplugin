# xplugin
generic plugin framework in C++




## Usage

## Example 1

#### 1. Define a plugin interface

`my_plugin_base.hpp`:
```cpp
class MyPluginBase
{
public:
    virtual ~MyPluginBase() = default;
    virtual void do_something() = 0;
};
```

#### 2. Define a plugin implementation / define multiple plugin implementations

`my_plugin_a.cpp`:
```cpp  
#include <my_plugin_base.hpp>
#include <xplugin/xfactory.hpp>

class MyPluginA : public MyPluginBase
{
    public:
    virtual ~MyPluginA() = default;
    override void do_something() override
    {
        std::cout << "MyPluginA::do_something()" << std::endl;
    }
}

using factory_type = xp::xfactory<MyPluginA, MyPluginBase>;
using factory_base_type = typename factory_type::factory_base_type;

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type * create_plugin_factory(){
    return new factory_type();
}
```


`my_plugin_b.cpp`:
```cpp  
#include <my_plugin_base.hpp>
#include <xplugin/xfactory.hpp>

class MyPluginB : public MyPluginBase
{
    public:
    virtual ~MyPluginB() = default;
    override void do_something() override
    {
        std::cout << "MyPluginB::do_something()" << std::endl;
    }
}

using factory_type = xp::xfactory<MyPluginB, MyPluginBase>;
using factory_base_type = typename factory_type::factory_base_type;

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type * create_plugin_factory(){
    return new factory_type();
}
```

#### 3. Use the plugin

`main.cpp`:
```cpp

#include <my_plugin_base.hpp>
#include <xplugin/xplugin_registry.hpp>
#include <xplugin/xfactory.hpp>


#include <iostream> // just for the example

using factory_base_type = xp::xfactory_base<MyPluginBase>;
using plugin_registry_type = xp::xplugin_registry<factory_base_type>;


int main(int argc, char** argv)
{

    if(argc != 2){
        std::cout << "usage: " << argv[0] << " <plugin_directory>" << std::endl;
        return 1;
    }
    std::string plugin_directory = argv[1];


    plugin_registry_type registry;
    registry.scan_directory(plugin_directory);

    std::cout << "available plugins:" << std::endl;
    for (auto& name : registry.plugin_names()){
        std::cout << name << std::endl;
        auto factory  registry.create_factory(name);
        auto plugin = factory_min->do_something();
    }
}
```