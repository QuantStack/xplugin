#include <custom_factory_with_metadata_plugin_base.hpp>

// for XPLUGIN_CREATE_XPLUGIN_FACTORY macro
#include <xplugin/xfactory.hpp>

class XPLUGIN_API PluginA : public PluginBase
{
  public:
    PluginA() = default;
    virtual ~PluginA() = default;

    std::string some_function() const override
    {
        return "PluginA";
    }

  private:
    int m_a;
    std::string m_b;
};

class XPLUGIN_API PluginAFactory : public PluginFactoryBase
{
  public:
    using factory_base_type = PluginFactoryBase;

    std::unique_ptr<PluginBase> create() override
    {
        return std::make_unique<PluginA>();
    }

    std::string name() const override
    {
        return "PluginA";
    }
    std::string description() const override
    {
        return "This is a plugin to showcase the plugin system."
               "In particular this example shows the usage of a plugin "
               "with a custom factory with additional metadata.";
    }
    std::string version() const override
    {
        return "0.0.1";
    }
};

// register the plugin
XPLUGIN_CREATE_XPLUGIN_FACTORY(PluginAFactory);
