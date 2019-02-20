#pragma once

#include <iostream>
#include <string>

inline void _logic_assert(const char * expression, const std::string & reason, const char * file, int line)
{
    std::cout
        << "Assertion Failed: (" << expression << ")\n"
        << "{\n"
        << "    Reason: " << reason << "\n"
        << "    File: " << file << "\n"
        << "    Line: " << line << "\n"
        << "}\n";

    std::abort();
}

#ifdef la_remove_release

#define logic_assert(EX, reason) ((void) 0)

#else

#define logic_assert(EX, reason) ((EX) ? (void) 0 : _logic_assert(#EX, reason, __FILE__, __LINE__))

#endif // la_remove_in_release
