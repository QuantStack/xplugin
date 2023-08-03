/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XLAZY_SHARED_LIBRARY_HPP
#define XLAZY_SHARED_LIBRARY_HPP

#include <filesystem>
#include <memory>
#include <xplugin/xshared_library.hpp>
#include <xplugin/xthreads.hpp>

namespace xp
{

class xlazy_shared_library
{
  public:
    xlazy_shared_library(const xlazy_shared_library &) = delete;
    xlazy_shared_library &operator=(const xlazy_shared_library &) = delete;

    xlazy_shared_library(xlazy_shared_library &&other) = delete;
    xlazy_shared_library &operator=(xlazy_shared_library &&other) = delete;

    ~xlazy_shared_library() = default;

    inline xlazy_shared_library(const std::filesystem::path &path);

    template <class T>
    inline T find_symbol(const std::string &name);

    inline const std::filesystem::path &path() const noexcept;

  private:
    xp::xmutex m_mutex;
    std::filesystem::path m_path;
    std::unique_ptr<xshared_library> m_library;
};

inline xlazy_shared_library::xlazy_shared_library(const std::filesystem::path &path)
    : m_path(path),
      m_library()
{
}

template <class T>
inline T xlazy_shared_library::find_symbol(const std::string &name)
{
    xscoped_lock<xp::xmutex> lock(m_mutex);
    if (!m_library)
    {
        m_library.reset(new xshared_library(m_path));
    }
    return m_library->find_symbol<T>(name);
}

const std::filesystem::path &xlazy_shared_library::path() const noexcept
{
    return m_path;
}

} // namespace xp

#endif // XLAZY_SHARED_LIBRARY_HPP
