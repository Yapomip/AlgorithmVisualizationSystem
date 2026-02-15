
#pragma once

#include <ostream>
#include <optional>

#include "utils.h"
#include "action.h"
#include "action_wraper.h"

#include "set.h"
#include "compare.h"
#include "add_mul_sub_div.h"

namespace help {

template <typename T>
struct arithmetic {
    friend T operator+(T lhs, const T& rhs) {
        lhs += rhs;
        return lhs;
    }
    friend T operator*(T lhs, const T& rhs) {
        lhs *= rhs;
        return lhs;
    }
    friend T operator-(T lhs, const T& rhs) {
        lhs -= rhs;
        return lhs;
    }
    friend T operator/(T lhs, const T& rhs) {
        lhs /= rhs;
        return lhs;
    }
};

}; // namespace help

namespace actions {

/* action set for container */
template<typename K, typename V>
struct element {
    K key;
};

template<typename K, typename V, typename CW>
struct element_impl {//: help::arithmetic<element_impl<K, V, CW>> {
    K key;
    mutable CW* cw; // container_wrapper

    element_impl(K k, CW* c) : key(k), cw(c) {}

    /* set */
    element_impl& operator=(const element_impl& e) {
        if (cw != e.cw) {
            std::cout << "ERROR different cw for element action operator=: " << cw << " " << e.cw << std::endl;
        }
        method_wrapper<actions::set<K, V>, CW>(cw, key, Key<K>{e.key});
        return *this;
    }
    element_impl& operator=(V&& new_value) {
        method_wrapper<set<K, V>, CW>(cw, key, Value<V>{new_value});
        return *this;
    }
    /* compare */
    auto operator<=>(const element_impl& e) const {
        if (cw != e.cw) {
            std::cout << "ERROR different cw for element action operator<=>: " << cw << " " << e.cw << std::endl;
        }
        return method_wrapper<compare<K, V>, CW>(cw, Key{key}, Key{e.key});
    }
    auto operator<=>(const V& v) const {
        return method_wrapper<compare<K, V>, CW>(cw, Key{key}, Value{v});
    }
    friend auto operator<=>(const V& v, const element_impl& e) {
        return method_wrapper<compare<K, V>, CW>(e.cw, Value{v}, Key{e.key});
    }

    /* add/mul/sub/div */
    element_impl& operator+=(const element_impl& e) {
        method_wrapper<add<K, V>, CW>(cw, key, Key{e.key});
        return *this;
    }
    element_impl& operator+=(const V& v) {
        method_wrapper<add<K, V>, CW>(cw, key, Value{v});
        return *this;
    }
    element_impl& operator*=(const element_impl& e) {
        method_wrapper<mul<K, V>, CW>(cw, key, Key{e.key});
        return *this;
    }
    element_impl& operator*=(const V& v) {
        method_wrapper<mul<K, V>, CW>(cw, key, Value{v});
        return *this;
    }
    element_impl& operator-=(const element_impl& e) {
        method_wrapper<sub<K, V>, CW>(cw, key, Key{e.key});
        return *this;
    }
    element_impl& operator-=(const V& v) {
        method_wrapper<sub<K, V>, CW>(cw, key, Value{v});
        return *this;
    }
    element_impl& operator/=(const element_impl& e) {
        method_wrapper<div<K, V>, CW>(cw, key, Key{e.key});
        return *this;
    }
    element_impl& operator/=(const V& v) {
        method_wrapper<div<K, V>, CW>(cw, key, Value{v});
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& out, const element_impl& e) {
        out << e.cw->get_container()[e.key];
        return out;
    }
};

template<typename ContainerType>
struct action_type<element, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = element<K, V>;

    // using IncludeToWrap = include_actions<set, compare, add, mul, sub, div>;
    using IncludeToHistory = include_actions<set, compare, add, mul, sub, div>;
    
    static decltype(auto) apply_action(Action& a, ContainerType& container) {
        return a.key;
    }
    // This will be in container, CRTP
    template<typename ContainerWrapperType>
    struct name_method_wrapper {
        element_impl<K, V, ContainerWrapperType> operator[](K key) {
            element_impl<K, V, ContainerWrapperType> e_impl{key, &get_container_wrapper<ContainerWrapperType>(this)};
            method_wrapper<actions::element<K, V>, ContainerWrapperType>(this, key);
            return e_impl;
        }
        const element_impl<K, V, ContainerWrapperType> operator[](K key) const {
            element_impl<K, V, ContainerWrapperType> e_impl{key, &get_container_wrapper<ContainerWrapperType>(this)};
            method_wrapper<actions::element<K, V>, ContainerWrapperType>(this, key);
            return e_impl;
        }
    };
};


template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const element<K, V>& e) {
    out << "element: " << e.key;
    return out;
}

}; // namespace actions
