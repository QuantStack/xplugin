/***************************************************************************
 * Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht,      *
 * Martin Renou and Dr. Thorsten Beier                                      *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/
#ifndef XPLUGIN_XSHARED_LIBRARY_HPP
#define XPLUGIN_XSHARED_LIBRARY_HPP

// std includes
#include <string>

// path
#include <filesystem>

// includes from xplugin
#include "xplugin_config.hpp"

// unix specific includes
#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
#include "xplugin/xshared_library.hpp"
#include <dlfcn.h>
#endif

// windows specific includes
#ifdef _WIN32
#include "xplugin/xshared_library.hpp"
#include <windows.h>
#endif

#include <iostream>

namespace xp
{
#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
class xshared_library
{
  public:
    xshared_library(const xshared_library &) = delete;
    xshared_library &operator=(const xshared_library &) = delete;

    inline xshared_library(xshared_library &&other) noexcept;
    inline xshared_library &operator=(xshared_library &&other) noexcept;

    inline xshared_library(std::filesystem::path path);
    inline ~xshared_library();

    template <class T>
    inline T find_symbol(const std::string &name);

  private:
    void *m_handle;
};
#endif // (defined(__linux__) || defined(__unix__) || defined(__APPLE__))

#ifdef _WIN32
class xshared_library
{
  public:
    xshared_library(const xshared_library &) = delete;
    xshared_library &operator=(const xshared_library &) = delete;
    inline xshared_library(xshared_library &&other) noexcept;
    inline xshared_library &operator=(xshared_library &&other) noexcept;

    inline xshared_library(std::filesystem::path path);
    inline ~xshared_library();

    template <class T>
    inline T find_symbol(const std::string &name);

  private:
    HINSTANCE m_handle;
};

#endif // _WIN32

#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))

xshared_library::xshared_library(xshared_library &&other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = nullptr;
}
xshared_library &xshared_library::operator=(xshared_library &&other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = nullptr;
    return *this;
}

xshared_library::xshared_library(std::filesystem::path path)
    : m_handle(nullptr)
{
    m_handle = dlopen(path.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
    if (!m_handle)
    {
        throw std::runtime_error("could not open shared library from path: " + path.string() + " error: " + dlerror());
    }
}

template <class T>
T xshared_library::find_symbol(const std::string &name)
{
    dlerror(); /* Clear any existing error */
    char *error;
    void *sym = dlsym(m_handle, name.c_str());
    if (sym == nullptr)
    {
        std::cout << "could not find symbol: " << name << "sym" << sym << std::endl;
    }
    if ((error = dlerror()) != NULL)
    {
        throw std::runtime_error("could not find symbol: " + name + " error: " + error);
    }
    return reinterpret_cast<T>(sym);
}

xshared_library::~xshared_library()
{
    if (m_handle)
    {
        dlclose(m_handle);
    }
}
#endif

#ifdef _WIN32
xshared_library::xshared_library(xshared_library &&other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = nullptr;
}
xshared_library &xshared_library::operator=(xshared_library &&other) noexcept
{
    m_handle = other.m_handle;
    other.m_handle = nullptr;
    return *this;
}

xshared_library::xshared_library(std::filesystem::path path)
    : m_handle(LoadLibrary(path.string().c_str()))
{
    if (!m_handle)
    {
        char buffer[255];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buffer, sizeof(buffer), 0);
        throw std::runtime_error("could not open shared library friom path: " + path.string() + " error: " + buffer);
    }
}

template <class T>
T xshared_library::find_symbol(const std::string &name)
{
    void *sym = GetProcAddress(m_handle, name.c_str());
    if (!sym)
    {
        char buffer[255];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, GetLastError(), 0, buffer, sizeof(buffer), 0);
        throw std::runtime_error("could not find symbol: " + name + " error: " + buffer);
    }
    return reinterpret_cast<T>(sym);
}
xshared_library::~xshared_library()
{
    if (m_handle)
    {
        FreeLibrary(m_handle);
    }
}
#endif // _WIN32

} // namespace xp

#endif // XPLUGIN_XSHARED_LIBRARY_HPP
