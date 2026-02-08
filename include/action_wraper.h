
#pragma once

/* wrapper for method in container wrapper */
template<typename Action, typename Container, typename History, typename... Args>
decltype(auto) method_wrapper_impl(Container& container, History& history, Args&&... args) {
    Action action = Action(args...);
    history.add(action);
    return action(container);
}

template<typename ActionWrapper, typename From>
ActionWrapper::Container& get_container(From* ptr) {
    return static_cast<ActionWrapper*>(ptr)->get_container();
}
template<typename ActionWrapper, typename From>
ActionWrapper::History& get_history(From* ptr) {
    return static_cast<ActionWrapper*>(ptr)->get_history();
}
template<
    typename ActionWrapper, 
    template<typename C> typename Action, 
    typename NameMethodWrapper,
    typename... Args
>
decltype(auto) method_wrapper(NameMethodWrapper* nmw, Args&&... args) {
    return method_wrapper_impl<Action<typename ActionWrapper::Container>>(
        get_container<ActionWrapper>(nmw),
        get_history<ActionWrapper>(nmw),
        args...
    );
}

/* wrapper for method name in container wrapper */
template<template<typename C> typename Action, typename ActionWrapper, typename ContainerType>
struct name_method_wrapper {};

template<
    typename ContainerWrapperType, 
    typename ContainerType, 
    typename HistoryType, 
    template<typename ActionContainer> typename... Actions>
struct action_wrapper : 
    name_method_wrapper<
        Actions, 
        action_wrapper<ContainerWrapperType, ContainerType, HistoryType, Actions...>, 
        ContainerType
    >...
{
    using ContainerWrapper = ContainerWrapperType;
    using Container = ContainerType;
    using History = HistoryType;

    History& get_history() {
        return static_cast<ContainerWrapper*>(this)->get_history();
    }
    Container& get_container() {
        return static_cast<ContainerWrapper*>(this)->get_container();
    }
    /* TODO action */
    size_t size() {
        return get_container().size();
    }
    // this method in wrap
    /*
    void set(size_t i, T new_data) { 
        // static_assert();
        ::set s = ::set(i, new_data);
        history.add(s);
        s(container);
    }
    */
};
