#include <test.hpp>

#include <vector>

#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include <testplugin_a/plugin_base.hpp>

#ifndef XPLUGIN_NO_THREADS
#include <thread>
#endif

#include <algorithm>

// to get a unique type for each test
// since xp::get_registry<tfactory_type>()
// returns a reference to a static variable
template <class FACTORY, int TAG>
class tagged_factory : public FACTORY
{
    using FACTORY::FACTORY;
};

TEST_CASE("test_xplugin")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    plugin_registry_type registry;
    registry.add_from_directory("testplugin_a");
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
}

TEST_CASE("test_xregistry_getter")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    // to get a different singleton for each test
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;

    auto &registry = xp::get_registry<test_factory_type>();
    registry.add_from_directory("testplugin_a");

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

TEST_CASE("test_xregistry_open_same_lib_multiple_times")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    std::vector<plugin_registry_type *> registries(1000);
    for (auto &registry : registries)
    {
        registry = new plugin_registry_type();
        registry->add_from_directory("testplugin_a");
    }

    for (auto &registry : registries)
    {
        CHECK_EQ(registry->size(), 3);

        auto names = registry->plugin_names();
        CHECK_EQ(names.size(), 3);
        CHECK(names.count("plugin_01"));
        CHECK(names.count("plugin_02"));
        CHECK(names.count("plugin_03"));

        auto factory_01 = registry->create_factory("plugin_01");
        auto factory_02 = registry->create_factory("plugin_02");
        auto factory_03 = registry->create_factory("plugin_03");

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
    }

    // delete all registries
    for (auto &registry : registries)
    {
        delete registry;
    }
}

TEST_CASE("test_xregistry_single_plugin")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;

    // to get a different singleton for each test
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;

    auto &registry = xp::get_registry<test_factory_type>();
    using registry_type = std::decay_t<decltype(registry)>;
    {
        auto ret = registry.add_plugin("testplugin_a", "plugin_01");
        CHECK(ret == registry_type::add_plugin_result::added);
    }

    CHECK_EQ(registry.size(), 1);

    {
        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 1);
        CHECK(names.count("plugin_01"));
    }

    // check idempotency
    {
        auto ret = registry.add_plugin("testplugin_a", "plugin_01");
        CHECK(ret == registry_type::add_plugin_result::already_exists);
    }

    CHECK_EQ(registry.size(), 1);

    {
        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 1);
        CHECK(names.count("plugin_01"));
    }
}

TEST_CASE("test_xregistry_single_plugin_with_multiple_search_dirs")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;

    // to get a different singleton for each test
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;

    auto &registry = xp::get_registry<test_factory_type>();
    using registry_type = std::decay_t<decltype(registry)>;
    {
        std::vector<std::filesystem::path> search_dirs = {"testplugin_b", "testplugin_a"};
        auto ret = registry.add_plugin("testplugin_a", "plugin_01");
        CHECK(ret == registry_type::add_plugin_result::added);
    }

    CHECK_EQ(registry.size(), 1);

    {
        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 1);
        CHECK(names.count("plugin_01"));
    }

    // check idempotency
    {
        auto ret = registry.add_plugin("testplugin_a", "plugin_01");
        CHECK(ret == registry_type::add_plugin_result::already_exists);
    }

    CHECK_EQ(registry.size(), 1);

    {
        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 1);
        CHECK(names.count("plugin_01"));
    }
}

TEST_CASE("test_xregistry_non_exisiting_single_plugin")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;

    auto &registry = xp::get_registry<test_factory_type>();
    using registry_type = std::decay_t<decltype(registry)>;

    auto ret = registry.add_plugin("testplugin_a", "plugin_NOT_EXISTING");
    CHECK(ret == registry_type::add_plugin_result::not_found);
}

#ifndef XPLUGIN_NO_THREADS
TEST_CASE("parallel_access")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;
    using plugin_registry_type = xp::xplugin_registry<test_factory_type>;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        plugin_registry_type registry;
        auto f = [&]() {
            registry.add_from_directory("testplugin_a");
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
        };
        std::thread thread1(f);
        std::thread thread2(f);
        thread1.join();
        thread2.join();
    }
}
#endif // XPLUGIN_NO_THREADS
