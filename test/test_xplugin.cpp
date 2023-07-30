#include <test.hpp>
#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include <testplugin_a/plugin_base.hpp>

TEST_CASE("test_xplugin")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    plugin_registry_type registry;
    registry.scan_directory("testplugin_a");
    {
        CHECK_EQ(registry.size(), 3);

        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 3);
        CHECK(names.count("plugin_01"));
        CHECK(names.count("plugin_02"));
        CHECK(names.count("plugin_03"));

        auto factory_01 = registry.create_factory("plugin_01");
        auto factory_02 = registry.create_factory("plugin_02");
        auto factory_03 = registry.create_factory("plugin_03");


        CHECK(factory_01);
        CHECK(factory_02);
        CHECK(factory_03);

        auto plugin_01 = factory_01->create(1, "a");
        auto plugin_02 = factory_02->create(2, "b");
        auto plugin_03 = factory_03->create(3, "c");

        CHECK(plugin_01);
        CHECK(plugin_02);
        CHECK(plugin_03);

        CHECK_EQ(plugin_01->name(), "Plugin01");
        CHECK_EQ(plugin_02->name(), "Plugin02");
        CHECK_EQ(plugin_03->name(), "Plugin03");

        CHECK_EQ(plugin_01->greet(), "hello from Plugin01");
        CHECK_EQ(plugin_02->greet(), "hello from Plugin02");
        CHECK_EQ(plugin_03->greet(), "hello from Plugin03");
    }

    // close the open libraries
    registry.close_all();

} 