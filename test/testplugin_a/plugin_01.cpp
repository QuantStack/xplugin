// the base-class with the plugin-interface
#include <plugin_base.hpp>

// the base factory for the plugin
#include <xplugin/xfactory.hpp>

class Plugin01 : public plugin::PluginBase
{
  public:
    Plugin01(int a, std::string b) : m_a(a), m_b(b)
    {
    }
    std::string name() override
    {
        return "Plugin01";
    }

  private:
    int m_a;
    std::string m_b;
};

// extern c function to factory st. we get a demangled name
extern "C" xp::xfactory_base<plugin::PluginBase, int, std::string> *create_plugin_factory()
{
    return new xp::xfactory<Plugin01, plugin::PluginBase, int, std::string>();
}
