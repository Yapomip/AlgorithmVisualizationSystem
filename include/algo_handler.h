
#pragma once

#include <string>
#include <memory>
#include <variant>
#include <optional>
#include <thread>

template<typename... Algs>
struct algo_handler {
    std::optional<std::variant<Algs...>> activ_algo;
    std::optional<std::variant<typename Algs::ContainerWrapper...>> container_wrapper;

    std::thread th;


    template<typename Algo, typename ToContainerWrapper>
    void start(ToContainerWrapper to_data) {
        auto algo = Algo();
        auto data = typename Algo::ContainerWrapper(to_data);

        // th = std::thread([algo, data](){
        //     algo.start(data.get_container());
        // });
        // th.detach();
        algo.start(data);
        activ_algo = algo;
        container_wrapper = std::variant<typename Algs::ContainerWrapper...>(data);
    }

    std::optional<std::variant<typename Algs::ContainerWrapper::History...>> get_history() {
        if (container_wrapper) {
            return std::visit(
                [](auto&& h){
                    return h.get_history();
                }, container_wrapper.value()
            );
        }
        return std::nullopt;
    }
    /*
    void start(std::variant<Algs...> algo) {
        th = [algo](){
            algo.start();
        };
        th.detach();
        activ_algo = algo;
    }

    template<typename T>
    void start_from_name(const std::string& Name) {
        th = [](){};
        th.detach();
    }
    */
};

/*



algo_handler<SetAllZero<int>, BubleSort<int>, SelectionSort<int>> ah();

algo_handler.get_history();

*/
