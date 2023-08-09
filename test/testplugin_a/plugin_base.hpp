#ifndef PLUGIN_BASE_HPP
#define PLUGIN_BASE_HPP

#include <xplugin/xfactory.hpp>

#include <string>
namespace plugin
{
class XPLUGIN_API PluginBase
{
  public:
    virtual ~PluginBase() = default;
    virtual std::string name() = 0;
    virtual std::string greet()
    {
        return "hello from " + name();
    };
};

}; // namespace plugin

#endif // PLUGIN_BASE_HPP
