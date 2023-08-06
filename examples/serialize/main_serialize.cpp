#include <serialize_base.hpp>
#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include <iostream>

// just to check that the plugins did their job
void ensure_same(std::unordered_map<std::string, double> &a, std::unordered_map<std::string, double> &b)
{
    if (a != b)
    {
        std::cout << "a != b" << std::endl;
        std::cout << "a: " << std::endl;
        for (auto &p : a)
        {
            std::cout << p.first << ": " << p.second << std::endl;
        }
        std::cout << "b: " << std::endl;
        for (auto &p : b)
        {
            std::cout << p.first << ": " << p.second << std::endl;
        }
        throw std::runtime_error("a != b");
    }
}

int main(int argc, char **argv)
{
    // get the plugin directory from the command line
    if (argc != 2)
    {
        std::cout << "usage: " << argv[0] << " <plugin_directory>" << std::endl;
        return 1;
    }
    std::string plugin_directory = argv[1];

    // the type base type of the factory
    using factory_base_type = xp::xfactory_base<SerializeBase>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    // create the registry
    plugin_registry_type registry(plugin_directory);

    // some data to serialize / deserialize
    std::unordered_map<std::string, double> data = {{"a", 1}, {"b", 2}, {"c", 3}};

    // use all plugins
    ///////////////////////////////////////////
    for (auto [plugin_name, factory] : registry)
    {
        // create and use plugin_serialize_json
        auto plugin = factory->create();
        auto serialized = plugin->serialize(data);
        std::cout << plugin_name << " serialized with" << plugin->name() << ":" << std::endl << serialized << std::endl;

        // deserialize with plugin_serialize_json
        auto deserialized = plugin->deserialize(serialized);

        // ensure deserialized data is the same as original data
        ensure_same(data, deserialized);
    }
}
