/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XPLUGIN_REGISTRY_impl_HPP
#define XPLUGIN_REGISTRY_impl_HPP

#include <filesystem>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

#include <xplugin/xlazy_shared_library_plugin_factory.hpp>
#include <xplugin/xplugin_config.hpp>
#include <xplugin/xthreads.hpp>
namespace xp
{

namespace detail
{

template <class FACTORY_BASE, bool THREAD_SAFE>
class xplugin_registry_impl
{
  public:
    enum class add_plugin_result
    {
        not_found,
        already_exists,
        added
    };

    using factory_base_type = FACTORY_BASE;

    inline xplugin_registry_impl() = default;

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

    inline factory_base_type &operator[](const std::string &name);

    inline std::size_t size() const;
    inline std::unordered_set<std::string> plugin_names();
    inline bool contains(const std::string &name) const;

  private:
    using mutex_type = xmutex_t<THREAD_SAFE>;
    using scoped_lock_type = xscoped_lock_t<THREAD_SAFE, mutex_type>;

    using lazy_shared_library_plugin_factory_type = xlazy_shared_library_plugin_factory<factory_base_type, THREAD_SAFE>;

    static inline std::string get_default_library_extension();
    static inline std::string get_default_library_prefix();

    std::unordered_map<std::string, lazy_shared_library_plugin_factory_type> m_lazy_shared_lib_factories;
    mutable mutex_type m_mutex;
};

} // namespace detail

template <class FACTORY_BASE>
using xplugin_registry = detail::xplugin_registry_impl<FACTORY_BASE, false>;

template <class FACTORY_BASE>
using xthread_save_plugin_registry = detail::xplugin_registry_impl<FACTORY_BASE, true>;

template <class FACTORY_BASE>
xp::xplugin_registry<FACTORY_BASE> &get_registry()
{
    using factory_base_type = FACTORY_BASE;
    static xp::xplugin_registry<factory_base_type> registry;
    return registry;
}

template <class FACTORY_BASE>
xp::xthread_save_plugin_registry<FACTORY_BASE> &get_thead_save_registry()
{
    using factory_base_type = FACTORY_BASE;
    static xp::xthread_save_plugin_registry<factory_base_type> registry;
    return registry;
}

namespace detail
{

template <class FACTORY_BASE, bool THREAD_SAVE>
std::size_t xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::add_from_directory(const std::filesystem::path &path,
                                                                                 const std::string &prefix,
                                                                                 const std::string &extension)
{
    scoped_lock_type lock(m_mutex);

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
                if (m_lazy_shared_lib_factories.find(name) != m_lazy_shared_lib_factories.end())
                {
                    continue;
                }
                else
                {
                    m_lazy_shared_lib_factories.emplace(name, entry.path());
                    ++n_added;
                }
            }
        }
    }
    return n_added;
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::add_plugin_result xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::add_plugin(const std::filesystem::path &path, const std::string &name,
                                           const std::string &prefix, const std::string &extension)
{
    scoped_lock_type lock(m_mutex);

    if (m_lazy_shared_lib_factories.find(name) != m_lazy_shared_lib_factories.end())
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
        m_lazy_shared_lib_factories.emplace(name, path / (prefix + name + extension));
        return add_plugin_result::added;
    }
}

template <class FACTORY_BASE, bool THREAD_SAVE>
template <class PATH_ITERATOR>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::add_plugin_result xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::add_plugin(PATH_ITERATOR begin, PATH_ITERATOR end, const std::string name,
                                           const std::string &prefix, const std::string &extension)
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

template <class FACTORY_BASE, bool THREAD_SAVE>
std::unordered_set<std::string> xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::plugin_names()
{
    scoped_lock_type lock(m_mutex);

    std::unordered_set<std::string> res;
    for (const auto &[key, value] : m_lazy_shared_lib_factories)
    {
        res.insert(key);
    }
    return res;
}

template <class FACTORY_BASE, bool THREAD_SAVE>
bool xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::contains(const std::string &name) const
{
    scoped_lock_type lock(m_mutex);
    return m_lazy_shared_lib_factories.find(name) != m_lazy_shared_lib_factories.end();
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::factory_base_type &xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::operator[](const std::string &name)
{
    scoped_lock_type lock(m_mutex);
    auto find_res = m_lazy_shared_lib_factories.find(name);
    if (find_res == m_lazy_shared_lib_factories.end())
    {
        throw std::runtime_error("Could not find plugin factory for " + name);
    }
    return find_res->second.factory();
}

template <class FACTORY_BASE, bool THREAD_SAVE>
std::size_t xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::size() const
{
    scoped_lock_type lock(m_mutex);
    return m_lazy_shared_lib_factories.size();
}

template <class FACTORY_BASE, bool THREAD_SAVE>
std::string xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::get_default_library_extension()
{
#ifdef _WIN32
    return ".dll";
#elif __APPLE__
    return ".dylib";
#else
    return ".so";
#endif
}

template <class FACTORY_BASE, bool THREAD_SAVE>
std::string xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::get_default_library_prefix()
{
#ifdef _WIN32
    return "";
#else
    return "lib";
#endif
}

} // namespace detail

} // namespace xp

#endif // XPLUGIN_REGISTRY_impl_HPP
