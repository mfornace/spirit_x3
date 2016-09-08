/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <x3/support/traits/attribute_of.hpp>
#include <x3/core/parser.hpp>
#include <x3/operator/detail/alternative.hpp>

namespace x3 {
    template <typename Left, typename Right>
    struct alternative : binary_parser<Left, Right, alternative<Left, Right>> {
        typedef binary_parser<Left, Right, alternative<Left, Right>> base_type;

        alternative(Left const& left, Right const& right) : base_type(left, right) {}

        template <class Range>
        auto check(Range &r) {
            optional<data_type> ret;
            if (!check_set(ret, this->left)) check_set(ret, this->right);
            return ret;
        }

        value_type parse(data_type data) {
            if (data[0_c]) return this->left.parse(std::move(data[0_c]));
            else return this->left.parse(std::move(data[1_c]));
        }

        template <typename Iterator, typename Context, typename RContext>
        bool parse(Iterator& first, Iterator const& last , Context const& context, RContext& rcontext, unused_type) const {
            return this->left.parse(first, last, context, rcontext, unused)
               || this->right.parse(first, last, context, rcontext, unused);
        }

        template <typename Iterator, typename Context , typename RContext, typename Attribute>
        bool parse(Iterator& first, Iterator const& last , Context const& context, RContext& rcontext, Attribute& attr) const {
            return detail::parse_alternative(this->left, first, last, context, rcontext, attr)
               || detail::parse_alternative(this->right, first, last, context, rcontext, attr);
        }
    };

    template <typename Left, typename Right>
    inline alternative< typename extension::as_parser<Left>::value_type , typename extension::as_parser<Right>::value_type>
    operator|(Left const& left, Right const& right) {return { as_parser(left), as_parser(right) }; }
}

namespace x3 { namespace traits
{
    template <typename Left, typename Right, typename Context>
    struct attribute_of<x3::alternative<Left, Right>, Context>
        : x3::detail::attribute_of_alternative<Left, Right, Context> {};
}}
