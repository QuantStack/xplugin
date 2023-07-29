/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht,      *
* Martin Renou and Dr. Thorsten Beier                                      *
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

#include <iostream>

namespace xp
{
    #if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
    class xunix_shared_library 
    {
    public:
        xunix_shared_library(const xunix_shared_library&) = delete;
        xunix_shared_library& operator=(const xunix_shared_library&) = delete;

        xunix_shared_library(xunix_shared_library&& other) noexcept;
        xunix_shared_library& operator=(xunix_shared_library&& other) noexcept ;

        inline xunix_shared_library(std::filesystem::path path);
        inline ~xunix_shared_library();

        template <class T>
        inline T find_symbol(const std::string& name);
    private:
        void* m_handle;

    };
    using xshared_library = xunix_shared_library;
    #endif // (defined(__linux__) || defined(__unix__) || defined(__APPLE__))

    #ifdef _WIN32
    class xwindows_shared_library 
    {
    public:

        xwindows_shared_library(const xwindows_shared_library&) = delete;
        xwindows_shared_library& operator=(const xwindows_shared_library&) = delete;
        xwindows_shared_library(xwindows_shared_library&& other) noexcept;
        xwindows_shared_library& operator=(xwindows_shared_library&& other) noexcept;

        inline xwindows_shared_library(std::filesystem::path path);
        inline ~xwindows_shared_library();

        template <class T>
        T find_symbol(const std::string& name);
    private:
        HINSTANCE m_handle;
    };

    using xshared_library = xwindows_shared_library;`
    #endif // _WIN32

    #if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))



    xunix_shared_library::xunix_shared_library(xunix_shared_library&& other)noexcept
     {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
    }
    xunix_shared_library& xunix_shared_library::operator=(xunix_shared_library&& other)noexcept
     {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        return *this;
    }


    xunix_shared_library::xunix_shared_library(std::filesystem::path path)
    :   m_handle(nullptr)
    {
        m_handle = dlopen(path.string().c_str(), RTLD_NOW | RTLD_GLOBAL );
        if(!m_handle){
            throw std::runtime_error("could not open shared library from path: " + path.string() + " error: " + dlerror());
        }
        std::cout<<"loaded shared library from path: " << path.string() << std::endl;
    }
    
    template <class T>
    T xunix_shared_library::find_symbol(const std::string& name)
    {   
        dlerror();    /* Clear any existing error */
        char *error;
        void* sym = dlsym(m_handle, name.c_str());
        if(sym == nullptr){
            std::cout<<"could not find symbol: " << name <<"sym" << sym << std::endl;
        }
        if ((error = dlerror()) != NULL)  {
            throw std::runtime_error("could not find symbol: " + name + " error: " + error);
        }
        return reinterpret_cast<T>(sym);
    }

    xunix_shared_library::~xunix_shared_library()
    {
        if(m_handle)
        {
            dlclose(m_handle);
        }
    }
    #endif

    #ifdef _WIN32
    xwindows_shared_library::xwindows_shared_library(xwindows_shared_library&& other)noexcept
     {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
    }
    xwindows_shared_library& xwindows_shared_library::operator=(xwindows_shared_library&& other)noexcept
     {
        m_handle = other.m_handle;
        other.m_handle = nullptr;
        return *this;
    }

    xwindows_shared_library::xwindows_shared_library(std::filesystem::path path)
        :   m_handle(LoadLibrary(path.string().c_str()))
    {
        if(!m_handle){
            throw std::runtime_error("could not open shared library friom path: " + path.string() + " error: " + get_error());
        }
    }

    template <class T>
    T xwindows_shared_library::find_symbol(const std::string& name)
    {   
        void* sym = GetProcAddress(m_handle, name.c_str());
        if(!sym){
            throw std::runtime_error("could not find symbol: " + name + " error: " + get_error());
        }
        return reinterpret_cast<T>(sym);
    }
    xwindows_shared_library::~xwindows_shared_library(){
        if(m_handle)
        {
            FreeLibrary(m_handle);
        }
    }
    #endif // _WIN32

}

#endif // XPLUGIN_XSHARED_LIBRARY_HPP