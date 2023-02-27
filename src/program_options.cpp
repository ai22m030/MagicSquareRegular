//
// Created by Adnan Vatric on 18.02.23.
//

#include "program_options.h"

#include <vector>

std::string_view program_options::get(
        const std::vector<std::string_view>& args,
        const std::string_view& option_name) {
    for (auto it = args.begin(), end = args.end(); it != end; ++it) {
        if (*it == option_name)
            if (it + 1 != end)
                return *(it + 1);
    }

    return "";
}

bool program_options::has(
        const std::vector<std::string_view>& args,
        const std::string_view& option_name) {
    for (auto arg : args) {
        if (arg == option_name)
            return true;
    }

    return false;
}
