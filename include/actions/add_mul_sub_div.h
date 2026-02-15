
template<typename K, typename V>
struct set;

#pragma once

#include <ostream>
#include <variant>
#include <compare>

#include "utils.h"
#include "action.h"
#include "set.h"

namespace actions {

/* action add for container */
#define action_struct_for_action_from_set(name) \
template<typename K, typename V>                                                     \
struct name : set<K, V> {                                                            \
    using set<K, V>::set;                                                            \
    friend std::ostream& operator<<(std::ostream& out, const name<K, V>& s) {        \
        out << #name << ": " << s.index << ", ";                                     \
        std::visit(overloaded {                                                      \
            [&](const Key<K>& key) {                                                 \
                out << "key " << key.value;                                          \
            },                                                                       \
            [&](const Value<V>& value) {                                             \
                out << "value " << value.value;                                      \
            }                                                                        \
        }, s.new_data);                                                              \
                                                                                     \
        return out;                                                                  \
    }                                                                                \
};

action_struct_for_action_from_set(add);
action_struct_for_action_from_set(mul);
action_struct_for_action_from_set(sub);
action_struct_for_action_from_set(div);


#define action_type_struct_for_action_from_set(name, one_operand_op) \
template<typename ContainerType>                                                                                \
struct action_type<name, ContainerType> {                                                                       \
    using K = ContainerType::key_type;                                                                          \
    using V = ContainerType::value_type;                                                                        \
    using Action = name<K, V>;                                                                                  \
                                                                                                                \
    static void apply_action(Action& a, ContainerType& container) {                                             \
        a.old_data = container[a.index];                                                                        \
        std::visit(overloaded {                                                                                 \
            [&](const Key<K>& key) {                                                                            \
                container[a.index] one_operand_op container[key.value];                                         \
            },                                                                                                  \
            [&](const Value<V>& value) {                                                                        \
                container[a.index] one_operand_op value.value;                                                  \
            }                                                                                                   \
        }, a.new_data);                                                                                         \
    }                                                                                                           \
                                                                                                                \
    template<typename ContainerWrapperType>                                                                     \
    struct name_method_wrapper {                                                                                \
        void name(K i, Key<K> new_data) {                                                                       \
            return method_wrapper<actions::name<K, V>, ContainerWrapperType>(this, i, new_data);                \
        }                                                                                                       \
        void name(K i, Value<V> new_data) {                                                                     \
            return method_wrapper<actions::name<K, V>, ContainerWrapperType>(this, i, new_data);                \
        }                                                                                                       \
                                                                                                                \
        void name(K i, K new_data) requires (!std::same_as<K, V>) {                                             \
            return method_wrapper<actions::name<K, V>, ContainerWrapperType>(this, i, Key<K>{new_data});        \
        }                                                                                                       \
        void name(K i, V new_data) requires (!std::same_as<K, V>) {                                             \
            return method_wrapper<actions::name<K, V>, ContainerWrapperType>(this, i, Value<V>{new_data});      \
        }                                                                                                       \
    };                                                                                                          \
};

action_type_struct_for_action_from_set(add, +=);
action_type_struct_for_action_from_set(mul, *=);
action_type_struct_for_action_from_set(sub, -=);
action_type_struct_for_action_from_set(div, /=);

} // namespace actions
