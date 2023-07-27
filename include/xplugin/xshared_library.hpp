/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   *
* Martin Renou, Dr. Thorsten Beier                                         *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/
#pragma once
#include <string>

#include "xplugin_config.hpp"

#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))
#include <dlfcn.h>
#include "xplugin/xshared_library.hpp"
#endif

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


        xunix_shared_library(std::filesystem::path path)
        :   m_path(path),
            m_handle(nullptr)
        {
           this->open();
        }

        template <class T>
        T find_symbol(const std::string& name)
        {   
            if(!m_handle){
                throw std::runtime_error("shared library not open");
            }
            void* sym = dlsym(m_handle, name.c_str());
            if(!sym){
                throw std::runtime_error("could not find symbol: " + name + " error: " + dlerror());
            }
            return reinterpret_cast<T>(sym);
        }
        void open(){
            if(m_handle){
                throw std::runtime_error("shared library already open");
            }
            m_handle = dlopen(m_path.string().c_str(), RTLD_NOW | RTLD_GLOBAL);
            if(!m_handle){
                throw std::runtime_error("could not open shared library friom path: " + m_path.string() + " error: " + dlerror());
            }
        }
        void close(){
            if(!m_handle){
                throw std::runtime_error("shared library not open");
            }
            dlclose(m_handle);
            m_handle = nullptr;
        }

    private:
        std::filesystem::path m_path;
        void* m_handle;

    };
    using xshared_library = xunix_shared_library;
    #endif

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

        xwindows_shared_library(std::filesystem::path path)
        :   m_path(path),
            m_handle(nullptr)
        {
            this->open();
        }
        template <class T>
        T find_symbol(const std::string& name)
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

        void open(){
            if(m_handle){
                throw std::runtime_error("shared library already open");
            }
            m_handle = LoadLibrary(m_path.string().c_str());
            if(!m_handle){
                throw std::runtime_error("could not open shared library friom path: " + m_path.string() + " error: " + get_error());
            }
        }
        void close(){
            if(!m_handle){
                throw std::runtime_error("shared library not open");
            }
            FreeLibrary(m_handle);
            m_handle = nullptr;
        }


    private:
        std::filesystem::path m_path;
        HINSTANCE m_handle;

    }
    using xshared_library = xunix_shared_library;
    #endif
}