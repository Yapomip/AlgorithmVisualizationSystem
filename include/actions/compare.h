
#pragma once

#include <ostream>

#include "action_wraper.h"

/* action compare for container */
template<typename K>
struct compare {
    K index1;
    K index2;
    
    compare(K index1, K index2) : index1(index1), index2(index2) {};
    
    template<typename U>
    std::strong_ordering operator()(const U& container) const {
        return container[index1] <= container[index2] ? 
            (container[index1] == container[index2] ? 
                std::strong_ordering::equal : std::strong_ordering::less)
             : std::strong_ordering::greater;
    }
};

template<typename ContainerType>
struct action_type<compare, ContainerType> {
    using K = ContainerType::key_type;
    using Action = ::compare<K>;
};

template<typename ContainerType, typename ActionWrapper>
struct name_method_wrapper<compare, ContainerType, ActionWrapper> {
    using type = action_type<compare, ContainerType>;
    using K = typename type::K;
    using Action = typename type::Action;

    std::strong_ordering compare(K i, K j) const {
        return method_wrapper<Action, const ActionWrapper>(this, i, j);
    }
};
template<typename K>
std::ostream& operator<<(std::ostream& out, const compare<K>& c) {
    out << "compare: " << c.index1 << ", " << c.index2;
    return out;
}
