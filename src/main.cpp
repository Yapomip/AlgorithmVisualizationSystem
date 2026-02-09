
#include <string>
#include <vector>
#include <variant>
#include <optional>
#include <iostream>
#include <memory>

#include "algo_handler.h"
#include "algorithm.h"

template<typename T>
concept Algoritm = requires(T t) {
    {T::Name} -> std::same_as<const char*&>;
    typename T::WorkedType;
};

template<Algoritm N>
std::string f(N _) {
    return N::Name; 
}

/* TODO concepts for all */
template<typename T>
concept MassContainer = requires(T t, size_t i, size_t j) {
    {t.size()} -> std::same_as<size_t>;
    {t.compare(i, j)} -> std::same_as<std::strong_ordering>;
    {t.swap(i, j)} -> std::same_as<void>;
};

/* not use in example */
template<typename T>
struct a {
    inline static const char* name = "AAA";
    using WorkedType = MassWrapper<T>;

    a() { std::cout << "Create A" << std::endl; }

    void start(WorkedType& mass) {
        for (size_t i = 0; i < mass.size(); ++i) {
            mass.set(i, static_cast<T>(0));
        }

        std::cout << "call g from A" << std::endl;
    }
};

/* not use in example */
template<typename... Types>
requires (Algoritm<Types> && ...) && (std::default_initializable<Types> && ...)
struct handler {
    std::optional<std::variant<Types...>> n;

    explicit handler(const std::string& _) : n(std::nullopt) {
        // bool is_init = (((name == Types::Name) ? (n = Types(), true) : false) || ...);

        if (n) {
            std::cout << "Initial index: " << n.value().index() << std::endl;
        }
    }

    void call_start() {
        if (n) {
            std::visit(
                [](auto t) {
                    t.start();
                }, n.value()
            );
        }
    }
};

void aaa(MassWrapper<int> c) {
    for (size_t i = 0; i < c.size(); ++i) {
        c.set(i, 0);
    }
    c.set(0, 1);
    c.set(1, 1);
    for (size_t i = 2; i < c.size(); ++i) {
        c.set(i, 21);
    }
}


// #include "a.h"

int main() {

    std::map<size_t, int> map{
        {0, 1},
        {1, 2},
        {2, 3},
        {3, 4},
        {4, 5},
        {5, 6},
        {6, 7},
        {7, 8},
        {8, 9},
        {9, 10},
    };

    map_wrap<int> map_container(map);
    // a::Massive<int> m;
    algo_handler<SetAllZero<int>> handler;
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    handler.start<SetAllZero<int>>(data);
    // auto h = handler.get_history();
    std::visit(
        [](auto&& h) {
            std::cout << h.get() << std::endl;
        },
        handler.get_history().value()
    );

    std::visit(
        [&map_container](auto&& h) {
            auto order_vec = h.get().get_order();

            for (auto& action_option : order_vec) {
                std::visit(
                    [&map_container](auto&& action) {
                        apply_action(action, map_container);
                    },
                    action_option
                );
            }
        },
        handler.get_history().value()
    );

    std::visit(
        [](auto&& h) {
            auto&& c = h.get_container();
            
            for (size_t i = 0; i < 10; ++i) {
                std::cout << i << " " << c[i] << std::endl;
            }
        },
        handler.get_container_wrapper().value()
    );


    for (size_t i = 0; i < 10; ++i) {
        std::cout << i << " " << map_container[i] << std::endl;
    }
    /* how to want 
    handler<A, B>()
    
    */
    /*
    std::vector<int> data = {1, 2, 3, 4, 5};
    mass_history<int> h;
    MassWrapper<int> mw(data, h);

    {
        auto midle = data[mw.size() / 2];
        for (auto i = 0; i < mw.size(); ++i) {
            if (mw.compare(mw.size() / 2, i) == std::strong_ordering::less) {
                mw.set(i, 1);
            } else {
                mw.set(i, 5);
            }
        }
    }

    std::cout << h << std::endl;
    */
    // std::cout << f(A{}) << std::endl;
    // std::cout << f(B{}) << std::endl;
    // handler<A, B> handler("BBB");
    // handler.call_g();
    return 0;
}
