
#pragma once

#include <ostream>
#include <variant>
#include <compare>

#include "utils.h"
#include "action_wraper.h"
#include "compare.h"


namespace actions {

/* action set for container */
template<typename K, typename V>
struct set {
    K index;
    std::variant<Key<K>, Value<V>> new_data;
    std::optional<V> old_data;

    set(K index, std::variant<Key<K>, Value<V>> new_data) : index(index), new_data(new_data), old_data(std::nullopt) {}
    set(K index, std::variant<Key<K>, Value<V>> new_data, std::optional<V> old_data) : index(index), new_data(new_data), old_data(old_data) {}

    friend std::ostream& operator<<(std::ostream& out, const set<K, V>& s) {
        out << "set: " << s.index << ", ";
        std::visit(overloaded {
            [&](const Key<K>& key) {
                out << "key " << key.value;
            },
            [&](const Value<V>& value) {
                out << "value " << value.value;
            }
        }, s.new_data);
    
        if (s.old_data) {
            out << ", " << *s.old_data;
        }
    
        return out;
    }
};

template<typename ContainerType>
struct action_type<set, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = set<K, V>;

    static void apply_action(Action& a, ContainerType& container) {
        a.old_data = container[a.index];
        std::visit(overloaded {
            [&](const Key<K>& key) {
                container[a.index] = container[key.value];
            },
            [&](const Value<V>& value) {
                container[a.index] = value.value;
            }
        }, a.new_data);
    }

    template<typename ContainerWrapperType>
    struct name_method_wrapper {
        void set(K i, Key<K> new_data) {
            return method_wrapper<actions::set<K, V>, ContainerWrapperType>(this, i, new_data);
        }
        void set(K i, Value<V> new_data) {
            return method_wrapper<actions::set<K, V>, ContainerWrapperType>(this, i, new_data);
        }

        void set(K i, K new_data) requires (!std::same_as<K, V>) {
            return method_wrapper<actions::set<K, V>, ContainerWrapperType>(this, i, Key<K>{new_data});
        }
        void set(K i, V new_data) requires (!std::same_as<K, V>) {
            return method_wrapper<actions::set<K, V>, ContainerWrapperType>(this, i, Value<V>{new_data});
        }
    };
};

}; // namespace actions
