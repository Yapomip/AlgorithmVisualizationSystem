
#pragma once

#include <algorithm>
#include <array>

#include "container_wraper.h"

/* not use in example */
template<typename T>
struct defoult_algorithm {
    inline static const char* name = "Defoult Algorithm";
    using WorkedType = MassWrapper<T>;

    defoult_algorithm() { std::cout << "Create defoult algorithm" << std::endl; }

    void start(WorkedType& mass) {
        for (size_t i = 0; i < mass.size(); ++i) {
            mass.set(i, static_cast<T>(0));
        }

        std::cout << "call g from A" << std::endl;
    }
};

/* not use in example */
template<template<typename... ContainerArgs> typename ContainerWrapperType, typename... Args>
struct set_all_zero_impl {
    inline static const char* name = "Set All Zero";
    using ContainerWrapper = ContainerWrapperType<Args...>;
    using Container = ContainerWrapper::Container;
    using History = ContainerWrapper::History;
    using T = Container::value_type;

    set_all_zero_impl() { std::cout << "Create Set All Zero impl" << std::endl; }

    void start(ContainerWrapper& mass) const {
        std::cout << "call start from SetAllZero" << std::endl;
        for (size_t i = 0; i < mass.size(); ++i) {
            if (mass.compare(i, mass.size() / 2) == std::strong_ordering::greater) {
                mass.set(i, static_cast<T>(-2));
            } else {
                // mass.set(i, static_cast<T>(2));
                mass.set(i, 2);
            }
        }
    }
};

template<typename T>
using SetAllZero = set_all_zero_impl<MassWrapper, T>;

/* not use in example */
template<typename T>
struct b {
    inline static const char* name = "BBB";
    using WorkedType = MassWrapper<T>;

    b() { std::cout << "Create b" << std::endl; }

    void start(WorkedType& mass) {
        std::cout << "call g from b" << std::endl;
        for (size_t i = 0; i < mass.size(); ++i) {
            mass.set(i, static_cast<T>(1));
        }
    }
};

namespace help {
    template<size_t N>
    struct fixed_string {
        std::array<char, N> data;
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays, modernize-avoid-c-arrays)
        constexpr explicit fixed_string(const char (&str)[N]) {
            std::copy_n(str, N, data.data);
        }
    };
}; // namespace help

/*
template<help::fixed_string Name, template<typename T> typename ContainerWrapperType>
struct AlgoFromFunction {
    using ContainerWrapper = ContainerType<Args...>;
    using Container = ContainerWrapper::Container;
    using History = ContainerWrapper::History;
    using T = Container::value_type;

    SetAllZero_impl() { std::cout << Name.data << std::endl; }

    void start(ContainerWrapper& mass) const {
        for (size_t i = 0; i < mass.size(); ++i) {
            if (mass.compare(i, mass.size() / 2) == std::strong_ordering::greater) {
                mass.set(i, static_cast<T>(-2));
            } else {
                // mass.set(i, static_cast<T>(2));
                mass.set(i, 2);
            }
        }

        std::cout << "call start from SetAllZero" << std::endl;
    }
};

*/