
#pragma once

#include <ostream>
#include <optional>

#include "action_wraper.h"

#include "compare.h"

/* action set for container */
template<typename K, typename V>
struct set {
    K index;
    V new_data;
    std::optional<V> old_data;

    using IncludeToWrap = include_actions<compare>;
    using IncludeToHistory = include_actions<compare>;
};

template<typename ContainerType>
struct action_type<set, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = set<K, V>;
};

template<typename ContainerType>
void apply_action(action<set, ContainerType>& a, ContainerType& container) {
    a.old_data = container[a.index];
    container[a.index] = a.new_data;
}

// This will be in container, CRTP
template<typename ContainerWrapperType, typename K, typename V>
struct name_method_wrapper<set<K, V>, ContainerWrapperType> {
    void set(K i, V new_data) {
        return method_wrapper<::set<K, V>, ContainerWrapperType>(this, i, new_data);
    }
};

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const set<K, V>& s) {
    out << "set: " << s.index << ", " << s.new_data;
    return out;
}
