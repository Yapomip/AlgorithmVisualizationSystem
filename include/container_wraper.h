
#pragma once

#include <vector>

#include "history.h"
#include "action_wraper.h"
#include "action.h"

#include "set.h"
#include "compare.h"

template<typename ContainerType, template<typename... _> typename... Actions>
struct container_wrapper : 
    action_wrapper_with_include<
        ContainerType,
        container_wrapper<ContainerType, Actions...>,
        Actions...
    >
{
    using Container = ContainerType;
    using History = default_history<typename action_type<Actions, ContainerType>::Action...>;

    Container container;
    mutable History history;

    template<typename T>
    explicit container_wrapper(T t) : container(t), history({}) { std::cout << "container_wrapper create\n"; }
    container_wrapper(const container_wrapper& cw) : container(cw.container), history(cw.history) { std::cout << "container_wrapper copy\n"; }
    container_wrapper(container_wrapper&& cw) noexcept : container(std::move(cw.container)), history(std::move(cw.history)) { std::cout << "container_wrapper move\n"; }
    container_wrapper& operator=(const container_wrapper& cw) { std::cout << "container_wrapper operator=\n"; if (this == &cw) { return *this; } container = cw.container; history = cw.history; return *this; }
    container_wrapper& operator=(const container_wrapper&& cw) noexcept { std::cout << "container_wrapper operator= move\n"; if (this == &cw) { return *this; } container = std::move(cw.container); history = std::move(cw.history); return *this; }

    History& get_history() { return history; }
    // for user it must be const container but it need to modify his hystory
    History& get_history() const { return history; }
    Container& get_container() { return container; }
    const Container& get_container() const { return container; }

    [[nodiscard]] size_t size() const {
        return get_container().size();
    }
};

template<typename T>
struct vector_wrap : std::vector<T> {
    using key_type = size_t;
};
template<typename T>
using MassWrapper = container_wrapper<vector_wrap<T>, set>;

#include <map>

template<typename T>
struct map_wrap : std::map<size_t, T> {
    using key_type = size_t;
    using value_type = T;
};
template<typename T>
using MapWrapper = container_wrapper<map_wrap<T>, set>;


template<typename T>
void apply_action(typename action_type<set, map_wrap<T>>::Action& a, map_wrap<T>& container) {
    a.old_data = container[a.index];
    if (a.index == 6) {
        container[a.index] = 1000;
    } else {
        container[a.index] = a.new_data;
    }
}

// typename set<size_t, int>::IncludeToWrap gg = 0;
// std::map m = help::is_tpack_v<typename set<size_t, int>::IncludeToWrap>;
// get_action_include_to_wrap<set<size_t, int>>::Type bbb = 0;

// help::includer<get_action_include_to_wrap, map_wrap<int>>::template get_action_type_from_tpack<help::tpack<compare>>::Type arr = 0;
// help::join<help::pack<>, help::pack<compare<unsigned long>>>::Type ggb = 0;
// help::unite_action_with_include<map_wrap<int>, set> aa = 0;