// quick_example.cpp
#include <emscripten/bind.h>

// 
#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include "testplugin_a/plugin_base.hpp"

#include <iostream>


int run_tests() {
    xp::xplugin_registry<xp::xfactory_base<plugin::PluginBase, int, std::string>> registry;
    registry.scan_directory("/plugins/");
    std::cout<<"registry size: "<<registry.size()<<std::endl;


    auto factory_01 = registry.create_factory("plugin_01");
    auto factory_02 = registry.create_factory("plugin_02");
    auto factory_03 = registry.create_factory("plugin_03");

    auto plugin_01 = factory_01->create(1, "a");
    auto plugin_02 = factory_02->create(2, "b");
    auto plugin_03 = factory_03->create(3, "c");

    std::cout<<plugin_01->name()<<std::endl;
    std::cout<<plugin_02->name()<<std::endl;
    std::cout<<plugin_03->name()<<std::endl;

    std::cout<<plugin_01->greet()<<std::endl;
    std::cout<<plugin_02->greet()<<std::endl;
    std::cout<<plugin_03->greet()<<std::endl;

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("run_tests", &run_tests);
}
