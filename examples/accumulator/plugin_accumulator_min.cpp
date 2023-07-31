#include <accumulator_base.hpp>
#include <xplugin/xfactory.hpp>

#include <algorithm>

class AccumulatorMin : public acc::AccumulatorBase
{
  public:
    AccumulatorMin() = default;
    virtual ~AccumulatorMin() = default;

    virtual double accumulate(const std::vector<double> &data) override
    {
        return *std::min_element(data.begin(), data.end());
    }
    virtual std::string name() const override
    {
        return "min";
    }
};

using factory_type = xp::xfactory<AccumulatorMin, acc::AccumulatorBase>;
using factory_base_type = typename factory_type::factory_base_type;

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type *create_plugin_factory()
{
    return new factory_type();
}
