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
#include <iterator>
#include <memory>
#include <string>
#include <type_traits>
#include <unordered_map>

#include <xplugin/xlazy_shared_library_plugin_factory.hpp>
#include <xplugin/xplugin_config.hpp>

namespace xp::detail
{

    template <class FACTORY_BASE, bool THREAD_SAFE>
    class xplugin_registry_impl;

    /// @brief Iterator for xplugin_registry_impl
    /// @tparam REGISTRY
    /// @tparam BASE_ITERATOR
    template <class REGISTRY, class BASE_ITERATOR>
    class xplugin_registry_iterator
    {
    private:
        
        using registry_type = REGISTRY;
        using factory_base_type = typename registry_type::factory_base_type;

    public:

        using base_iterator_type = BASE_ITERATOR;
        using value_type = std::pair<std::string, factory_base_type *>;
        using reference = value_type &;
        using pointer = value_type *;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::forward_iterator_tag;

        xplugin_registry_iterator() = default;
        xplugin_registry_iterator(const xplugin_registry_iterator &) = default;
        xplugin_registry_iterator &operator=(const xplugin_registry_iterator &) = default;
        xplugin_registry_iterator(xplugin_registry_iterator &&) = default;
        xplugin_registry_iterator &operator=(xplugin_registry_iterator &&) = default;
        ~xplugin_registry_iterator() = default;

        xplugin_registry_iterator(base_iterator_type base_iterator);

        // increment
        xplugin_registry_iterator &operator++();
        xplugin_registry_iterator operator++(int);

        // compare
        bool operator==(const xplugin_registry_iterator &rhs) const;
        bool operator!=(const xplugin_registry_iterator &rhs) const;

        // dereference
        reference operator*() const;
        pointer operator->() const;

    private:

        void set_current_value() const;

        base_iterator_type m_base_iterator;
        mutable value_type m_current_value;
    };

    /// @brief A registry for plugins.
    /// @tparam FACTORY_BASE
    /// @tparam THREAD_SAFE
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

        using self_type = xplugin_registry_impl<FACTORY_BASE, THREAD_SAFE>;
        using factory_base_type = FACTORY_BASE;
        using lazy_shared_library_plugin_factory_type = xlazy_shared_library_plugin_factory<factory_base_type, THREAD_SAFE>;
        using storage_map_type = std::unordered_map<std::string, lazy_shared_library_plugin_factory_type>;
        using const_iterator = xplugin_registry_iterator<self_type, typename storage_map_type::const_iterator>;
        using iterator = xplugin_registry_iterator<self_type, typename storage_map_type::iterator>;

        xplugin_registry_impl(const std::filesystem::path &path,
                                    const std::string &prefix = get_default_library_prefix(),
                                    const std::string &extension = get_default_library_extension());

        factory_base_type *operator[](const std::string &name);

        std::size_t size() const;
        bool empty() const;
        bool contains(const std::string &name) const;

        iterator begin();
        iterator end();

        const_iterator begin() const;
        const_iterator end() const;

        const_iterator cbegin() const;
        const_iterator cend() const;

    private:

        static std::string get_default_library_extension();
        static std::string get_default_library_prefix();

        storage_map_type m_lazy_shared_lib_factories;
    };

} // namespace xp::detail

namespace xp
{
    /// @brief A non-thread-safe registry for plugins.
    /// @tparam FACTORY_BASE
    template <class FACTORY_BASE>
    using xplugin_registry = detail::xplugin_registry_impl<FACTORY_BASE, false>;

    /// @brief A thread-safe registry for plugins.
    /// @tparam FACTORY_BASE
    template <class FACTORY_BASE>
    using xthread_save_plugin_registry = detail::xplugin_registry_impl<FACTORY_BASE, true>;
}

namespace xp::detail
{

    // iterator implementation
    template <class REGISTRY, class BASE_ITERATOR>
    xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::xplugin_registry_iterator(base_iterator_type iter)
        : m_base_iterator(iter)
    {
    }

    template <class REGISTRY, class BASE_ITERATOR>
    xplugin_registry_iterator<REGISTRY, BASE_ITERATOR> &xplugin_registry_iterator<REGISTRY,
                                                                                        BASE_ITERATOR>::operator++()
    {
        ++m_base_iterator;
        return *this;
    }

    template <class REGISTRY, class BASE_ITERATOR>
    xplugin_registry_iterator<REGISTRY, BASE_ITERATOR> xplugin_registry_iterator<REGISTRY,
                                                                                        BASE_ITERATOR>::operator++(int)
    {
        xplugin_registry_iterator<REGISTRY, BASE_ITERATOR> tmp(*this);
        ++(*this);
        return tmp;
    }

    template <class REGISTRY, class BASE_ITERATOR>
    bool xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::operator==(const xplugin_registry_iterator &rhs) const
    {
        return m_base_iterator == rhs.m_base_iterator;
    }
    template <class REGISTRY, class BASE_ITERATOR>
    bool xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::operator!=(const xplugin_registry_iterator &rhs) const
    {
        return m_base_iterator != rhs.m_base_iterator;
    }

    template <class REGISTRY, class BASE_ITERATOR>
    void xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::set_current_value() const
    {
        m_current_value.first = m_base_iterator->first;
        m_current_value.second = m_base_iterator->second.factory();
    }

    template <class REGISTRY, class BASE_ITERATOR>
    typename xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::reference xplugin_registry_iterator<
        REGISTRY, BASE_ITERATOR>::operator*() const
    {
        this->set_current_value();
        return m_current_value;
    }

    template <class REGISTRY, class BASE_ITERATOR>
    typename xplugin_registry_iterator<REGISTRY, BASE_ITERATOR>::pointer xplugin_registry_iterator<
        REGISTRY, BASE_ITERATOR>::operator->() const
    {
        this->set_current_value();
        return &m_current_value;
    }

    // registry implementation
    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::iterator xplugin_registry_impl<FACTORY_BASE,
                                                                                            THREAD_SAVE>::begin()
    {
        return iterator(m_lazy_shared_lib_factories.begin());
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::iterator xplugin_registry_impl<FACTORY_BASE,
                                                                                            THREAD_SAVE>::end()
    {
        return iterator(m_lazy_shared_lib_factories.end());
    }

    // const iterator
    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
        FACTORY_BASE, THREAD_SAVE>::begin() const
    {
        return const_iterator(m_lazy_shared_lib_factories.cbegin());
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
        FACTORY_BASE, THREAD_SAVE>::end() const
    {
        return const_iterator(m_lazy_shared_lib_factories.cend());
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
        FACTORY_BASE, THREAD_SAVE>::cbegin() const
    {
        return const_iterator(m_lazy_shared_lib_factories.cbegin());
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::const_iterator xplugin_registry_impl<
        FACTORY_BASE, THREAD_SAVE>::cend() const
    {
        return const_iterator(m_lazy_shared_lib_factories.cend());
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::xplugin_registry_impl(const std::filesystem::path &path,
                                                                            const std::string &prefix,
                                                                            const std::string &extension)
    {

        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.path().extension() == extension)
            {
                std::string name = entry.path().stem().string();

                // remove prefix
                if (name.substr(0, prefix.size()) == prefix)
                {
                    name = name.substr(prefix.size());
                    m_lazy_shared_lib_factories.emplace(name, entry.path());
                }
            }
        }
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    bool xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::contains(const std::string &name) const
    {
        return m_lazy_shared_lib_factories.find(name) != m_lazy_shared_lib_factories.end();
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    typename xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::factory_base_type *xplugin_registry_impl<
        FACTORY_BASE, THREAD_SAVE>::operator[](const std::string &name)
    {
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
        return m_lazy_shared_lib_factories.size();
    }

    template <class FACTORY_BASE, bool THREAD_SAVE>
    bool xplugin_registry_impl<FACTORY_BASE, THREAD_SAVE>::empty() const
    {
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

} // namespace xp::detail

#endif // XPLUGIN_REGISTRY_HPP
