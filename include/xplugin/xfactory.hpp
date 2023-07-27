#pragma once

#include <memory>

namespace xp
{

    template<class BASE_CLASE, class... ARGS>
    class xfactory_base
    {
        public:
            using instance_type = BASE_CLASE;
            virtual ~xfactory_base() {}
            virtual std::unique_ptr<instance_type>  create(ARGS...) = 0;
    };


    // default implementation of the factory
    template<class INSTANCE,class BASE_CLASS, class... ARGS>
    class xfactory : public xfactory_base<BASE_CLASS, ARGS...>
    {
        public:
            using instance_type = INSTANCE;
            using base_class_type = BASE_CLASS;

            virtual ~xfactory() {}
            std::unique_ptr<base_class_type> create(ARGS... args) override
            {
                return std::make_unique<instance_type>(args...);
            }

    };

}