
#pragma once

#include <vector>

#include "action_wraper.h"
#include "history.h"

#include "action.h"

template<typename ContainerType, template<typename ActionContainerType> typename... Actions>
struct container_wrapper : 
    action_wrapper<
        container_wrapper<ContainerType, Actions...>, 
        ContainerType,
        default_history<Actions<ContainerType>...>, 
        Actions...
    >
{
    using Container = ContainerType;
    using History = default_history<Actions<ContainerType>...>;

    Container container;
    History history;

    template<typename T>
    container_wrapper(T t) : container(t), history({}) {}

    History& get_history() {
        return history;
    }
    Container& get_container() {
        return container;
    }
};

template<typename T>
using MassWrapper = container_wrapper<std::vector<T>, set, compare>;
