
#pragma once

#include <ostream>

#include "action_wraper.h"

/* action set for container */
template<typename U>
struct set {
    using K = U::key_type;
    using V = U::value_type;

    K index;
    std::optional<V> old_data;
    V new_data;

    set(K index, V new_data) : index(index), old_data(std::nullopt), new_data(new_data) {};

    void operator()(U& container) {
        old_data = container[index];
        container[index] = new_data;
    }
};
template<typename ActionWrapper, typename ContainerType>
struct name_method_wrapper<set, ActionWrapper, ContainerType> {
    using K = ::set<ContainerType>::K;
    using V = ::set<ContainerType>::V;
    void set(K i, V new_data) {
        return method_wrapper<ActionWrapper, ::set>(this, i, new_data);
    }
};
template<typename U>
std::ostream& operator<<(std::ostream& out, const set<U>& s) {
    out << "set: " << s.index << ", " << s.new_data;
    return out;
}

/* action compare for container */
template<typename U>
struct compare {
    using K = U::key_type;
    K index1;
    K index2;
    
    compare(K index1, K index2) : index1(index1), index2(index2) {};
    
    std::strong_ordering operator()(U& container) const {
        return container[index1] <= container[index2] ? 
            (container[index1] == container[index2] ? 
                std::strong_ordering::equal : std::strong_ordering::less)
             : std::strong_ordering::greater;
    }
};
template<typename ActionWrapper, typename ContainerType>
struct name_method_wrapper<compare, ActionWrapper, ContainerType> {
    using K = ContainerType::key_type;
    std::strong_ordering compare(K i, K j) {
        return method_wrapper<ActionWrapper, ::compare>(this, i, j);
    }
};
template<typename U>
std::ostream& operator<<(std::ostream& out, const compare<U>& c) {
    out << "compare: " << c.index1 << ", " << c.index2;
    return out;
}

