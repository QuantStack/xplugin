#include <xplugin/xplugin_registry.hpp>
#include <accumulator_base.hpp>

int main(int argc, char** argv)
{
    using instance_type = acc::AccumulatorBase;
    using factory_type = xp::xfactory_base<instance_type>;

    using plugin_registry_type = xp::xplugin_registry<factory_type>;

    plugin_registry_type registry;

    registry.scan_directory(".");
    auto factory = registry.create_factory("plugin_accumulator_max");

    auto acc = factory->create();

    std::vector<double> data = {1,2,3,4,5,6,7,8,9,10};

    auto result = acc->accumulate(data);

    // print complete result vector
    std::cout <<"accumulator "<< acc->name() << " result: ";
    for (auto& r : result){
        std::cout << r << " ";
    }
    std::cout << std::endl;

}