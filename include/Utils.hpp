#pragma once
#include <format>
#include <stacktrace>
#include <print>

namespace Utils {
    template <typename... Args>
    [[noreturn]] void fatalError(std::format_string<Args...> fmt, Args&&... args) {
        auto stack = std::stacktrace::current();
        std::println(stderr, "=====FATAL ERROR=====");
        std::println(stderr, fmt, std::forward<Args>(args)...);
        std::println(stderr, "Stack trace:\n{}", stack);
        std::abort();
    }

    template <typename... Args>
    void assertTrue(bool condition, std::format_string<Args...> fmt, Args&&... args) {
        if (!condition) {
            fatalError(fmt, std::forward<Args>(args)...);
        }
    }
}