#include <vector>
#include <string>

#include <xplugin/xplugin_registry.hpp>
#include <xplugin/xfactory.hpp>

namespace acc
{
    class AccumulatorBase
    {
        public:

            virtual ~AccumulatorBase() {}

            virtual std::vector<double> accumulate(const std::vector<double>& data) = 0;
            virtual std::string name() const = 0;
    };

    template<class CONCRETE>
    class AccumulatorFactory : public xp::xfactory_base<AccumulatorBase>
    {
        public:
            AccumulatorFactory() = default;
            std::unique_ptr<instance_type>  create() override {
                return std::unique_ptr<instance_type>(new CONCRETE());
            }
    };
}