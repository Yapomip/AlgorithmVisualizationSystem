
#pragma once

#include "container_wraper.h"

/* not use in example */
template<typename T>
struct defoult_algorithm {
    inline static const char* Name = "AAA";
    using WorkedType = MassWrapper<T>;

    defoult_algorithm() { std::cout << "Create A" << std::endl; }

    void start(WorkedType& mass) {
        for (size_t i = 0; i < mass.size(); ++i) {
            mass.set(i, static_cast<T>(0));
        }

        std::cout << "call g from A" << std::endl;
    }
};


/* not use in example */
template<template<typename... ContainerArgs> typename ContainerType, typename... Args>
struct SetAllZero_impl {
    inline static const char* Name = "SetAllZero";
    using ContainerWrapper = ContainerType<Args...>;
    using Container = ContainerWrapper::Container;
    using T = Container::value_type;

    SetAllZero_impl() { std::cout << "Create SetAllZero impl" << std::endl; }

    void start(ContainerWrapper& mass) const {
        for (size_t i = 0; i < mass.size(); ++i) {
            if (mass.compare(i, mass.size() / 2) == std::strong_ordering::greater) {
                mass.set(i, static_cast<T>(-2));
            } else {
                mass.set(i, static_cast<T>(2));
            }
        }

        std::cout << "call start from SetAllZero" << std::endl;
    }
};

template<typename T>
using SetAllZero = SetAllZero_impl<MassWrapper, T>;

/* not use in example */
template<typename T>
struct B {
    inline static const char* Name = "BBB";
    using WorkedType = MassWrapper<T>;

    B() { std::cout << "Create B" << std::endl; }

    void start(WorkedType& mass) {
        for (size_t i = 0; i < mass.size(); ++i) {
            mass.set(i, static_cast<T>(1));
        }
        std::cout << "call g from B" << std::endl;
    }
};

