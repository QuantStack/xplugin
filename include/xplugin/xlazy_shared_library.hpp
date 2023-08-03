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

namespace xp
{
// note that this class is not thread safe but the
// xplugin_registry containing the xlazy_shared_library objects is

class xlazy_shared_library
{
  public:
    xlazy_shared_library(const xlazy_shared_library &) = delete;
    xlazy_shared_library &operator=(const xlazy_shared_library &) = delete;

    xlazy_shared_library(xlazy_shared_library &&other) = default;
    xlazy_shared_library &operator=(xlazy_shared_library &&other) = default;

    ~xlazy_shared_library() = default;

    inline xlazy_shared_library(const std::filesystem::path &path);

    template <class T>
    inline T find_symbol(const std::string &name);

    const std::filesystem::path &path() const noexcept;

  private:
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
    if (!m_library)
    {
        m_library.reset(new xshared_library(m_path));
    }
    return m_library->find_symbol<T>(name);
}

} // namespace xp

#endif // XLAZY_SHARED_LIBRARY_HPP
