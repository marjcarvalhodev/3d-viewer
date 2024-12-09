#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>

inline void printErr(const std::exception &e)
{
    std::cerr << "Error: " << e.what() << std::endl;
}

// Variadic template function for printing multiple arguments
template <typename T, typename... Args>
inline void print(const T &first, const Args &...rest)
{
    std::cout << first;
    ((std::cout << " " << rest), ...); // Fold expression to print all arguments
    std::cout << std::endl;
}

#endif