
template<typename K, typename V>
struct element;

#pragma once

#include <ostream>
#include <optional>

#include "action.h"
#include "action_wraper.h"

#include "set.h"
#include "compare.h"


/* action set for container */
template<typename K, typename V>
struct element {
    K key;
};

template<typename K, typename V, typename CW>
struct element_impl {
    K key;
    mutable CW* cw; // container_wrapper

    element_impl& operator=(const element_impl& e) {
        if (cw != e.cw) {
            std::cout << "ERROR different cw for element action operator=: " << cw << " " << e.cw << std::endl;
        }
        method_wrapper<::set<K, V>, CW>(cw, key, Key<K>{e.key});
        return *this;
    }
    element_impl& operator=(V&& new_value) {
        method_wrapper<::set<K, V>, CW>(cw, key, Value<V>{new_value});
        return *this;
    }
    auto operator<=>(const element_impl& e) const {
        if (cw != e.cw) {
            std::cout << "ERROR different cw for element action operator<=>: " << cw << " " << e.cw << std::endl;
        }
        return method_wrapper<::compare<K, V>, CW>(cw, key, e.key);
    }
    auto operator<=>(const V& v) const {
        return method_wrapper<::compare<K, V>, CW>(cw, key, v);
    }
    friend auto operator<=>(const V& v, const element_impl& e) {
        return method_wrapper<::compare<K, V>, CW>(e.cw, v, e.key);
    }

};

template<typename ContainerType>
struct action_type<element, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = element<K, V>;

    using IncludeToWrap = include_actions<set, compare>;
    using IncludeToHistory = include_actions<set, compare>;
};

template<typename ContainerType>
decltype(auto) apply_action(action<element, ContainerType>& a, ContainerType& container) {
    return a.key;
}

// This will be in container, CRTP
template<typename ContainerWrapperType, typename K, typename V>
struct name_method_wrapper<element<K, V>, ContainerWrapperType> {
    // using V = decltype(get_container<ContainerWrapperType, name_method_wrapper<element<K>, ContainerWrapperType>>(nullptr))::value_type;
    // using V = ContainerWrapperType::Container::value_type;

    element_impl<K, V, ContainerWrapperType> operator[](K key) {
        element_impl<K, V, ContainerWrapperType> e_impl(key, &get_container_wrapper<ContainerWrapperType>(this));
        method_wrapper<::element<K, V>, ContainerWrapperType>(this, key);
        return e_impl;
    }
    const element_impl<K, V, ContainerWrapperType> operator[](K key) const {
        element_impl<K, V, ContainerWrapperType> e_impl(key, &get_container_wrapper<ContainerWrapperType>(this));
        method_wrapper<::element<K, V>, ContainerWrapperType>(this, key);
        return e_impl;
    }
};

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const element<K, V>& e) {
    out << "element: " << e.key;
    return out;
}
