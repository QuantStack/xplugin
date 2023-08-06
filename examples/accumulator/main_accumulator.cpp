#include <accumulator_base.hpp>
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

    using base_type = acc::AccumulatorBase;
    using factory_base_type = xp::xfactory_base<base_type>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    plugin_registry_type registry(plugin_directory);

    std::cout << "available plugins:" << std::endl;
    for (auto p : registry)
    {
        std::cout << p.first << std::endl;
    }

    // create data to accumulate
    std::vector<double> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // create and use plugin_accumulator_max
    auto acc_max = registry["plugin_accumulator_max"]->create();
    auto result_max = acc_max->accumulate(data);
    std::cout << "accumulator " << acc_max->name() << " result: " << result_max << std::endl;

    // create and use plugin_accumulator_min
    auto acc_min = registry["plugin_accumulator_min"]->create();
    auto result_min = acc_min->accumulate(data);
    std::cout << "accumulator " << acc_min->name() << " result: " << result_min << std::endl;
}
