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

#include <xplugin/xlazy_shared_library.hpp>
#include <xplugin/xplugin_config.hpp>
#include <xplugin/xthreads.hpp>

namespace xp
{

template <class FACTORY_BASE>
class xplugin_registry
{
  public:
    enum class add_plugin_result
    {
        not_found,
        already_exists,
        added
    };

    using factory_base_type = FACTORY_BASE;

    inline xplugin_registry() = default;

    // add all plugins from a directory
    inline std::size_t add_from_directory(const std::filesystem::path &path,
                                          const std::string &prefix = get_default_library_prefix(),
                                          const std::string &extension = get_default_library_extension());

    inline add_plugin_result add_plugin(const std::filesystem::path &path, const std::string &name,
                                        const std::string &prefix = get_default_library_prefix(),
                                        const std::string &extension = get_default_library_extension());

    template <class PATH_ITERATOR>
    inline add_plugin_result add_plugin(PATH_ITERATOR begin, PATH_ITERATOR end, const std::string name,
                                        const std::string &prefix = get_default_library_prefix(),
                                        const std::string &extension = get_default_library_extension());

    inline std::unique_ptr<factory_base_type> create_factory(const std::string &name);

    inline std::size_t size() const;
    inline std::unordered_set<std::string> plugin_names();
    inline bool contains(const std::string &name) const;

  private:
    using create_plugin_factory_type = factory_base_type *(*)();

    static inline std::string get_default_library_extension();
    static inline std::string get_default_library_prefix();

    std::unordered_map<std::string, xlazy_shared_library> m_libraries;
    mutable xmutex m_mutex;
};

template <class FACTORY_BASE>
std::size_t xplugin_registry<FACTORY_BASE>::add_from_directory(const std::filesystem::path &path,
                                                               const std::string &prefix, const std::string &extension)
{
    xscoped_lock<xmutex> lock(m_mutex);

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

                // check if name is already in registry
                if (m_libraries.find(name) != m_libraries.end())
                {
                    continue;
                }
                else
                {
                    m_libraries.emplace(name, entry.path());
                    ++n_added;
                }
            }
        }
    }
    return n_added;
}

template <class FACTORY_BASE>
typename xplugin_registry<FACTORY_BASE>::add_plugin_result xplugin_registry<FACTORY_BASE>::add_plugin(
    const std::filesystem::path &path, const std::string &name, const std::string &prefix, const std::string &extension)
{
    xscoped_lock<xmutex> lock(m_mutex);

    if (m_libraries.find(name) != m_libraries.end())
    {
        return add_plugin_result::already_exists;
    }
    else
    {
        const auto filename = path / (prefix + name + extension);
        if (!std::filesystem::exists(filename))
        {
            return add_plugin_result::not_found;
        }
        m_libraries.emplace(name, path / (prefix + name + extension));
        return add_plugin_result::added;
    }
}

template <class FACTORY_BASE>
template <class PATH_ITERATOR>
typename xplugin_registry<FACTORY_BASE>::add_plugin_result xplugin_registry<FACTORY_BASE>::add_plugin(
    PATH_ITERATOR begin, PATH_ITERATOR end, const std::string name, const std::string &prefix,
    const std::string &extension)
{
    while (begin != end)
    {
        auto res = add_plugin(*begin, name, prefix, extension);
        if (res == add_plugin_result::added || res == add_plugin_result::already_exists)
        {
            return res;
        }
        ++begin;
    }
    return add_plugin_result::not_found;
}

template <class FACTORY_BASE>
std::unordered_set<std::string> xplugin_registry<FACTORY_BASE>::plugin_names()
{
    xscoped_lock<xmutex> lock(m_mutex);

    std::unordered_set<std::string> res;
    for (const auto &[key, value] : m_libraries)
    {
        res.insert(key);
    }
    return res;
}

template <class FACTORY_BASE>
bool xplugin_registry<FACTORY_BASE>::contains(const std::string &name) const
{
    xscoped_lock<xmutex> lock(m_mutex);
    return m_libraries.find(name) != m_libraries.end();
}

template <class FACTORY_BASE>
std::unique_ptr<FACTORY_BASE> xplugin_registry<FACTORY_BASE>::create_factory(const std::string &name)
{
    xscoped_lock<xmutex> lock(m_mutex);
    auto find_res = m_libraries.find(name);
    if (find_res == m_libraries.end())
    {
        throw std::runtime_error("Could not find plugin factory for " + name);
    }
    auto &library = find_res->second;
    auto factory = library.template find_symbol<create_plugin_factory_type>("create_plugin_factory")();
    return std::unique_ptr<factory_base_type>(factory);
}

template <class FACTORY_BASE>
std::size_t xplugin_registry<FACTORY_BASE>::size() const
{
    xscoped_lock<xmutex> lock(m_mutex);
    return m_libraries.size();
}

template <class FACTORY_BASE>
std::string xplugin_registry<FACTORY_BASE>::get_default_library_extension()
{
#ifdef _WIN32
    return ".dll";
#elif __APPLE__
    return ".dylib";
#else
    return ".so";
#endif
}

template <class FACTORY_BASE>
std::string xplugin_registry<FACTORY_BASE>::get_default_library_prefix()
{
#ifdef _WIN32
    return "";
#else
    return "lib";
#endif
}

template <class FACTORY_BASE>
xp::xplugin_registry<FACTORY_BASE> &get_registry()
{
    using factory_base_type = FACTORY_BASE;
    static xp::xplugin_registry<factory_base_type> registry;
    return registry;
}
} // namespace xp

#endif // XPLUGIN_REGISTRY_HPP
