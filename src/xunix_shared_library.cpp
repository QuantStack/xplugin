/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   *
* Martin Renou                                                             *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#if (defined(__linux__) || defined(__unix__) || defined(__APPLE__))

#include <dlfcn.h>
#include "xplugin/xshared_library.hpp"

namespace xp
{
    class xunix_shared_library : public xshared_library
    {
    public:

        xunix_shared_library(void* handle);
        virtual ~xunix_shared_library();

    private:

        void* find_symbol(const std::string& name);

        void* m_handle;
    };

    xunix_shared_library::xunix_shared_library(void* handle)
        : m_handle(handle)
    {
    }

    xunix_shared_library::~xunix_shared_library()
    {
        dlclose(m_handle);
        m_handle = nullptr;
    }

    void* xunix_shared_library::find_symbol(const std::string& name)
    {
        void* sym = dlsym(m_handle, name.c_str());
        return sym;
    }

    const char* xshared_library::get_error()
    {
        return dlerror();
    }

    xshared_library* xshared_library::open(const std::string& name)
    {
        void* handle = dlopen(name.c_str(), RTLD_NOW | RTLD_GLOBAL);
        return handle ? new xunix_shared_library(handle) : nullptr;
    }
}

#endif
