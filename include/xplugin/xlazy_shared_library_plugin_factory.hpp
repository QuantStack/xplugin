/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XLAZY_SHARED_LIBRARY_PLUGIN_FACTORY_HPP
#define XLAZY_SHARED_LIBRARY_PLUGIN_FACTORY_HPP

#include <filesystem>
#include <memory>
#include <xplugin/xshared_library.hpp>
#include <xplugin/xthreads.hpp>

namespace xp
{

// lazy storage of a shared library
// and the factory it contains.
// Both are loaded / created on demand
template <class FACTORY_BASE, bool THREAD_SAFE>
class xlazy_shared_library_plugin_factory
{
  public:
    using factory_base_type = FACTORY_BASE;
    xlazy_shared_library_plugin_factory(const xlazy_shared_library_plugin_factory &) = delete;
    xlazy_shared_library_plugin_factory &operator=(const xlazy_shared_library_plugin_factory &) = delete;

    xlazy_shared_library_plugin_factory(xlazy_shared_library_plugin_factory &&other) = delete;
    xlazy_shared_library_plugin_factory &operator=(xlazy_shared_library_plugin_factory &&other) = delete;

    ~xlazy_shared_library_plugin_factory() = default;

    inline xlazy_shared_library_plugin_factory(const std::filesystem::path &path);

    inline const std::filesystem::path &path() const noexcept;

    factory_base_type *factory() const;

  private:
    using create_plugin_factory_type = factory_base_type *(*)();
    using mutex_type = xmutex_t<THREAD_SAFE>;
    using scoped_lock_type = xscoped_lock_t<THREAD_SAFE, mutex_type>;

    mutable mutex_type m_mutex;
    std::filesystem::path m_path;
    mutable std::unique_ptr<xshared_library> m_library;
    mutable std::unique_ptr<factory_base_type> m_factory;
};

template <class FACTORY_BASE, bool THREAD_SAFE>
inline xlazy_shared_library_plugin_factory<FACTORY_BASE, THREAD_SAFE>::xlazy_shared_library_plugin_factory(
    const std::filesystem::path &path)
    : m_path(path),
      m_library()
{
}

template <class FACTORY_BASE, bool THREAD_SAFE>
inline typename xlazy_shared_library_plugin_factory<FACTORY_BASE, THREAD_SAFE>::factory_base_type *
xlazy_shared_library_plugin_factory<FACTORY_BASE, THREAD_SAFE>::factory() const
{
    scoped_lock_type lock(m_mutex);
    if (!m_factory)
    {

        m_library.reset(new xshared_library(m_path));
        m_factory.reset(m_library->find_symbol<create_plugin_factory_type>("create_plugin_factory")());
    }
    return m_factory.get();
}

template <class FACTORY_BASE, bool THREAD_SAFE>
const std::filesystem::path &xlazy_shared_library_plugin_factory<FACTORY_BASE, THREAD_SAFE>::path() const noexcept
{
    return m_path;
}

} // namespace xp

#endif // XLAZY_SHARED_LIBRARY_PLUGIN_FACTORY_HPP
