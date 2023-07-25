#include <xplugin/xplugin_registry.hpp>
#include <xplugin/xfactory.hpp>
#include <accumulator_base.hpp>

int main(int argc, char** argv)
{

    if(argc != 2){
        std::cout << "usage: " << argv[0] << " <plugin_directory>" << std::endl;
        return 1;
    }
    std::string plugin_directory = argv[1];



    using instance_type = acc::AccumulatorBase;
    using factory_type = xp::xfactory_base<instance_type>;
    using plugin_registry_type = xp::xplugin_registry<factory_type>;

    plugin_registry_type registry;
    registry.scan_directory(plugin_directory);

    std::cout << "available plugins:" << std::endl;
    for (auto& p : registry.plugin_names()){
        std::cout << p << std::endl;
    }

    // create data to accumulate
    std::vector<double> data = {1,2,3,4,5,6,7,8,9,10};

    // create and use plugin_accumulator_max 
    auto factory_max = registry.create_factory("plugin_accumulator_max");
    auto acc_max = factory_max->create();
    auto result_max = acc_max->accumulate(data);
    std::cout <<"accumulator "<< acc_max->name() << " result: "<< result_max << std::endl;

    // create and use plugin_accumulator_min
    auto factory_min = registry.create_factory("plugin_accumulator_min");
    auto acc_min = factory_min->create();
    auto result_min = acc_min->accumulate(data);
    std::cout <<"accumulator "<< acc_min->name() << " result: "<< result_min << std::endl;


}