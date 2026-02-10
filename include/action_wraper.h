
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
        template<typename... _> typename GetIncludies,
        typename Container
    >
    struct includer {
        template<typename Tpack>
        struct get_action_type_from_tpack {};
        template<>
        struct get_action_type_from_tpack<tpack<>> {
            using Type = pack<>;
        };
        template<template<typename... _> typename T, template<typename... _> typename... Actions>
        struct get_action_type_from_tpack<tpack<T, Actions...>> {
            using Type = typename join<
                typename get_action_type_from_tpack<
                    tpack<Actions...>
                >::Type, 
                pack<typename action_type<T, Container>::Action>
            >::Type;
        };

        template<typename In, typename Out>
        struct get_all_includes {};

        template<typename... Out>
        struct get_all_includes<pack<>, pack<Out...>> {
            using Type = pack<Out...>;
        };
        template<typename T, typename... In, typename... Out>
        requires requires() { typename GetIncludies<T>::Type; }
        struct get_all_includes<pack<T, In...>, pack<Out...>> {
            using Type = typename std::conditional_t<
                is_contains_v<T, Out...>,
                get_all_includes<pack<In...>, pack<Out...>>,
                get_all_includes<
                    typename join<
                        pack<In...>, 
                        typename get_action_type_from_tpack<
                            typename GetIncludies<T>::Type
                        >::Type
                    >::Type,
                    pack<Out..., T>
                >
            >::Type;
        };
    };
    
    // template<
    //     typename Container,
    //     template<typename... _> typename... Actions
    // >
    // using unite_action_with_include = remove_doubles<
    //     typename join<
    //         pack<typename action_type<Actions, Container>::Action...>, 
    //         // typename action_type<Actions, Container>::IncludeToWrap...
    //         typename get_action_include_wrap<Container, Actions>::Type...
    //     >::Type
    // >::Type;
    template<
        typename Container,
        template<typename... _> typename... Actions
    >
    using unite_action_with_include = typename includer<get_action_include_to_wrap, Container>::template 
        get_all_includes<
            pack<typename action_type<Actions, Container>::Action...>, 
            pack<>
        >::Type;

}; //namespace help


template<
    typename Container,
    typename ContainerWrapper,
    template<typename... _> typename... Actions
>
using action_wrapper_with_include = typename help::unpack<
    help::unite_action_with_include<Container, Actions...>,
    action_wrapper_impl, 
    ContainerWrapper
>::Type;
