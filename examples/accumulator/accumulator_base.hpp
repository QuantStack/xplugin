#ifndef ACCUMULATOR_BASE_HPP
#define ACCUMULATOR_BASE_HPP

#include <string>
#include <vector>

#include <xplugin/xplugin_config.hpp> // XPLUGIN_API

namespace acc
{
class XPLUGIN_API AccumulatorBase
{
  public:
    virtual ~AccumulatorBase()
    {
    }

    virtual double accumulate(const std::vector<double> &data) = 0;
    virtual std::string name() const = 0;
};
} // namespace acc

#endif // ACCUMULATOR_BASE_HPP
