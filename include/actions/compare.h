
template<typename K, typename V>
struct compare;

#pragma once

#include <ostream>
#include <variant>
#include <compare>

#include "utils.h"
#include "action_wraper.h"
#include "set.h"

/* action compare for container */
template<typename K, typename V>
struct compare {
    std::variant<K, V> index1;
    std::variant<K, V> index2;

    using IncludeToWrap = include_actions<set>;
    using IncludeToHistory = include_actions<set>;
};

template<typename ContainerType>
struct action_type<compare, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = ::compare<K, V>;
};

template<typename ContainerType>
std::strong_ordering apply_action(const typename action_type<compare, ContainerType>::Action& c, ContainerType& container) {
    using K = action_type<compare, ContainerType>::K;
    using V = action_type<compare, ContainerType>::V;
    return std::visit(overloaded {
        [&](const K& key1, const K& key2) -> std::strong_ordering {
            return container[key1] <=> container[key2];
        },
        [&](const K& key1, const V& value2) -> std::strong_ordering {
            return container[key1] <=> value2;
        },
        [&](const V& value1, const K& key2) -> std::strong_ordering {
            return value1 <=> container[key2];
        },
        [&](const V& value1, const V& value2) -> std::strong_ordering {
            std::cout << "ERORR compare on two values";
            return value1 <=> value2;
        },
    }, c.index1, c.index2);
    
    // return container[c.index1] <=> container[c.index2];
}

template<typename K, typename V, typename ContainerWrapperType>
struct name_method_wrapper<compare<K, V>, ContainerWrapperType> {
    [[nodiscard]] std::strong_ordering compare(K i, K j) const {
        return method_wrapper<::compare<K, V>, const ContainerWrapperType>(this, i, j);
    }
};
template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const compare<K, V>& c) {
    out << "compare ";
    std::visit(overloaded {
        [&](const K& key1, const K& key2) {
            out << "key " << key1 << " key " << key2;
        },
        [&](const K& key1, const V& value2) {
            out << "key " << key1 << " value " << value2;
        },
        [&](const V& value1, const K& key2) {
            out << "value " << value1 << " key " << key2;
        },
        [&](const V& value1, const V& value2) {
            std::cout << "ERORR compare on two values";
            out << "value " << value1 << " value " << value2;
        },
    }, c.index1, c.index2);
    
    return out;
}
