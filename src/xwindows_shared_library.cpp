/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   *
* Martin Renou                                                             *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#ifdef _WIN32

#include <windows.h>
#include "xplugin/xshared_library.hpp"

namespace xp
{
    class xwindows_shared_library : public xshared_library
    {
    public:

    private:
        
        xwindows_shared_library(HINSTANCE handle);
        virtual ~xwindows_shared_library();

    private:

        void* find_symbol(const std::string& name);

        HINSTANCE m_handle;
    };

    xwindows_shared_library::xwindows_shared_library(HINSTANCE handle)
        : m_handle(handle)
    {
    }

    xwindows_shared_library::~xwindows_shared_library()
    {
        FreeLibrary(m_handle);
    }

    void* xindows_shared_library::find_symbol(const std::string& name)
    {
        void* sym = GetProcAddress(name.c_str());
        return sym;
    }

    const char* xwindows_shared_library::get_error()
    {
        char buffer[255];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM,0,GetLastError(),
                      0, buffer,sizeof(buffer),0);
        return buffer;
    }

    xshared_library* xshared_library::open(const std::string& name)
    {
        HINSTANCE handle = LoadLibrary(name.c_str());
        return handle ? new xwindows_shared_library(handle) : nullptr;
    }

}

#endif

