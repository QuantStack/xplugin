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

    plugin_registry_type registry;
    registry.add_from_directory(plugin_directory);

    std::cout << "available plugins:" << std::endl;
    for (auto &p : registry.plugin_names())
    {
        std::cout << p << std::endl;
    }

    for (auto &p : registry.plugin_names())
    {
        auto factory = registry.create_factory(p);
        auto instance = factory->create("main_fubar");
        std::cout << "plugin " << p << " says: " << instance->fubar() << std::endl;
    }
}
