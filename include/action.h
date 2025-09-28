
#pragma once

#include <ostream>

#include "action_wraper.h"

/* action set for container */
template<typename U>
struct set {
    using T = U::value_type;

    size_t index;
    std::optional<T> old_data;
    T new_data;

    set(size_t index, T new_data) : index(index), old_data(std::nullopt), new_data(new_data) {};

    void operator()(U& container) {
        old_data = container[index];
        container[index] = new_data;
    }
};
template<typename ActionWrapper, typename ContainerType>
struct name_method_wrapper<set, ActionWrapper, ContainerType> {
    void set(size_t i, typename ContainerType::value_type new_data) {
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
    size_t index1;
    size_t index2;
    
    compare(size_t index1, size_t index2) : index1(index1), index2(index2) {};
    
    std::strong_ordering operator()(U& container) const {
        return container[index1] <= container[index2] ? 
            (container[index1] == container[index2] ? 
                std::strong_ordering::equal : std::strong_ordering::less)
             : std::strong_ordering::greater;
    }
};
template<typename ActionWrapper, typename ContainerType>
struct name_method_wrapper<compare, ActionWrapper, ContainerType> {
    std::strong_ordering compare(size_t i, size_t j) {
        return method_wrapper<ActionWrapper, ::compare>(this, i, j);
    }
};
template<typename U>
std::ostream& operator<<(std::ostream& out, const compare<U>& c) {
    out << "compare: " << c.index1 << ", " << c.index2;
    return out;
}

