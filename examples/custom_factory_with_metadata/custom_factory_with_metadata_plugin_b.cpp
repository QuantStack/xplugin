#include <custom_factory_with_metadata_plugin_base.hpp>

// for XPLUGIN_CREATE_XPLUGIN_FACTORY macro
#include <xplugin/xfactory.hpp>

class XPLUGIN_API PluginB : public PluginBase
{
  public:
    PluginB() = default;
    virtual ~PluginB() = default;

    std::string some_function() const override
    {
        return "PluginB";
    }

  private:
    int m_a;
    std::string m_b;
};

class XPLUGIN_API PluginBFactory : public PluginFactoryBase
{
  public:
    using factory_base_type = PluginFactoryBase;

    std::unique_ptr<PluginBase> create() override
    {
        return std::make_unique<PluginB>();
    }

    std::string name() const override
    {
        return "PluginB";
    }
    std::string description() const override
    {
        return "This is also a plugin to showcase the plugin system."
               "In particular this example shows the usage of a plugin "
               "with a custom factory with additional metadata.";
    }
    std::string version() const override
    {
        return "1.0.1rc";
    }
};

// register the plugin
XPLUGIN_CREATE_XPLUGIN_FACTORY(PluginBFactory);
