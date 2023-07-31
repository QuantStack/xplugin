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

XPLUGIN_CREATE_XPLUGIN_FACTORY((xp::xfactory<AccumulatorMax, acc::AccumulatorBase>));
