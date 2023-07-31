// the base-class with the plugin-interface
#include <plugin_base.hpp>

// the base factory for the plugin
#include <xplugin/xfactory.hpp>

class Plugin03 : public plugin::PluginBase
{
  public:
    Plugin03(int a, std::string b) : m_a(a), m_b(b)
    {
    }
    std::string name() override
    {
        return "Plugin03";
    }

  private:
    int m_a;
    std::string m_b;
};

XPLUGIN_CREATE_XPLUGIN_FACTORY((xp::xfactory<Plugin03, plugin::PluginBase, int, std::string>));
