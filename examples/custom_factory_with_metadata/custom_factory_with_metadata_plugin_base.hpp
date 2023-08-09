#ifndef CUSTOM_FACTORY_WITH_METADATA_PLUGIN_BASE_HPP
#define CUSTOM_FACTORY_WITH_METADATA_PLUGIN_BASE_HPP

#include <memory>
#include <string>

#include <xplugin/xplugin_config.hpp>

class XPLUGIN_API PluginBase
{
  public:
    virtual ~PluginBase()
    {
    }

    virtual std::string some_function() const = 0;
};

class XPLUGIN_API PluginFactoryBase
{
  public:
    virtual ~PluginFactoryBase() = default;

    // the factory method
    virtual std::unique_ptr<PluginBase> create() = 0;

    // some additional metadata
    virtual std::string name() const = 0;
    virtual std::string description() const = 0;
    virtual std::string version() const = 0;
};

#endif // CUSTOM_FACTORY_WITH_METADATA_PLUGIN_BASE_HPP
