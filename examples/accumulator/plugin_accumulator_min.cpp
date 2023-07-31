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

XPLUGIN_CREATE_XPLUGIN_FACTORY((xp::xfactory<AccumulatorMin, acc::AccumulatorBase>));
