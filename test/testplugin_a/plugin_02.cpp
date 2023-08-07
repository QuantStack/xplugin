// the base-class with the plugin-interface
#include <plugin_base.hpp>

// the base factory for the plugin
#include <xplugin/xfactory.hpp>

class Plugin02 : public plugin::PluginBase
{
  public:
    Plugin02(int a, std::string b)
        : m_a(a),
          m_b(b)
    {
    }
    std::string name() override
    {
        return "Plugin02";
    }

  private:
    int m_a;
    std::string m_b;
};

using factory_type = xp::xfactory<Plugin02, plugin::PluginBase, int, std::string>;
XPLUGIN_CREATE_XPLUGIN_FACTORY(factory_type);
