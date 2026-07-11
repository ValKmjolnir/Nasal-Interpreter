#include "util/colorful.hpp"

#ifdef _WIN32
    #include <io.h>
    #define isatty(fd) _isatty(fd)
    #define fileno(fd) _fileno(fd)
#else
    #include <unistd.h>
#endif

namespace nasal {

static bool stdout_is_tty() {
    static bool init = false;
    static bool tty = false;
    if (!init) {
        init = true;
        tty = isatty(fileno(stdout));
    }
    return tty;
}

static bool stderr_is_tty() {
    static bool init = false;
    static bool tty = false;
    if (!init) {
        init = true;
        tty = isatty(fileno(stderr));
    }
    return tty;
}

static bool enable_color(std::ostream& s) {
    if (s.rdbuf() == std::cout.rdbuf()) {
        return stdout_is_tty();
    }
    if (s.rdbuf() == std::cerr.rdbuf()) {
        return stderr_is_tty();
    }
    return false;
}

#ifdef _WIN32
#include <windows.h> // use SetConsoleTextAttribute
struct for_reset {
    CONSOLE_SCREEN_BUFFER_INFO scr;
    for_reset() {
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &scr);
    }
};
static for_reset windows_system_set;
#endif

std::ostream& clear_screen(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) {
        return s;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        return s;
    }

    auto rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    auto cols = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD coord = { 0, 0 };
    DWORD dwCharsWritten;

    FillConsoleOutputCharacter(hConsole, ' ', dwConSize, coord, &dwCharsWritten);

    // set raw attribute
    FillConsoleOutputAttribute(
        hConsole,
        csbi.wAttributes,
        dwConSize,
        coord,
        &dwCharsWritten
    );

    // set cursor position
    SetConsoleCursorPosition(hConsole, coord);
#else
    s << "\033c";
#endif
    return s;
}

std::ostream& set_cursor(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
#else
    s << "\033[0;0H";
#endif
    return s;
}

std::ostream& back_white(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
#else
    s << "\033[7m";
#endif
    return s;
}

std::ostream& red(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
#else
    s << "\033[91;1m";
#endif
    return s;
}

std::ostream& cyan(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
#else
    s << "\033[36;1m";
#endif
    return s;
}

std::ostream& orange(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
#else
    s << "\033[93;1m";
#endif
    return s;
}

std::ostream& white(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
#else
    s << "\033[0m\033[1m";
#endif
    return s;
}

std::ostream& reset(std::ostream& s) {
    if (!enable_color(s)) {
        return s;
    }

#ifdef _WIN32
    SetConsoleTextAttribute(
        GetStdHandle(STD_OUTPUT_HANDLE),
        windows_system_set.scr.wAttributes
    );
#else
    s << "\033[0m";
#endif
    return s;
}

}