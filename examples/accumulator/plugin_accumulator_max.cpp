#include <accumulator_base.hpp>
#include <xplugin/xfactory.hpp>

#include <algorithm>

class AccumulatorMax : public acc::AccumulatorBase
{
  public:
    AccumulatorMax() = default;
    virtual ~AccumulatorMax() = default;

    virtual double accumulate(const std::vector<double> &data) override
    {
        return *std::max_element(data.begin(), data.end());
    }
    virtual std::string name() const override
    {
        return "max";
    }
};

using factory_type = xp::xfactory<AccumulatorMax, acc::AccumulatorBase>;
using factory_base_type = typename factory_type::factory_base_type;

// extern c function to factory st. we get a demangled name
extern "C" factory_base_type *create_plugin_factory()
{
    return new factory_type();
}
