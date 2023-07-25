#pragma once

#include <memory>

namespace xp
{

    template<class INSTANCE, class... ARGS>
    class xfactory_base
    {
        public:
            using instance_type = INSTANCE;
            virtual ~xfactory_base() {}
            virtual std::unique_ptr<instance_type>  create(ARGS...) = 0;
    };

}