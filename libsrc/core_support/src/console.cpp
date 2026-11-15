#include "console.h"

#include <iostream>
#include <windows.h>

void CoreConsoleWrite(std::int32_t direction, const std::string &value)
{
    (direction == 2 ? std::cerr : std::cout) << value;
}

void CoreConsoleWrite(const std::string &value)
{
    CoreConsoleWrite(1, value);
}
std::string CoreConsoleRead(bool echo)
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    DWORD original = 0;
    const bool console = input != INVALID_HANDLE_VALUE && GetConsoleMode(input, &original) != FALSE;
    if (console && !echo)
        SetConsoleMode(input, original & ~ENABLE_ECHO_INPUT);
    std::string result;
    std::getline(std::cin, result);
    if (console && !echo)
    {
        SetConsoleMode(input, original);
        std::cout << '\n';
    }
    return result;
}
