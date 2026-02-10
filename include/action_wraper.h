
#pragma once

#include <cstddef>

#include "action.h"

// TODO(yapomip): CONCEPT
// template<typename T>
// concept ActionConcept = requires(T a) {
//     { a() } -> std::same_as<void>;
// };

template<
    typename ContainerWrapper,
    typename... Actions
>
struct action_wrapper_impl : 
    name_method_wrapper<
        Actions, 
        ContainerWrapper
    >...
{};

template<
    typename Container,
    typename ContainerWrapper,
    template<typename... _> typename... Actions
>
using action_wrapper = action_wrapper_impl<ContainerWrapper, typename action_type<Actions, Container>::Action...>;

namespace help {
    template<
        typename Container,
        template<typename... _> typename GetIncludies1,
        template<template<typename... __> typename _Action, typename... _> typename GetIncludies2
    >
    struct includer {
        template<typename In, typename Out>
        struct get_all_includes {};

        template<typename... Out>
        struct get_all_includes<tpack<>, pack<Out...>> {
            using Type = pack<Out...>;
        };

        template<template<typename... _> typename T, template<typename... _> typename... In, typename... Out>
        requires requires() { 
            typename GetIncludies1<typename action_type<T, Container>::Action>::Type; 
            typename GetIncludies2<T, Container>::Type; 
        }
        struct get_all_includes<tpack<T, In...>, pack<Out...>> {
            using Type = typename std::conditional_t<
                is_contains_v<typename action_type<T, Container>::Action, Out...>,
                get_all_includes<tpack<In...>, pack<Out...>>,
                get_all_includes<
                    typename tjoin<
                        tpack<In...>, 
                        typename GetIncludies1<typename action_type<T, Container>::Action>::Type,
                        typename GetIncludies2<T, Container>::Type
                    >::Type,
                    pack<Out..., typename action_type<T, Container>::Action>
                >
            >::Type;
        };
    };
    
    template<
        typename Container,
        template<typename... _> typename... Actions
    >
    using unite_action_with_include_to_wrap = typename includer<Container, get_action_include_to_wrap, get_action_include_to_wrap2>::template 
        get_all_includes<
            tpack<Actions...>, 
            pack<>
        >::Type;

}; //namespace help


template<
    typename Container,
    typename ContainerWrapper,
    template<typename... _> typename... Actions
>
using action_wrapper_with_include = typename help::unpack<
    help::unite_action_with_include_to_wrap<Container, Actions...>,
    action_wrapper_impl, 
    ContainerWrapper
>::Type;
