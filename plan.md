

c[i]   ->    set_wrap { i, &c }

c[i] = c[j]   ->   set_wrap { i, &c } = set_wrap { j, &c }   ->   c.get_history().add(set_wrap(i, j))

c[i] > c[j]   ->   set_wrap { i, &c } > set_wrap { j, &c }   ->   c.get_history().add(compare_wrap(i, j))


add history include

add action wrapper include



/usr/bin/clang-tidy-18 /home/yapomip/project/AlgorithmVisualizationSystem/include/action.h /home/yapomip/project/AlgorithmVisualizationSystem/include/action_wraper.h /home/yapomip/project/AlgorithmVisualizationSystem/include/actions/compare.h /home/yapomip/project/AlgorithmVisualizationSystem/include/actions/set.h /home/yapomip/project/AlgorithmVisualizationSystem/include/algo_handler.h /home/yapomip/project/AlgorithmVisualizationSystem/include/algorithm.h /home/yapomip/project/AlgorithmVisualizationSystem/include/container_wraper.h /home/yapomip/project/AlgorithmVisualizationSystem/include/history.h /home/yapomip/project/AlgorithmVisualizationSystem/src/main.cpp --config-file=/home/yapomip/project/AlgorithmVisualizationSystem/.clang-tidy --header-filter=.* -p /home/yapomip/project/AlgorithmVisualizationSystem/build

make -C build lint

method_wrapper<
compare<unsigned long>, 
const action_wrapper<container_wrapper<vector_wrap<int>, set, compare>, vector_wrap<int>, default_history<set<unsigned long, int>, compare<unsigned long>>, set, compare>, const name_method_wrapper<compare, vector_wrap<int>, action_wrapper<container_wrapper<vector_wrap<int>, set, compare>, vector_wrap<int>, default_history<set<unsigned long, int>, compare<unsigned long>>, set, compare>>, unsigned long &, unsigned long &>'
