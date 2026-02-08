
#pragma once

#include <iostream>
#include <utility>

namespace help {
    template <typename T, typename... Types>
    struct index_in {};

    template <typename T>
    struct index_in<T> {
        static constexpr std::size_t value = 1;
    };
    template <typename T, typename... Types>
    struct index_in<T, T, Types...> {
        static constexpr std::size_t value = 0;
    };

    template <typename T, typename U, typename... Types>
    struct index_in<T, U, Types...> {
        static constexpr std::size_t value = 1 + index_in<T, Types...>::value;
    };

    template <typename T, typename... Types>
    inline constexpr std::size_t index_in_v = index_in<T, Types...>::value;
};

template <typename... Types>
struct default_history
{
    // using StorageType = std::vector<Types>...;
    std::tuple<std::vector<Types>...> all;
    std::vector<std::pair<size_t, size_t>> order;

    default_history() : all({}), order({}) { std::cout << "default_history create\n"; }
    default_history(const default_history& h) : all(h.all), order(h.order) { std::cout << "default_history copy\n"; }
    default_history(default_history&& h) : all(std::move(h.all)), order(std::move(h.order)) { std::cout << "default_history move\n"; }
    default_history& operator=(const default_history& h) { std::cout << "default_history operator=\n"; if (this == &h) { return *this; } all = h.all; order = h.order; return *this; }
    default_history& operator=(const default_history&& h) { std::cout << "default_history operator= move\n"; if (this == &h) { return *this; } all = std::move(h.all); order = std::move(h.order); return *this; }

    template <typename T>
    requires(std::is_same_v<std::decay_t<T>, Types> || ...)
    void add(T &&action)
    {
        auto &place = std::get<std::vector<std::decay_t<T>>>(all);
        // std::cout << place->size() << " " << place << std::endl;
        order.emplace_back(help::index_in_v<std::decay_t<T>, Types...>, place.size());
        place.emplace_back(std::forward<T>(action));
    }
};
template <typename... Types>
std::ostream &operator<<(std::ostream &out, const default_history<Types...>& h)
{
    // using VecPtrVariant = std::variant<const std::vector<Types>*...>;
    // std::vector<VecPtrVariant> vec_ptr;

    std::vector<std::variant<const std::vector<Types> *...>> vec_ptr;
    std::apply(
        [&out, &vec_ptr](const std::vector<Types> &...vectors) -> void
        {
            auto f = [&out](auto &&vector)
            {
                out << "size: " << vector.size() << std::endl;
                for (auto &&action : vector)
                {
                    out << action << std::endl;
                }
                out << std::endl;
            };
            (f(vectors), ...);
            (vec_ptr.push_back(&vectors), ...);
        },
        h.all);

    for (auto &p : h.order)
    {
        std::visit(
            [&p](auto vec)
            {
                std::cout << (*vec)[p.second] << std::endl;
            },
            vec_ptr[p.first]);
    }

    return out;
}
