
#pragma once

#include <iostream>

template<typename... Types>
struct default_history {
    std::tuple<std::vector<Types>...> all;

    template<typename T>
    void add(T&& action) {
        std::get<std::vector<std::decay_t<T>>>(all).push_back(action);
        /*
        std::visit(
            [this](auto&& act) -> void {
                std::apply(
                    [&act](auto&& tuple_vector) -> void {
                        if constexpr (std::is_same_v<typename decltype(tuple_vector)::value_type, decltype(act)>) {
                            tuple_vector.push_back(act);
                        }
                    },
                    all
                );
            },
            action
        );
        */
    }
};
template<typename... Types>
std::ostream& operator<<(std::ostream& out, const default_history<Types...>& h) {
    std::apply(
        [&out](const std::vector<Types>&... vectors) -> void {
            auto f = [&out](auto&& vector) {
                out << "size: " << vector.  size() << std::endl;
                for (auto&& action: vector) {
                    out << action << std::endl;
                }
                out << std::endl;
            };
            (f(vectors), ...);
        },
        h.all
    );
    return out;
}
