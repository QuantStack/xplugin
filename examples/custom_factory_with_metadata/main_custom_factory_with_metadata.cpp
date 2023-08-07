#include <custom_factory_with_metadata_plugin_base.hpp>
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

    using factory_base_type = PluginFactoryBase;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;
    plugin_registry_type registry(plugin_directory);

    for (auto [name, factory] : registry)
    {

        std::cout << "plugin " << name << " factory metadata: " << std::endl;
        std::cout << "  name: " << factory->name() << std::endl;
        std::cout << "  description: " << factory->description() << std::endl;
        std::cout << "  version: " << factory->version() << std::endl;

        auto instance = factory->create();
        std::cout << "plugin " << name << " says: " << instance->some_function() << std::endl;
    }
}
