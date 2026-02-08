
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
std::string f(N n) {
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
struct A {
    inline static const char* Name = "AAA";
    using WorkedType = MassWrapper<T>;

    A() { std::cout << "Create A" << std::endl; }

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

    handler(const std::string& name) : n(std::nullopt) {
        bool is_init = (((name == Types::Name) ? (n = Types(), true) : false) || ...);

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

// #include "a.h"

int main() {

    // a::Massive<int> m;
    
    algo_handler<SetAllZero<int>> handler;
    std::vector<int> data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    handler.start<SetAllZero<int>>(data);

    std::visit(
        [](auto&& h) {
            std::cout << h << std::endl;
        },
        handler.get_history().value()
    );
    
    /* how to want 
    handler<A, B>()
    
    */
// handler
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
