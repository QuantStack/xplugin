/***************************************************************************
* Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht and   *
* Martin Renou                                                             *
*                                                                          *
* Distributed under the terms of the BSD 3-Clause License.                 *
*                                                                          *
* The full license is in the file LICENSE, distributed with this software. *
****************************************************************************/

#include <string>

#include "xplugin_config.hpp"

namespace xp
{
    class XPLUGIN_API xshared_library
    {
    public:

        virtual ~xshared_library() {}

        xshared_library(const xshared_library&) = delete;
        xshared_library& operator=(const xshared_library&) = delete;

        xshared_library(xshared_library&&) = delete;
        xshared_library& operator=(xshared_library&&) = delete;

        static xshared_library* open(const std::string& name);
        static void close(xshared_library* library);
        static const char* get_error();

        template <class T>
        T find_symbol(const std::string& name);

    protected:

        xshared_library() {}

    private:

        virtual void* find_symbol(const std::string& name) = 0;
    };

    template <class T>
    inline T xshared_library::find_symbol(const std::string& name)
    {
        return reinterpret_cast<T>(name);
    }

    inline void xshared_library::close(xshared_library* library)
    {
        delete library;
    }
}

