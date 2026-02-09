
#pragma once

/*
 * OPTINAL FOR ACTION
 */

/* Action type must be inside */
template<
    template<typename... _> typename Action,
    typename Container
>
struct include_action_to_history {};

template<
    template<typename... _> typename Action,
    typename Container
>
struct include_action_to_wrapper {};

/*
 * REQUIRED FOR ACTION
 */

/* Define action actually type, because it depends only from container type 
 * this is must be for registrate in histoy
 * Action type must be inside
 */
template<
    template<typename... _> typename Action,
    typename Container
>
struct action_type {};

template<template<typename... _> typename Action, typename ContainerType>
void apply_action(typename action_type<Action, ContainerType>::Action& _, ContainerType& __) {
    std::cout << "void action" << std::endl;
}

/* Wrapper for method name to be in container wrapper, CRTP use */
template<
    template<typename... _> typename Action,
    typename Container,
    // parent type
    typename ActionWrapper
>
struct name_method_wrapper {};

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
template<typename ActionWrapper, typename From>
decltype(auto) get_container(From* ptr) {
    return static_cast<ActionWrapper*>(ptr)->get_container();
}
/* Help for get history from action wrapper */
template<typename ActionWrapper, typename From>
decltype(auto) get_history(From* ptr) {
    return static_cast<ActionWrapper*>(ptr)->get_history();
}
/* Method to call action from action wrapper, use cast to parent */
template<
    typename Action, 
    // parent type
    typename ActionWrapper, 
    // pointer this type, can change to just T or void*
    typename NameMethodWrapperImpl,
    typename... Args
>
decltype(auto) method_wrapper(NameMethodWrapperImpl* nmw, Args&&... args) {
    return method_wrapper_impl<Action>(
        get_container<ActionWrapper>(nmw),
        get_history<ActionWrapper>(nmw),
        args...
    );
}

/* Help for get action wrapper */
template<typename ActionWrapper, typename From>
decltype(auto) get_action_wrapper(From* ptr) {
    return *static_cast<ActionWrapper*>(ptr);
}

