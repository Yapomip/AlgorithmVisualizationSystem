
#pragma once

#include <cstddef>

#include "action.h"

// TODO(yapomip): CONCEPT
// template<typename T>
// concept ActionConcept = requires(T a) {
//     { a() } -> std::same_as<void>;
// };

template<
    // type of previous level attachment
    typename ContainerType, 
    typename HistoryType, 
    typename ContainerWrapperType,
    template<typename... _> typename... Actions
>
struct action_wrapper : 
    name_method_wrapper<
        Actions, 
        ContainerType,
        action_wrapper<ContainerType, HistoryType, ContainerWrapperType, Actions...>
    >...
{
    using ContainerWrapper = ContainerWrapperType;
    using Container = ContainerType;
    using History = HistoryType;

    History& get_history() { return static_cast<ContainerWrapper*>(this)->get_history(); }
    Container& get_container() { return static_cast<ContainerWrapper*>(this)->get_container(); }
    [[nodiscard]] History& get_history() const { return static_cast<const ContainerWrapper*>(this)->get_history(); }
    [[nodiscard]] const Container& get_container() const { return static_cast<const ContainerWrapper*>(this)->get_container(); }
    /* TODO action */
    [[nodiscard]] size_t size() const {
        return get_container().size();
    }
};
