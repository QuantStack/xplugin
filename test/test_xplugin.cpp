#include <test.hpp>

#include <set>
#include <vector>

#include <xplugin/xfactory.hpp>
#include <xplugin/xplugin_registry.hpp>

#include <testplugin_a/plugin_base.hpp>

#ifndef XPLUGIN_NO_THREADS
#include <thread>
#endif

#include <algorithm>

TEST_CASE("test_xplugin")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    plugin_registry_type registry("testplugin_a");

    {
        CHECK_EQ(registry.size(), 3);

        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 3);
        CHECK(names.count("plugin_01"));
        CHECK(names.count("plugin_02"));
        CHECK(names.count("plugin_03"));

        // factories are references
        auto factory_01 = registry["plugin_01"];
        auto factory_02 = registry["plugin_02"];
        auto factory_03 = registry["plugin_03"];

        // plugin instances are in unique_ptrs
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

        // // iterate over plugins
        SUBCASE("begin_end")
        {
            std::cout << "begin" << std::endl;
            auto begin = registry.begin();
            auto begin2 = registry.begin();
            CHECK(begin == begin2);

            auto end = registry.end();
            CHECK(begin != end);
            CHECK(end != begin);
            CHECK(begin == begin);
            CHECK(end == end);

            CHECK(begin == registry.begin());
            CHECK(end == registry.end());

            auto d = std::distance(begin, end);
            CHECK_EQ(d, 3);
        }

        SUBCASE("increment")
        {
            auto begin = registry.begin();
            auto end = registry.end();
            CHECK(begin != end);

            auto d = std::distance(begin, end);
            CHECK_EQ(d, 3);

            ++begin;
            CHECK(begin != end);
            d = std::distance(begin, end);
            CHECK_EQ(d, 2);

            ++begin;
            CHECK(begin != end);
            d = std::distance(begin, end);
            CHECK_EQ(d, 1);

            ++begin;
            CHECK(begin == end);
            d = std::distance(begin, end);
            CHECK_EQ(d, 0);
        }

        SUBCASE("dereference")
        {
            auto begin = registry.begin();
            auto pair = *begin;
            CHECK_EQ(pair.first[0], 'p');
        }

        SUBCASE("iterator-values-copy")
        {
            std::set<std::string> plugin_names;
            for (auto p : registry)
            {
                p.second->create(1, "a")->greet();
                plugin_names.insert(p.first);
            }

            CHECK_EQ(plugin_names.size(), 3);
            CHECK(plugin_names.count("plugin_01"));
            CHECK(plugin_names.count("plugin_02"));
            CHECK(plugin_names.count("plugin_03"));
        }
        SUBCASE("iterator-values-ref")
        {
            std::set<std::string> plugin_names;
            for (auto &p : registry)
            {
                p.second->create(1, "a")->greet();
                plugin_names.insert(p.first);
            }

            CHECK_EQ(plugin_names.size(), 3);
            CHECK(plugin_names.count("plugin_01"));
            CHECK(plugin_names.count("plugin_02"));
            CHECK(plugin_names.count("plugin_03"));
        }
        SUBCASE("iterator-unpack")
        {
            std::set<std::string> plugin_names;
            for (auto [name, factory] : registry)
            {
                factory->create(1, "a")->greet();
                plugin_names.insert(name);
            }
            CHECK_EQ(plugin_names.size(), 3);
            CHECK(plugin_names.count("plugin_01"));
            CHECK(plugin_names.count("plugin_02"));
            CHECK(plugin_names.count("plugin_03"));
        }
    }
}

TEST_CASE("test_xregistry_getter")
{

    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;
    plugin_registry_type registry("testplugin_a");

    CHECK_EQ(registry.size(), 3);

    auto names = registry.plugin_names();
    CHECK_EQ(names.size(), 3);
    CHECK(names.count("plugin_01"));
    CHECK(names.count("plugin_02"));
    CHECK(names.count("plugin_03"));

    // factories are references
    auto factory_01 = registry["plugin_01"];
    auto factory_02 = registry["plugin_02"];
    auto factory_03 = registry["plugin_03"];

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
        registry = new plugin_registry_type("testplugin_a");
    }

    for (auto &registry : registries)
    {
        CHECK_EQ(registry->size(), 3);

        auto names = registry->plugin_names();
        CHECK_EQ(names.size(), 3);
        CHECK(names.count("plugin_01"));
        CHECK(names.count("plugin_02"));
        CHECK(names.count("plugin_03"));

        // factories are references
        auto factory_01 = registry->operator[]("plugin_01");
        auto factory_02 = registry->operator[]("plugin_02");
        auto factory_03 = registry->operator[]("plugin_03");

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

#ifndef XPLUGIN_NO_THREADS
TEST_CASE("parallel_access")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xthread_save_plugin_registry<factory_base_type>;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        plugin_registry_type registry("testplugin_a");
        auto f = [&]() {
            auto names = registry.plugin_names();
            CHECK_EQ(names.size(), 3);
            CHECK(names.count("plugin_01"));
            CHECK(names.count("plugin_02"));
            CHECK(names.count("plugin_03"));

            // factories are references
            auto factory_01 = registry["plugin_01"];
            auto factory_02 = registry["plugin_02"];
            auto factory_03 = registry["plugin_03"];

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

#ifndef XPLUGIN_NO_THREADS
TEST_CASE("parallel_iterator_access")
{
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xthread_save_plugin_registry<factory_base_type>;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        plugin_registry_type registry("testplugin_a");
        auto f = [&registry]() {
            for (auto name_factory : registry)
            {
                auto name = name_factory.first;
                auto factory = name_factory.second;

                CHECK(name[0] == 'p');
                CHECK(factory);
                auto plugin = factory->create(1, "a");
                CHECK(plugin);
                CHECK_EQ(plugin->name()[0], 'P');
            }
        };
        std::thread thread1(f);
        std::thread thread2(f);
        thread1.join();
        thread2.join();
    }
}
#endif // XPLUGIN_NO_THREADS
