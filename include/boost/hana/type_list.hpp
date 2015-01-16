/*!
@file
Defines `boost::hana::TypeList`.

@copyright Louis Dionne 2014
Distributed under the Boost Software License, Version 1.0.
(See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_HANA_TYPE_LIST_HPP
#define BOOST_HANA_TYPE_LIST_HPP

#include <boost/hana/fwd/type_list.hpp>

#include <boost/hana/bool.hpp>
#include <boost/hana/type.hpp>

// instances
#include <boost/hana/comparable.hpp>
#include <boost/hana/foldable.hpp>
#include <boost/hana/iterable.hpp>
#include <boost/hana/list.hpp>


namespace boost { namespace hana {
    template <>
    struct equal_impl<TypeList, TypeList> {
        template <typename Xs, typename Ys>
        static constexpr auto apply(Xs, Ys)
        { return false_; }

        template <typename Xs>
        static constexpr auto apply(Xs, Xs)
        { return true_; }
    };

    template <>
    struct Foldable::instance<TypeList> : Foldable::unpack_mcd {
        //! @todo Fix the lost optimization caused by unpacking with `Type`s.
        template <typename ...Xs, typename F>
        static constexpr auto unpack_impl(detail::repr::type_list<Xs...>, F f)
        { return f(type<Xs>...); }

        template <typename Xs, typename F>
        static constexpr auto unpack_impl(Xs, F f)
        { return unpack_impl(typename Xs::storage{}, f); }
    };

    template <>
    struct head_impl<TypeList> {
        template <typename Xs>
        static constexpr auto apply(Xs)
        { return apply(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto apply(detail::repr::type_list<X, Xs...>)
        { return type<X>; }
    };

    template <>
    struct tail_impl<TypeList> {
        template <typename Xs>
        static constexpr auto apply(Xs)
        { return apply(typename Xs::storage{}); }

        template <typename X, typename ...Xs>
        static constexpr auto apply(detail::repr::type_list<X, Xs...>)
        { return type_list<Xs...>; }
    };

    template <>
    struct is_empty_impl<TypeList> {
        template <typename Xs>
        static constexpr auto apply(Xs)
        { return apply(typename Xs::storage{}); }

        template <typename ...Xs>
        static constexpr auto apply(detail::repr::type_list<Xs...>)
        { return bool_<sizeof...(Xs) == 0>; }
    };

    template <>
    struct List::instance<TypeList> : List::mcd<TypeList> {
        template <typename X, typename ...Xs>
        static constexpr auto cons_impl(X, detail::repr::type_list<Xs...>)
        { return type_list<typename X::type, Xs...>; }

        template <typename X, typename Xs>
        static constexpr auto cons_impl(X x, Xs xs)
        { return cons_impl(x, typename Xs::storage{}); }

        static constexpr auto nil_impl()
        { return type_list<>; }
    };
}} // end namespace boost::hana

#endif // !BOOST_HANA_TYPE_LIST_HPP
