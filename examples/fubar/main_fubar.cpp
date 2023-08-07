#include <fubar_base.hpp>
#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <plugin_directory>" << std::endl;
        return 1;
    }
    std::string plugin_directory = argv[1];

    using factory_type = xp::xfactory_base<fubar::FubarBase, const std::string &>;
    using plugin_registry_type = xp::xplugin_registry<factory_type>;

    plugin_registry_type registry(plugin_directory);

    for (auto [name, factory] : registry)
    {
        auto instance = factory->create("main_fubar");
        std::cout << "plugin " << name << " says: " << instance->fubar() << std::endl;
    }
}
