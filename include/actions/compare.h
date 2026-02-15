
#pragma once

#include <ostream>
#include <variant>
#include <compare>

#include "utils.h"
#include "action_wraper.h"
#include "set.h"

namespace actions {

/* action compare for container */
template<typename K, typename V>
struct compare {
    std::variant<std::tuple<K, K>, std::tuple<K, V>, std::tuple<V, K>> indexs;

//    using IncludeToWrap = include_actions<actions::set>;
//    using IncludeToHistory = include_actions<actions::set>;
};

template<typename ContainerType>
struct action_type<compare, ContainerType> {
    using K = ContainerType::key_type;
    using V = ContainerType::value_type;
    using Action = actions::compare<K, V>;

    static std::strong_ordering apply_action(const Action& c, ContainerType& container) {
        return std::visit(overloaded {
            [&](const std::tuple<K, K>& p) -> std::strong_ordering {
                return container[std::get<0>(p)] <=> container[std::get<1>(p)];
            },
            [&](const std::tuple<K, V>& p) -> std::strong_ordering {
                return container[std::get<0>(p)] <=> std::get<1>(p);
            },
            [&](const std::tuple<V, K>& p) -> std::strong_ordering {
                return std::get<0>(p) <=> container[std::get<1>(p)];
            }
        }, c.indexs);
    }
     
    template<typename ContainerWrapperType>
    struct name_method_wrapper {
        [[nodiscard]] std::strong_ordering compare(K i, K j) const requires (!std::same_as<K, V>) {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i, j);
        }
        [[nodiscard]] std::strong_ordering compare(K i, V j) const requires (!std::same_as<K, V>) {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i, j);
        }
        [[nodiscard]] std::strong_ordering compare(V i, K j) const requires (!std::same_as<K, V>) {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i, j);
        }

        [[nodiscard]] std::strong_ordering compare(Key<K> i, Key<K> j) const {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i.value, j.value);
        }
        [[nodiscard]] std::strong_ordering compare(Key<K> i, Value<V> j) const {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i.value, j.value);
        }
        [[nodiscard]] std::strong_ordering compare(Value<V> i, Key<K> j) const {
            return method_wrapper<const actions::compare<K, V>, const ContainerWrapperType>(this, i.value, j.value);
        }
    };
};

template<typename K, typename V>
std::ostream& operator<<(std::ostream& out, const compare<K, V>& c) {
    out << "compare ";
    std::visit(overloaded {
        [&](const std::tuple<K, K>& p) {
            out << "key " << std::get<0>(p) << " key " << std::get<1>(p);
        },
        [&](const std::tuple<K, V>& p) {
            out << "key " << std::get<0>(p) << " value " << std::get<1>(p);
        },
        [&](const std::tuple<V, K>& p) {
            out << "value " << std::get<0>(p) << " key " << std::get<1>(p);
        }
    }, c.indexs);
    
    return out;
}

}; // namespace actions
