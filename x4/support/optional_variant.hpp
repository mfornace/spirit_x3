#pragma once

#include <boost/hana/prepend.hpp>
#include <boost/hana/plus.hpp>
#include <boost/hana/equal.hpp>
#include <boost/hana/minus.hpp>

#include "variant.hpp"
#include <utility>

namespace x4 {

struct blank {
    auto & value() {return *this;}
    auto const & value() const {return *this;}
};

/******************************************************************************************/

template <bool Recursive, class ...Types>
class optional_variant : public variant<Recursive, blank, Types...> {
    using base = variant<Recursive, blank, Types...>;

    struct at {
        template <class I, class Var, class F, class ...Ts>
        decltype(auto) operator()(I i, Var &&self, F &&f, Ts &&...ts) const {
            return std::forward<F>(f)(i-1_c, self[i-1_c], std::forward<Ts>(ts)...);
        }
    };

public:
    optional_variant() : base(0_c) {}

    constexpr operator bool() const {return base::index();}
    auto index() const {return base::index() ? base::index() - 1_c : throw variant_error<hana::int_<0>>(0);}

    template <class I> decltype(auto) operator[](I i) {return base::operator[](i + 1_c);}
    template <class I> decltype(auto) operator[](I i) const {return base::operator[](i + 1_c);}

    template <class I, class ...Ts>
    void emplace(I i, Ts &&...ts) {base::emplace(i + 1_c, std::forward<Ts>(ts)...);}

    template <class ...Ts>
    decltype(auto) visit(Ts &&...ts) {
        assert(bool(*this));
        return base::fold(1_c, base::index(), at(), *this, std::forward<Ts>(ts)...);
    }

    template <class ...Ts>
    decltype(auto) visit(Ts &&...ts) const {
        assert(bool(*this));
        return base::fold(1_c, base::index(), at(), *this, std::forward<Ts>(ts)...);}
};

template <class ...Types> using recursive_optional_variant = optional_variant<true, Types...>;
template <class ...Types> using nonrecursive_optional_variant = optional_variant<false, Types...>;

template <bool B=false> static constexpr auto optional_variant_c = hana::fuse(hana::template_<recursive_optional_variant>);
template <> static constexpr auto optional_variant_c<> = hana::fuse(hana::template_<nonrecursive_optional_variant>);

/******************************************************************************************/

}