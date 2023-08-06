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
    std::cout << "test_xplugin" << std::endl;

    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using plugin_registry_type = xp::xplugin_registry<factory_base_type>;

    std::cout << "test_xplugin: create registry" << std::endl;
    plugin_registry_type registry;

    std::cout << "test_xplugin: add add_from_directory" << std::endl;
    registry.add_from_directory("testplugin_a");

    {
        CHECK_EQ(registry.size(), 3);

        std::cout << "test_xplugin: plugin_names" << std::endl;
        auto names = registry.plugin_names();
        CHECK_EQ(names.size(), 3);
        CHECK(names.count("plugin_01"));
        CHECK(names.count("plugin_02"));
        CHECK(names.count("plugin_03"));

        // factories are references
        std::cout << "create factories" << std::endl;
        auto factory_01 = registry["plugin_01"];
        auto factory_02 = registry["plugin_02"];
        auto factory_03 = registry["plugin_03"];

        // plugin instances are in unique_ptrs
        std::cout << "create plugins" << std::endl;
        auto plugin_01 = factory_01->create(1, "a");
        auto plugin_02 = factory_02->create(2, "b");
        auto plugin_03 = factory_03->create(3, "c");

        CHECK(plugin_01);
        CHECK(plugin_02);
        CHECK(plugin_03);

        std::cout << "check plugin names" << std::endl;
        CHECK_EQ(plugin_01->name(), "Plugin01");
        CHECK_EQ(plugin_02->name(), "Plugin02");
        CHECK_EQ(plugin_03->name(), "Plugin03");

        std::cout << "check plugin greets" << std::endl;
        CHECK_EQ(plugin_01->greet(), "hello from Plugin01");
        CHECK_EQ(plugin_02->greet(), "hello from Plugin02");
        CHECK_EQ(plugin_03->greet(), "hello from Plugin03");

        std::cout << "iterate" << std::endl;
        // // iterate over plugins
        SUBCASE("begin_end")
        {
            std::cout << "begin_end" << std::endl;
            auto begin = registry.begin();
            auto end = registry.end();
            CHECK(begin != end);

            auto d = std::distance(begin, end);
            CHECK_EQ(d, 3);
        }

        SUBCASE("increment")
        {
            std::cout << "increment" << std::endl;
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
            std::cout << "dereference" << std::endl;
            auto begin = registry.begin();
            auto pair = *begin;
            std::cout << "The value is" << pair.first << std::endl;
            CHECK_EQ(pair.first[0], 'p');
        }

        SUBCASE("iterator-values-copy")
        {
            std::cout << "iterator-values-copy" << std::endl;
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
            std::cout << "iterator-values-ref" << std::endl;
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
            std::cout << "iterator-unpack" << std::endl;
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
    std::cout << "test_xregistry_getter" << std::endl;

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

TEST_CASE("test_xregistry_single_plugin")
{
    std::cout << "test_xregistry_single_plugin" << std::endl;
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
    std::cout << "test_xregistry_single_plugin_with_multiple_search_dirs" << std::endl;
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
    std::cout << "parallel_access" << std::endl;
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;
    using plugin_registry_type = xp::xthread_save_plugin_registry<test_factory_type>;

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
    std::cout << "parallel_iterator_access" << std::endl;
    using base_type = plugin::PluginBase;
    using factory_base_type = xp::xfactory_base<base_type, int, std::string>;
    using test_factory_type = tagged_factory<factory_base_type, __LINE__>;
    using plugin_registry_type = xp::xthread_save_plugin_registry<test_factory_type>;

    for (std::size_t i = 0; i < 1000; ++i)
    {
        plugin_registry_type registry;
        auto f = [&registry]() {
            registry.add_from_directory("testplugin_a");
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
