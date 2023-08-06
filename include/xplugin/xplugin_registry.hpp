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
#include <iterator>
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
class xplugin_registry_impl;

template <class REGISTRY>
class xplugin_registry_iterator
{
  private:
    using registry_type = REGISTRY;
    using factory_base_type = typename registry_type::factory_base_type;

  public:
    using value_type = std::pair<std::string, factory_base_type *>;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;

    inline xplugin_registry_iterator() = default;
    inline xplugin_registry_iterator(const xplugin_registry_iterator &) = default;
    inline xplugin_registry_iterator &operator=(const xplugin_registry_iterator &) = default;
    inline xplugin_registry_iterator(xplugin_registry_iterator &&) = default;
    inline xplugin_registry_iterator &operator=(xplugin_registry_iterator &&) = default;
    inline ~xplugin_registry_iterator() = default;

    inline xplugin_registry_iterator(registry_type *registry, bool end = false);

    // increment
    inline xplugin_registry_iterator &operator++();
    inline xplugin_registry_iterator operator++(int);

    // compare
    inline bool operator==(const xplugin_registry_iterator &rhs) const;
    inline bool operator!=(const xplugin_registry_iterator &rhs) const;

    // dereference
    inline reference operator*() const;
    inline pointer operator->() const;

  private:
    void set_current_value() const;

    registry_type *m_registry = nullptr;
    std::unordered_set<std::string> m_names;
    std::unordered_set<std::string>::const_iterator m_name_iterator;
    mutable value_type m_current_value = {"", nullptr};
};

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
    using const_iterator = xplugin_registry_iterator<xplugin_registry_impl<factory_base_type, THREAD_SAFE>>;
    using iterator = const_iterator;

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

    inline factory_base_type *operator[](const std::string &name);

    inline std::size_t size() const;
    inline bool empty() const;
    inline std::unordered_set<std::string> plugin_names();
    inline bool contains(const std::string &name) const;

    inline iterator begin();
    inline iterator end();

    inline const_iterator begin() const;
    inline const_iterator end() const;

    inline const_iterator cbegin() const;
    inline const_iterator cend() const;

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

// iterator implementation
template <class REGISTRY>
inline xplugin_registry_iterator<REGISTRY>::xplugin_registry_iterator(registry_type *registry, bool end)
    : m_registry(registry)
{
    if (m_registry)
    {
        m_names = m_registry->plugin_names();
        if (end)
        {
            m_name_iterator = m_names.cend();
        }
        else
        {
            m_name_iterator = m_names.cbegin();
        }
    }
}

template <class REGISTRY>
inline xplugin_registry_iterator<REGISTRY> &xplugin_registry_iterator<REGISTRY>::operator++()
{
    ++m_name_iterator;
    return *this;
}

template <class REGISTRY>
inline xplugin_registry_iterator<REGISTRY> xplugin_registry_iterator<REGISTRY>::operator++(int)
{
    xplugin_registry_iterator tmp(*this);
    ++(*this);
    return tmp;
}

template <class REGISTRY>
inline bool xplugin_registry_iterator<REGISTRY>::operator==(const xplugin_registry_iterator &rhs) const
{
    return m_registry == rhs.m_registry && m_name_iterator == rhs.m_name_iterator;
}

template <class REGISTRY>
inline void xplugin_registry_iterator<REGISTRY>::set_current_value() const
{
    if (m_registry && m_name_iterator != m_names.end())
    {
        m_current_value.first = *m_name_iterator;
        m_current_value.second = (*m_registry)[m_current_value.first];
    }
}

template <class REGISTRY>
inline bool xplugin_registry_iterator<REGISTRY>::operator!=(const xplugin_registry_iterator &rhs) const
{
    return (m_registry != rhs.m_registry) || (m_name_iterator != rhs.m_name_iterator);
}

template <class REGISTRY>
inline typename xplugin_registry_iterator<REGISTRY>::reference xplugin_registry_iterator<REGISTRY>::operator*() const
{
    this->set_current_value();
    return m_current_value;
}

template <class REGISTRY>
inline typename xplugin_registry_iterator<REGISTRY>::pointer xplugin_registry_iterator<REGISTRY>::operator->() const
{
    this->set_current_value();
    return &m_current_value;
}

// xplugin_registry_impl implementation

// iterator
template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::iterator xplugin_registry_impl<FACTORY_BASE,
                                                                                          THREAD_SAVE>::begin()
{
    return iterator(this, false);
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::iterator xplugin_registry_impl<FACTORY_BASE,
                                                                                          THREAD_SAVE>::end()
{
    return iterator(this, true);
}

// const iterator
template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::begin() const
{
    return const_iterator(this, false);
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::end() const
{
    return const_iterator(this, true);
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::cbegin() const
{
    return const_iterator(this, false);
}

template <class FACTORY_BASE, bool THREAD_SAVE>
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::cend() const
{
    return const_iterator(this, true);
}

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
typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::factory_base_type *xplugin_registry_impl<
    FACTORY_BASE, THREAD_SAVE>::operator[](const std::string &name)
{
    scoped_lock_type lock(m_mutex);
    auto find_res = m_lazy_shared_lib_factories.find(name);
    if (find_res == m_lazy_shared_lib_factories.end())
    {
        throw std::runtime_error("Could not find plugin factory for " + name);
    }
    return &find_res->second.factory();
}

template <class FACTORY_BASE, bool THREAD_SAVE>
std::size_t xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::size() const
{
    scoped_lock_type lock(m_mutex);
    return m_lazy_shared_lib_factories.size();
}

template <class FACTORY_BASE, bool THREAD_SAVE>
bool xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::empty() const
{
    scoped_lock_type lock(m_mutex);
    return m_lazy_shared_lib_factories.empty();
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
