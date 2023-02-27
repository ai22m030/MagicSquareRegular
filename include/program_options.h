//
// Created by Adnan Vatric on 18.02.23.
//

#ifndef MAGIC_SQUARE_PROGRAM_OPTIONS_H
#define MAGIC_SQUARE_PROGRAM_OPTIONS_H

#include <string_view>

namespace program_options {
    bool has(const std::vector<std::string_view>& args, const std::string_view& option_name);

    std::string_view get(const std::vector<std::string_view> &args, const std::string_view &option_name);
}  // namespace program_options

#endif //MAGIC_SQUARE_PROGRAM_OPTIONS_H
