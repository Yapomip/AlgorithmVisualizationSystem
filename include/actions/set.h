
#pragma once

#include <ostream>

#include "action_wraper.h"

/* action set for container */
template<typename K, typename V>
struct set {
    K index;
    std::optional<V> old_data;
    V new_data;

    set(K index, V new_data) : index(index), old_data(std::nullopt), new_data(new_data) {};

    template<typename U>
    void operator()(U& container) {
        old_data = container[index];
        container[index] = new_data;
    }
};

template<typename ContainerType>
struct action_type<set, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = ::set<K, V>;
};

// This will be in container
template<typename ContainerType, typename ActionWrapper>
struct name_method_wrapper<set, ContainerType, ActionWrapper> {
    using type = action_type<set, ContainerType>;
    using K = typename type::K;
    using V = typename type::V;
    using Action = typename type::Action;

    void set(K i, V new_data) {
        return method_wrapper<Action, ActionWrapper>(this, i, new_data);
    }
};

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const set<K, V>& s) {
    out << "set: " << s.index << ", " << s.new_data;
    return out;
}
