/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XPLUGIN_REGISTRY_HPP
#define XPLUGIN_REGISTRY_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <xplugin/xplugin_config.hpp>
#include <xplugin/xshared_library.hpp>

namespace xp
{

template <class FACTORY_BASE> class xplugin_registry
{
  public:
    using factory_base_type = FACTORY_BASE;

    using create_plugin_factory_type = factory_base_type *(*)();

    inline xplugin_registry() = default;

    inline std::size_t add_from_directory(const std::filesystem::path &path,
                                          const std::string &prefix = get_default_library_prefix(),
                                          const std::string &extension = get_default_library_extension());

    inline std::unique_ptr<factory_base_type> create_factory(const std::string &name);

    inline std::size_t size() const;
    inline std::unordered_set<std::string> plugin_names();

  private:
    static inline std::string get_default_library_extension();
    static inline std::string get_default_library_prefix();

    std::unordered_map<std::string, xshared_library> m_open_libraries;
    std::unordered_map<std::string, std::filesystem::path> m_locations;
};

template <class FACTORY_BASE>
std::size_t xplugin_registry<FACTORY_BASE>::add_from_directory(const std::filesystem::path &path,
                                                               const std::string &prefix, const std::string &extension)
{
    std::size_t n_added = 0;
    for (const auto &entry : std::filesystem::directory_iterator(path))
    {
        if (entry.path().extension() == extension)
        {
            std::string name = entry.path().stem().string();

            // remove prefix
            if (name.substr(0, prefix.size()) == prefix)
            {
                name = name.substr(prefix.size());
                m_locations[name] = entry.path();
                ++n_added;
            }
        }
    }
    return n_added;
}

template <class FACTORY_BASE> std::unordered_set<std::string> xplugin_registry<FACTORY_BASE>::plugin_names()
{
    std::unordered_set<std::string> res;
    for (const auto &[key, value] : m_locations)
    {
        res.insert(key);
    }
    return res;
}

template <class FACTORY_BASE>
std::unique_ptr<FACTORY_BASE> xplugin_registry<FACTORY_BASE>::create_factory(const std::string &name)
{
    auto find_res = m_locations.find(name);
    if (find_res == m_locations.end())
    {
        throw std::runtime_error("Could not find plugin factory for " + name);
    }

    xshared_library *library_ptr = nullptr;
    auto open_find_res = m_open_libraries.find(name);
    if (open_find_res != m_open_libraries.end())
    {
        library_ptr = &open_find_res->second;
    }
    else
    {
        xshared_library library(find_res->second);
        auto [iter, inserted] = m_open_libraries.emplace(name, std::move(library));
        library_ptr = &(iter->second);
    }

    auto factory = library_ptr->find_symbol<create_plugin_factory_type>("create_plugin_factory")();
    return std::unique_ptr<factory_base_type>(factory);
}

template <class FACTORY_BASE> std::size_t xplugin_registry<FACTORY_BASE>::size() const
{
    return m_locations.size();
}

template <class FACTORY_BASE> static std::string xplugin_registry<FACTORY_BASE>::get_default_library_extension()
{
#ifdef _WIN32
    return ".dll";
#elif __APPLE__
    return ".dylib";
#else
    return ".so";
#endif
}

template <class FACTORY_BASE> static std::string xplugin_registry<FACTORY_BASE>::get_default_library_prefix()
{
#ifdef _WIN32
    return "";
#else
    return "lib";
#endif
}

template <class FACTORY_BASE> xp::xplugin_registry<FACTORY_BASE> &get_registry()
{
    using factory_base_type = FACTORY_BASE;
    static xp::xplugin_registry<factory_base_type> registry;
    return registry;
}
} // namespace xp

#endif // XPLUGIN_REGISTRY_HPP
