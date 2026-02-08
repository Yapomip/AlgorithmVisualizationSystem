
#pragma once

#include "action.h"

template<typename T>
concept ActionConcept = requires(T a) {
    { a() } -> std::same_as<void>;
};

template<
    // type of previous level attachment
    typename ContainerWrapperType, 
    typename ContainerType, 
    typename HistoryType, 
    template<typename... _> typename... Actions
>
struct action_wrapper : 
    name_method_wrapper<
        Actions, 
        ContainerType,
        action_wrapper<ContainerWrapperType, ContainerType, HistoryType, Actions...>
    >...
{
    using ContainerWrapper = ContainerWrapperType;
    using Container = ContainerType;
    using History = HistoryType;

    History& get_history() { return static_cast<ContainerWrapper*>(this)->get_history(); }
    Container& get_container() { return static_cast<ContainerWrapper*>(this)->get_container(); }
    History& get_history() const { return static_cast<const ContainerWrapper*>(this)->get_history(); }
    const Container& get_container() const { return static_cast<const ContainerWrapper*>(this)->get_container(); }
    /* TODO action */
    size_t size() const {
        return get_container().size();
    }
};
