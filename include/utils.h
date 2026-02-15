
#pragma once

template<typename T> struct Key { T value; };
template<typename T> struct Value { T value; };

template<class... Ts>
struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;
 

