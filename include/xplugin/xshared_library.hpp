/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   *
* Martin Renou, Dr. Thorsten Beier                                         *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#pragma once
#ifndef XPLUGIN_XSHARED_LIBRARY_HPP
#define XPLUGIN_XSHARED_LIBRARY_HPP

// std includes
#include <string>

// includes from xplugin
#include "xplugin_config.hpp"

// unix specific includes
#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
#include <dlfcn.h>
#include "xplugin/xshared_library.hpp"
#endif

// windows specific includes
#ifdef _WIN32
#include <windows.h>
#include "xplugin/xshared_library.hpp"
#endif

namespace xp
{
    #if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
    class xunix_shared_library 
    {
    public:
        // delete copy constructor
        xunix_shared_library(const xunix_shared_library&) = delete;
        // delete copy assignment
        xunix_shared_library& operator=(const xunix_shared_library&) = delete;

        // move constructor
        xunix_shared_library(xunix_shared_library&& other) noexcept = default;
        // move assignment
        xunix_shared_library& operator=(xunix_shared_library&& other) noexcept = default;

        inline xunix_shared_library(std::filesystem::path path);

        template <class T>
        inline T find_symbol(const std::string& name);
        
        inline void open();
        inline void close();

    private:
        std::filesystem::path m_path;
        void* m_handle;

    };
    using xshared_library = xunix_shared_library;
    #endif // (defined(__linux__) || defined(__unix__) || defined(__APPLE__))

    #ifdef _WIN32
    class xwindows_shared_library 
    {
    public:

        // delete copy constructor
        xwindows_shared_library(const xwindows_shared_library&) = delete;
        // delete copy assignment
        xwindows_shared_library& operator=(const xwindows_shared_library&) = delete;

        // move constructor
        xwindows_shared_library(xwindows_shared_library&& other) noexcept = default;
        // move assignment
        xwindows_shared_library& operator=(xwindows_shared_library&& other) noexcept = default;

        // constructor
        xwindows_shared_library(std::filesystem::path path);

        template <class T>
        T find_symbol(const std::string& name);

        inline void open();
        inline void close();

    private:
        std::filesystem::path m_path;
        HINSTANCE m_handle;
    }
    using xshared_library = xwindows_shared_library;
    #endif // _WIN32

    #if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
    xunix_shared_library::xunix_shared_library(std::filesystem::path path)
    :   m_path(path),
        m_handle(nullptr)
    {
    this->open();
    }

    template <class T>
    T xunix_shared_library::find_symbol(const std::string& name)
    {   
        dlerror();    /* Clear any existing error */
        char *error;
        if(!m_handle){
            throw std::runtime_error("shared library not open");
        }
        void* sym = dlsym(m_handle, name.c_str());
        if ((error = dlerror()) != NULL)  {
            throw std::runtime_error("could not find symbol: " + name + " error: " + error);
        }
        if(!sym){
            throw std::runtime_error("could not find symbol: " + name + " error: " + dlerror());
        }
        return reinterpret_cast<T>(sym);
    }
    void xunix_shared_library::open(){
        if(m_handle){
            throw std::runtime_error("shared library already open");
        }
        m_handle = dlopen(m_path.string().c_str(), RTLD_NOW | RTLD_LOCAL );
        if(!m_handle){
            throw std::runtime_error("could not open shared library friom path: " + m_path.string() + " error: " + dlerror());
        }
    }
    void xunix_shared_library::close(){
        dlerror();    /* Clear any existing error */
        if(!m_handle){
            throw std::runtime_error("shared library not open");
        }
        int ret = dlclose(m_handle);
        if(ret != 0){
            throw std::runtime_error("could not close shared library friom path: " + m_path.string() + " error: " + dlerror());
        }
        m_handle = nullptr;
    }
    #endif

    #ifdef _WIN32
    xwindows_shared_library::xwindows_shared_library(std::filesystem::path path)
        :   m_path(path),
            m_handle(nullptr)
        {
            this->open();
        }

    template <class T>
    T xwindows_shared_library::find_symbol(const std::string& name)
    {   
        if(!m_handle){
            throw std::runtime_error("shared library not open");
        }
        void* sym = GetProcAddress(m_handle, name.c_str());
        if(!sym){
            throw std::runtime_error("could not find symbol: " + name + " error: " + get_error());
        }
        return reinterpret_cast<T>(sym);
    }

    void xwindows_shared_library::open(){
        if(m_handle){
            throw std::runtime_error("shared library already open");
        }
        m_handle = LoadLibrary(m_path.string().c_str());
        if(!m_handle){
            throw std::runtime_error("could not open shared library friom path: " + m_path.string() + " error: " + get_error());
        }
    }
    void xwindows_shared_library::close(){
        if(!m_handle){
            throw std::runtime_error("shared library not open");
        }
        FreeLibrary(m_handle);
        m_handle = nullptr;
    }
    #endif // _WIN32

}

#endif // XPLUGIN_XSHARED_LIBRARY_HPP