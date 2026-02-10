
template<typename K>
struct compare;

#pragma once

#include <ostream>

#include "action_wraper.h"
#include "set.h"

/* action compare for container */
template<typename K>
struct compare {
    K index1;
    K index2;

    using IncludeToWrap = include_actions<set>;
    using IncludeToHistory = include_actions<set>;
};

template<typename ContainerType>
struct action_type<compare, ContainerType> {
    using K = ContainerType::key_type;
    using Action = ::compare<K>;
};

template<typename ContainerType>
std::strong_ordering apply_action(const typename action_type<compare, ContainerType>::Action& c, ContainerType& container) {
    if (container[c.index1] == container[c.index2]) {
        return std::strong_ordering::equal;
    }
    if (container[c.index1] <= container[c.index2]) {
        return std::strong_ordering::less;
    }
    return std::strong_ordering::greater;
}

template<typename K, typename ContainerWrapperType>
struct name_method_wrapper<compare<K>, ContainerWrapperType> {
    [[nodiscard]] std::strong_ordering compare(K i, K j) const {
        return method_wrapper<::compare<K>, const ContainerWrapperType>(this, i, j);
    }
};
template<typename K>
std::ostream& operator<<(std::ostream& out, const compare<K>& c) {
    out << "compare: " << c.index1 << ", " << c.index2;
    return out;
}
