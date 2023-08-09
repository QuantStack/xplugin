/***************************************************************************
 * Copyright (c) 2023, Dr. Thorsten Beier                                   *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef XPLUGIN_XFACTORY_HPP
#define XPLUGIN_XFACTORY_HPP

#include <xplugin/xplugin_config.hpp>

#include <memory>

#define XPLUGIN_CREATE_XPLUGIN_FACTORY(FACTORY_TYPE)                                                                   \
    extern "C" XPLUGIN_API typename FACTORY_TYPE::factory_base_type *create_plugin_factory()                           \
    {                                                                                                                  \
        return new FACTORY_TYPE();                                                                                     \
    }

namespace xp
{

template <class BASE_TYPE, class... ARGS>
class xfactory_base
{
  public:
    using base_type = BASE_TYPE;
    virtual ~xfactory_base() = default;
    virtual std::unique_ptr<base_type> create(ARGS...) = 0;
};

// default implementation of the factory
template <class CONCRETE_TYPE, class BASE_TYPE, class... ARGS>
class xfactory : public xfactory_base<BASE_TYPE, ARGS...>
{
  public:
    using concrete_type = CONCRETE_TYPE;
    using base_type = BASE_TYPE;
    using factory_base_type = xfactory_base<BASE_TYPE, ARGS...>;
    virtual ~xfactory() = default;
    std::unique_ptr<base_type> create(ARGS... args) override;
};

template <class CONCRETE_TYPE, class BASE_TYPE, class... ARGS>
auto xfactory<CONCRETE_TYPE, BASE_TYPE, ARGS...>::create(ARGS... args) -> std::unique_ptr<base_type>
{
    return std::make_unique<concrete_type>(args...);
}

} // namespace xp

#endif // XPLUGIN_XFACTORY_HPP
