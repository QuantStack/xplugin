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
    if(registry.size() != 3){
        std::cout<<"Test failed: registry.size() != 3"<<std::endl;
        return 1;
    }


    auto factory_01 = registry.create_factory("plugin_01");
    auto factory_02 = registry.create_factory("plugin_02");
    auto factory_03 = registry.create_factory("plugin_03");

    if(!factory_01){
        std::cout<<"Test failed: factory_01"<<std::endl;
        return 1;
    }
    if(!factory_02){
        std::cout<<"Test failed: factory_02"<<std::endl;
        return 1;
    }
    if(!factory_03){
        std::cout<<"Test failed: factory_03"<<std::endl;
        return 1;
    }

    auto plugin_01 = factory_01->create(1, "a");
    auto plugin_02 = factory_02->create(2, "b");
    auto plugin_03 = factory_03->create(3, "c");

    if(!plugin_01){
        std::cout<<"Test failed: plugin_01"<<std::endl;
        return 1;
    }
    if(!plugin_02){
        std::cout<<"Test failed: plugin_02"<<std::endl;
        return 1;
    }
    if(!plugin_03){
        std::cout<<"Test failed: plugin_03"<<std::endl;
        return 1;
    }

    auto name01 = plugin_01->name();
    auto name02 = plugin_02->name();
    auto name03 = plugin_03->name();

    if(name01 != "Plugin01"){
        std::cout<<"Test failed: name01: "<<name01<<std::endl;
        return 1;
    }
    if(name02 != "Plugin02"){
        std::cout<<"Test failed: name02: "<<name02<<std::endl;
        return 1;
    }
    if(name03 != "Plugin03"){
        std::cout<<"Test failed: name03: "<<name03<<std::endl;
        return 1;
    }


    auto greet01 = plugin_01->greet();
    auto greet02 = plugin_02->greet();
    auto greet03 = plugin_03->greet();

    if(greet01 != "hello from Plugin01"){
        std::cout<<"Test failed: greet01: "<<greet01<<std::endl;
        return 1;
    }
    if(greet02 != "hello from Plugin02"){
        std::cout<<"Test failed: greet02: "<<greet02<<std::endl;
        return 1;
    }
    if(greet03 != "hello from Plugin03"){
        std::cout<<"Test failed: greet03: "<<greet03<<std::endl;
        return 1;
    }

    return 0;
}

EMSCRIPTEN_BINDINGS(my_module) {
    emscripten::function("run_tests", &run_tests);
}
