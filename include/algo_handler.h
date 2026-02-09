
#pragma once

#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <thread>
#include <functional>

template<typename... Algos>
struct algo_handler {
    using AlgoVariant = std::variant<Algos...>;
    using ContainerWrapperVariant = std::variant<typename Algos::ContainerWrapper...>;
    using HistoryVariant = std::variant<typename Algos::History...>;
    using HistoryRefVariant = std::variant<std::reference_wrapper<typename Algos::History>...>;
    using HistoryRefVariantConst = std::variant<std::reference_wrapper<const typename Algos::History>...>;

    std::optional<AlgoVariant> activ_algo;
    std::optional<ContainerWrapperVariant> container_wrapper;

    template<typename Algo, typename ToContainerWrapper>
    void start(ToContainerWrapper to_data) {
        auto algo = Algo();
        auto data = typename Algo::ContainerWrapper(to_data);
        
        algo.start(data);

        activ_algo = algo;
        container_wrapper = static_cast<ContainerWrapperVariant>(std::move(data));
    }

    std::optional<ContainerWrapperVariant>& get_container_wrapper() {
        return container_wrapper;
    }
    std::optional<HistoryRefVariant> get_history() {
        if (container_wrapper) {
            return std::visit(
                [](auto& h) {
                    return static_cast<HistoryRefVariant>(std::ref(h.get_history()));
                }, *container_wrapper
            );
        
        }
        return std::nullopt;
    }
    [[nodiscard]] std::optional<HistoryRefVariantConst> get_history() const {
        if (container_wrapper) {
            return std::visit(
                [](auto& h) {
                    return static_cast<HistoryRefVariantConst>(std::ref(h.get_history()));
                }, *container_wrapper
            );
        
        }
        return std::nullopt;
    }
};
