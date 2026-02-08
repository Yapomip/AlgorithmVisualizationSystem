
#pragma once

#include <vector>

#include "history.h"
#include "action_wraper.h"
#include "action.h"

#include "set.h"
#include "compare.h"

template<typename ContainerType, template<typename... _> typename... Actions>
struct container_wrapper : 
    action_wrapper<
        container_wrapper<ContainerType, Actions...>, 
        ContainerType,
        default_history<typename action_type<Actions, ContainerType>::Action...>, 
        Actions...
    >
{
    using Container = ContainerType;
    using History = default_history<typename action_type<Actions, ContainerType>::Action...>;

    Container container;
    mutable History history;

    template<typename T>
    container_wrapper(T t) : container(t), history({}) { std::cout << "container_wrapper create\n"; }
    container_wrapper(const container_wrapper& cw) : container(cw.container), history(cw.history) { std::cout << "container_wrapper copy\n"; }
    container_wrapper(container_wrapper&& cw) : container(std::move(cw.container)), history(std::move(cw.history)) { std::cout << "container_wrapper move\n"; }
    container_wrapper& operator=(const container_wrapper& cw) { std::cout << "container_wrapper operator=\n"; if (this == &cw) { return *this; } container = cw.container; history = cw.history; return *this; }
    container_wrapper& operator=(const container_wrapper&& cw) { std::cout << "container_wrapper operator= move\n"; if (this == &cw) { return *this; } container = std::move(cw.container); history = std::move(cw.history); return *this; }

    History& get_history() { return history; }
    // for user it must be const container but it need to modify his hystory
    History& get_history() const { return history; }
    Container& get_container() { return container; }
    const Container& get_container() const { return container; }
};

template<typename T>
struct vector_wrap : std::vector<T> {
    using key_type = size_t;
};
template<typename T>
using MassWrapper = container_wrapper<vector_wrap<T>, set, compare>;
