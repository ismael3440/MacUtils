#include <optional>
#include <print>
#include <vector>
#include "utils.h"

int main() {
    std::vector<std::string> args = {
        "test1",
        "test2",
        "-key=value",
        "-verbose=true",
        "127.0.0.1"
    };
    auto parse = arg_parser(args);
    const auto& data = parse.value().first;
    const auto& params = parse.value().second;
    for (const auto& i : data) {
        std::println("data: {}", i);
    }
    for (const auto& i: params) {
        std::println("param: {}: {}", i.first, i.second);
    }
}
