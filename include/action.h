
#pragma once

#include <utility>
#include <type_traits>

namespace help {
    template<typename ... Types> struct pack {};
    
    template<typename Type> struct is_pack : std::false_type {};
    template<typename... Types> struct is_pack<pack<Types...>> : std::true_type {};
    template<typename... Types>
    constexpr bool is_pack_v = is_pack<Types...>::value;
    
    template<template<typename... _> typename... Types> struct tpack {};
    
    template<typename Type> struct is_tpack : std::false_type {};
    template<template<typename... _> typename... Types> struct is_tpack<tpack<Types...>> : std::true_type {};
    template<typename Type>
    constexpr bool is_tpack_v = is_tpack<Type>::value;

    template<typename... Types>
    struct join {
        using Type = pack<Types...>;
    };
    template<typename... Types>
    struct join<pack<Types...>> {
        using Type = pack<Types...>;
    };
    template<typename... Types1, typename... Types2, typename... Types3>
    struct join<pack<Types1...>, pack<Types2...>, Types3...> {
        using Type = join<pack<Types1..., Types2...>, Types3...>::Type;
    };
    
    template<typename... Types>
    struct tjoin {
        using Type = join<Types...>;
    };
    template<template<typename... _> typename... Types>
    struct tjoin<tpack<Types...>> {
        using Type = tpack<Types...>;
    };
    template<template<typename... _> typename... Types1, template<typename... _> typename... Types2, typename... Types3>
    struct tjoin<tpack<Types1...>, tpack<Types2...>, Types3...> {
        using Type = tjoin<tpack<Types1..., Types2...>, Types3...>::Type;
    };


    template<typename Pack, template<typename... _> typename To, typename... Types>
    struct unpack {};
    template<typename... Types, template<typename... _> typename To, typename... Args>
    struct unpack<pack<Types...>, To, Args...> {
        using Type = To<Args..., Types...>;
    };

    template <typename T, typename... Types>
    struct is_contains : std::disjunction<std::is_same<T, Types>...> {};
    template <typename T, typename... Types>
    struct is_contains<T, pack<Types...>> : is_contains<T, Types...> {};
    template<typename T, typename... Types>
    constexpr bool is_contains_v = is_contains<T, Types...>::value;

    // template <template<typename... _> typename T, template<typename... _> typename... Types>
    // struct is_tcontains : std::disjunction<std::is_same<T, Types>...> {};
    // template <template<typename... _> typename T, template<typename... _> typename... Types>
    // struct is_tcontains<T, tpack<Types...>> : std::disjunction<std::is_same<T, Types>...> {};
    // template <template<typename... _> typename T, template<typename... _> typename... Types>
    // constexpr bool is_tcontains_v = is_contains<T, Types...>::value;

    template<typename In, typename Out>
    struct unique_impl;
    template<typename... Out>
    struct unique_impl<pack<>, pack<Out...>> {
        using Type = pack<Out...>;
    };
    template<typename T, typename... In, typename... Out>
    struct unique_impl<pack<T, In...>, pack<Out...>> {
        using Type = typename std::conditional_t<
            is_contains<T, Out...>::value,
            unique_impl<pack<In...>, pack<Out...>>,
            unique_impl<pack<In...>, pack<Out..., T>>
        >::Type;
    };
    template<typename... Types>
    struct remove_doubles {
        using Type = unique_impl<pack<Types...>, pack<>>::Type;
    };

    template<typename T>
    struct decompose;
    template<template <typename... _> typename Template, typename... Args>
    struct decompose<Template<Args...>> {
        template <typename... Types> 
        using TemplateType = Template<Types...>; 
        using ArgsType = pack<Args...>;
    };
}; // namespace help

namespace actions {

/*
 * REQUIRED FOR ACTION
 */

/* 
 * Define action actually type, because it depends only from container type 
 * this is must be for registrate in histoy
 * Action type must be inside
 * Main structure for action
 */
template<
    template<typename... _> typename Action,
    typename Container
>
struct action_type {};

/* Define usage action to container */
template<typename ContainerType, template<typename... _> typename Action, typename... Args>
void apply_action(Action<Args...>& _a, ContainerType& _c) {
    // std::cout << "void action" << std::endl;
    if constexpr (requires () { action_type<Action, std::decay_t<ContainerType>>::apply_action(_a, _c); }) {
        action_type<Action, std::decay_t<ContainerType>>::apply_action(_a, _c);
    } else {
        std::cout << "void action" << std::endl;
    }
}

/*
 * OPTINAL FOR ACTION
 */

/*
 * Using type help::pack type must be inside
 * types in help::pack will be add to history
 * it mean that applying this actions will be saved
 */
template<template<typename... _> typename... Actions>
using include_actions = help::tpack<Actions...>;

/* Include to history */

template<typename Action, typename = void>
struct get_action_include_to_wrap {
    using Type = help::tpack<>;
};

template<typename Action>
struct get_action_include_to_wrap<Action, std::void_t<typename Action::IncludeToWrap>> {
    using Type = std::conditional_t<
        help::is_tpack_v<typename Action::IncludeToWrap>,
        typename Action::IncludeToWrap,
        help::tpack<>
    >;
};

template<template<typename... _> typename Action, typename Container, typename = void>
struct get_action_include_to_wrap2 {
    using Type = help::tpack<>;
};

template<template<typename... _> typename  Action, typename Container>
struct get_action_include_to_wrap2<Action, Container, std::void_t<typename action_type<Action, Container>::IncludeToWrap>> {
    using Type = std::conditional_t<
        help::is_tpack_v<typename action_type<Action, Container>::IncludeToWrap>,
        typename action_type<Action, Container>::IncludeToWrap,
        help::tpack<>
    >;
};

/* Include to history */

template<template<typename... _> typename Action, typename Container, typename = void>
struct get_action_include_to_history {
    using Type = help::tpack<>;
};

template<template<typename... _> typename  Action, typename Container>
struct get_action_include_to_history<Action, Container, std::void_t<typename action_type<Action, Container>::IncludeToHistory>> {
    using Type = std::conditional_t<
        help::is_tpack_v<typename action_type<Action, Container>::IncludeToHistory>,
        typename action_type<Action, Container>::IncludeToHistory,
        help::tpack<>
    >;
};

/*
 * HELP FUNCTION FOR ACTION
 */

/* Wrapper for method in container wrapper 
 * for simple methods
 */
template<typename Action, typename Container, typename History, typename... Args>
decltype(auto) method_wrapper_impl(Container& container, History& history, Args&&... args) {
    Action action = Action(args...);
    history.add(action);
    return apply_action(action, container);
}
/* Help for get container from action wrapper */
template<typename ContainerWrapperType, typename NameMethodWrapperImplType>
decltype(auto) get_container(NameMethodWrapperImplType* ptr) {
    return static_cast<ContainerWrapperType*>(ptr)->get_container();
}
/* Help for get history from action wrapper */
template<typename ContainerWrapperType, typename NameMethodWrapperImplType>
decltype(auto) get_history(NameMethodWrapperImplType* ptr) {
    return static_cast<ContainerWrapperType*>(ptr)->get_history();
}
/* Method to call action from action wrapper, use cast to parent */
template<
    typename Action, 
    // parent type
    typename ContainerWrapperType,
    // pointer this type, need to static cast
    typename NameMethodWrapperImplType,
    typename... Args
>
decltype(auto) method_wrapper(NameMethodWrapperImplType* nmw, Args&&... args) {
    return method_wrapper_impl<Action>(
        get_container<ContainerWrapperType>(nmw),
        get_history<ContainerWrapperType>(nmw),
        args...
    );
}

/* Method to call action from action wrapper, use cast to parent */
template<
    template<typename... _> typename Action, 
    // parent type
    typename ContainerWrapperType,
    // pointer this type, need to static cast
    typename NameMethodWrapperImplType,
    typename... Args
>
decltype(auto) method_wrapper(NameMethodWrapperImplType* nmw, Args&&... args) {
    using Container = ContainerWrapperType::Container;
    using ActionType = action_type<Action, Container>::Action;
    return method_wrapper_impl<ActionType>(
        get_container<ContainerWrapperType>(nmw),
        get_history<ContainerWrapperType>(nmw),
        args...
    );
}

/* Help for get action wrapper */
template<typename ContainerWrapperType, typename NameMethodWrapperImplType>
decltype(auto) get_container_wrapper(NameMethodWrapperImplType* ptr) {
    return *static_cast<ContainerWrapperType*>(ptr);
}

}; // namespace actions
