#include <optional>
#include <print>
#include <vector>
#include "utils.h"

int main() {
    std::vector<std::string> args = {
        "test1",
        "test2",
        ""
    };
    auto parse = arg_parser(args);
    if (!parse.has_value()) {
        std::println("not value");
        return -1;
    }
    const auto& data = parse.value().first;
    const auto& params = parse.value().second;
    for (const auto& i : data) {
        std::println("data: {}", i);
    }
    for (const auto& i: params) {
        std::println("param: {}: {}", i.first, i.second);
    }
    return 0;
}
