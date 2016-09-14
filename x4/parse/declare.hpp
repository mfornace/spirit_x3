#pragma once
#include "parse.hpp"

namespace x4 {

/******************************************************************************************/

struct declaration : expression_base {constexpr declaration() {}};

template <class T>
struct declaration_t : expression_base {constexpr declaration_t() {}};

template <class T> static constexpr auto declaration_c = declaration_t<T>();

/******************************************************************************************/

void implement(...);

template <class P, class=void> struct has_adl_impl : std::false_type {};
template <class P> struct has_adl_impl<P, void_if<!(std::is_same<decltype(implement(std::declval<P const>())), void>::value)>> : std::true_type {};

template <class P, class=void> struct has_member_impl : std::false_type {};
template <class P> struct has_member_impl<P, void_if<!(std::is_same<decltype(std::declval<P const>().implement()), void>::value)>> : std::true_type {};

/******************************************************************************************/

template <class P>
struct implementation<P, void_if<has_adl_impl<P>::value && !has_member_impl<P>::value>> {
    template <class Window>
    constexpr auto check(P const &p, Window &w) const {return check_of(implement(p), w);}

    template <class Data>
    constexpr auto success(P const &p, Data const &data) const {return success_of(implement(p), data);}

    template <class ...Args>
    constexpr auto parse(P const &p, Args &&...args) const {return parse_of(implement(p), std::forward<Args>(args)...);}
};

/******************************************************************************************/

template <class P>
struct implementation<P, void_if<has_member_impl<P>::value>> {
    template <class Window>
    constexpr auto check(P const &p, Window &w) const {return check_of(p.implement(), w);}

    template <class Data>
    constexpr auto success(P const &p, Data const &data) const {return success_of(p.implement(), data);}

    template <class ...Args>
    constexpr auto parse(P const &p, Args &&...args) const {return parse_of(p.implement(), std::forward<Args>(args)...);}
};

/******************************************************************************************/

#define X4_DECLARE(NAME) static auto constexpr NAME = ::x4::declaration_c<class NAME>;

#define X4_DEFINE(NAME) \
template <class T> struct implementation; \
template <> struct implementation<std::decay_t<decltype(NAME)>> {template <bool=true> static constexpr bool value = false;}; \
template <bool B=true> constexpr auto implement(std::decay_t<decltype(NAME)>) {return implementation<std::decay_t<decltype(NAME)>>::value<B>;} \
template <> auto const implementation<std::decay_t<decltype(NAME)>>::value<>

}
