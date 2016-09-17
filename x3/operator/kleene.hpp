/*=============================================================================
    Copyright (c) 2001-2014 Joel de Guzman
    Copyright (c) 2001-2011 Hartmut Kaiser

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#pragma once

#include <x3/core/parser.hpp>
#include <x3/support/traits/container_traits.hpp>
#include <x3/support/traits/attribute_of.hpp>
#include <x3/core/detail/parse_into_container.hpp>

namespace x3
{
    template <typename Subject>
    struct kleene : unary_parser<Subject, kleene<Subject>>
    {
        typedef unary_parser<Subject, kleene<Subject>> base_type;
        static bool const handles_container = true;

        kleene(Subject const& subject)
          : base_type(subject) {}

        template <typename Iterator, typename Context
          , typename RContext, typename Attribute>
        bool parse(Iterator& first, Iterator const& last
          , Context const& context, RContext& rcontext, Attribute& attr) const
        {
            while (detail::parse_into_container(
                this->subject, first, last, context, rcontext, attr))
                ;
            return true;
        }
    };

    template <typename Subject>
    inline kleene<typename extension::as_parser<Subject>::value_type>
    operator*(Subject const& subject)
    {
        return { as_parser(subject) };
    }
}

namespace x3 { namespace traits
{
    template <typename Subject, typename Context>
    struct attribute_of<x3::kleene<Subject>, Context>
        : build_container<
            typename attribute_of<Subject, Context>::type> {};
}}
