# xplugin

[![ci](https://github.com/QuantStack/xplugin/actions/workflows/ci.yaml/badge.svg)](https://github.com/QuantStack/xplugin/actions/workflows/ci.yaml)

Generic plugin framework in C++.

## Features

- [x] Generic plugin framework
- [x] Arbitrary abstract base classes can be used as plugin interface
- [x] Customizable with custom plugin factories (see examples)
- [x] Works on linux, mac, windows and emscripten

## Usage

## Example 1

All plugins are derived from a base class, in the first example `MyPluginBase`.
All concrete plugins are derived from `MyPluginBase` and implement the pure virtual function (in this example it is only the `do_something()` function).
Furthermore, for this example, we assume that all concrete plugins have an empty constructor.
(see example 2 for a more complex example)

#### Define a plugin interface

`my_plugin_base.hpp`:
```cpp
class MyPluginBase
{
public:
    virtual ~MyPluginBase() = default;
    virtual void do_something() = 0;
};
```

#### Define a plugin implementation / define multiple plugin implementations

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
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
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
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
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


    plugin_registry_type registry(plugin_directory);

    for (auto [name, factory] : registry){
        std::cout << name << std::endl;
        auto plugin = factory->create();
        plugin->do_something();
    }
}
```


## Example 2


We again define a plugin interface `MyOtherPluginBase` and a concrete plugin  implementations `MyOtherPluginA` and `MyOtherPluginB`.
The difference to the first example is that the concrete plugins have a constructor with arguments.
But both plugins have the same constructor signature.

`my_other_plugin_base.hpp`:
```cpp
class MyOtherPluginBase
{
public:
    virtual ~MyOtherPluginBase() = default;
    virtual void do_something() = 0;
};
```


`my_other_plugin_a.cpp`:
```cpp
#include <my_plugin_base.hpp>
#include <xplugin/xfactory.hpp>

class MyOtherPluginA : public MyPluginBase
{
    public:
    MyOtherPluginA(int some_data, const std::string  & some_other_data)
    : m_some_data(some_data)
    , m_some_other_data(some_other_data)
    {}

    virtual ~MyOtherPluginA() = default;
    override void do_something() override
    {
        std::cout << "MyOtherPluginA::do_something()" << std::endl;
        std::cout << "some_data: " << m_some_data << std::endl;
        std::cout << "some_other_data: " << m_some_other_data << std::endl;

    }
    private:
    int m_some_data;
    std::string m_some_other_data;
}

using factory_type = xp::xfactory<MyOtherPluginA, MyPluginBase, int, const std::string &>;
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
```


`my_other_plugin_b.cpp`:
```cpp
#include <my_plugin_base.hpp>
#include <xplugin/xfactory.hpp>

class MyOtherPluginB : public MyPluginBase
{
    public:
    MyOtherPluginB(int some_data, const std::string  & some_other_data)
    : m_some_data(some_data)
    , m_some_other_data(some_other_data)
    {}

    virtual ~MyOtherPluginB() = default;
    override void do_something() override
    {
        std::cout << "MyOtherPluginB::do_something()" << std::endl;
        std::cout << "some_data: " << m_some_data << std::endl;
        std::cout << "some_other_data: " << m_some_other_data << std::endl;

    }
    private:
    int m_some_data;
    std::string m_some_other_data;
}

using factory_type = xp::xfactory<MyOtherPluginB, MyPluginBase, int, const std::string &>;
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
```


`main.cpp`:
```cpp

#include <my_plugin_base.hpp>
#include <xplugin/xplugin_registry.hpp>
#include <xplugin/xfactory.hpp>


#include <iostream> // just for the example

using factory_base_type = xp::xfactory_base<MyOtherPluginBase, int, const std::string &>;
using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

int main(int argc, char** argv)
{

    if(argc != 2){
        std::cout << "usage: " << argv[0] << " <plugin_directory>" << std::endl;
        return 1;
    }
    std::string plugin_directory = argv[1];


    plugin_registry_type registry(plugin_directory);

    int some_data = 42;
    std::string some_other_data = "Hello World";

    for (auto [name, factory] : registry){
        std::cout << name << std::endl;
        auto plugin = factory->create(some_data, some_other_data);
        plugin->do_something();
    }
}
```
