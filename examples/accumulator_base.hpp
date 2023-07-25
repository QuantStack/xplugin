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
}