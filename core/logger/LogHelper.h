//
// Created by sanek on 12/7/2025.
//

#ifndef MINIMALSDL2APP_LOGHELPER_H
#define MINIMALSDL2APP_LOGHELPER_H
#include <format>
#include <cstdio>
#include <string>

namespace LogHelper {

    inline void debug(const std::string& msg) {
        std::fputs("[DEBUG] ", stdout);
        std::fputs(msg.c_str(), stdout);
        std::fputc('\n', stdout);
    }

    template <typename... Args>
    inline void debug(std::string_view fmt, Args&&... args) {
        auto msg = std::vformat(fmt, std::make_format_args(args...));
        std::fputs("[DEBUG] ", stdout);
        std::fputs(msg.c_str(), stdout);
        std::fputc('\n', stdout);
    }

} // namespace log

#ifdef ENABLE_DEBUG_LOGS
// Macro that forwards all arguments correctly
#define DEBUG_LOG(...) ::LogHelper::debug(__VA_ARGS__)
#else
// Logs removed entirely from Release builds
#define DEBUG_LOG(...) (void)0
#endif


#endif //MINIMALSDL2APP_LOGHELPER_H