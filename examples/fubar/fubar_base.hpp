#ifndef FUBAR_BASE_HPP
#define FUBAR_BASE_HPP

#include <string>
#include <vector>

#include <xplugin/xplugin_config.hpp>

namespace fubar
{
class XPLUGIN_API FubarBase
{
  public:
    virtual ~FubarBase()
    {
    }
    virtual std::string fubar() const = 0;
};
} // namespace fubar

#endif // FUBAR_BASE_HPP
