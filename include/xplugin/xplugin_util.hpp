/***************************************************************************
 * Copyright (c) 2018, Johan Mabille, Sylvain Corlay, Wolf Vollprecht,      *
 * Martin Renou and Dr. Thorsten Beier                                      *
 *                                                                          *
 * Distributed under the terms of the BSD 3-Clause License.                 *
 *                                                                          *
 * The full license is in the file LICENSE, distributed with this software. *
 ****************************************************************************/

#ifndef XPLUGIN_UTIL_HPP
#define XPLUGIN_UTIL_HPP

#include <type_traits>

namespace xp::util
{
    template <class T, class = void>
    struct has_begin : std::false_type
    {
    };

    template <class T>
    struct has_begin<T, std::void_t<decltype(std::declval<T>().begin())>> : std::true_type
    {
    };

    template <class T, class = void>
    struct has_end : std::false_type
    {
    };

    template <class T>
    struct has_end<T, std::void_t<decltype(std::declval<T>().end())>> : std::true_type
    {
    };

    template <class T>
    using is_range = std::conjunction<has_begin<T>, has_end<T>>;

    template <class T>
    constexpr bool is_range_v = is_range<T>::value;

    namespace detail
    {
        struct invalid_type {};
    }

    template <class R, class = void>
    struct range_value
    {
        using type = detail::invalid_type;
    };

    template <class R>
    struct range_value<R, std::void_t<std::decay_t<decltype(*(std::declval<R>().begin()))>>>
    {
        using type = typename R::value_type;
    };

    template <class R>
    using range_value_t = typename range_value<R>::type;

    template <class R, class T>
    struct is_range_of
        : std::conjunction<
            is_range<R>,
            std::is_convertible<range_value_t<R>, T>
        >
    {
    };

    template <class R, class T>
    constexpr bool is_range_of_v = is_range_of<R, T>::value;
}

#endif
