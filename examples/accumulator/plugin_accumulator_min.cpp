#include <accumulator_base.hpp>
#include <xplugin/xfactory.hpp>

#include <algorithm>

class XPLUGIN_API AccumulatorMin : public acc::AccumulatorBase
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
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
