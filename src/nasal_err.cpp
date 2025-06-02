#include "nasal_err.h"
#include "repl/repl.h"

namespace nasal {

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
#ifdef _WIN32
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {0, 0});
#else
    s << "\033[0;0H";
#endif
    return s;
}

std::ostream& back_white(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xf0);
#else
    s << "\033[7m";
#endif
    return s;
}

std::ostream& red(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0c);
#else
    s << "\033[91;1m";
#endif
    return s;
}

std::ostream& cyan(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
#else
    s << "\033[36;1m";
#endif
    return s;
}

std::ostream& orange(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0e);
#else
    s << "\033[93;1m";
#endif
    return s;
}

std::ostream& white(std::ostream& s) {
#ifdef _WIN32
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f);
#else
    s << "\033[0m\033[1m";
#endif
    return s;
}

std::ostream& reset(std::ostream& s) {
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

void filestream::load(const std::string& f) {
    if (file==f) { // don't need to load a loaded file
        return;
    }
    
    // update file name
    file = f;

    // REPL: load from memory
    if (repl::info::instance()->in_repl_mode &&
        repl::info::instance()->repl_file_name==file) {
        const auto& source = repl::info::instance()->repl_file_source;
        res = {};
        size_t pos = 0, last = 0;
        while ((pos = source.find("\n", last))!=std::string::npos) {
            res.push_back(source.substr(last, pos - last));
            last = pos + 1;
        }
        if (last<source.length()) {
            res.push_back(source.substr(last));
        } else {
            res.push_back("");
        }
        return;
    }

    res.clear();
    std::ifstream in(f, std::ios::binary);
    if (in.fail()) {
        std::cerr << red << "src: " << reset << "cannot open <" << f << ">\n";
        std::exit(1);
    }
    
    while(!in.eof()) {
        std::string line;
        std::getline(in, line);
        res.push_back(line);
    }
}

void error::err(const std::string& stage, const std::string& info) {
    ++cnt;
    std::cerr << red << stage << ": " << white << info << reset << "\n\n";
}

void error::warn(const std::string& stage, const std::string& info) {
    std::clog << orange << stage << ": " << white << info << reset << "\n\n";
}

void error::err(const std::string& stage,
                const span& loc,
                const std::string& info) {
    // load error occurred file into string lines
    load(loc.file);

    ++cnt;

    std::cerr << red << stage << ": " << white << info << reset << "\n";
    std::cerr << cyan << "  --> " << red;
    loc.dump_begin(std::cerr);
    std::cerr << reset << "\n";

    const usize maxlen = std::to_string(loc.end_line).length();
    const std::string iden = identation(maxlen);

    for (u64 line = loc.begin_line; line<=loc.end_line; ++line) {
        // skip line 0
        if (!line) {
            continue;
        }

        if (loc.begin_line<line && line<loc.end_line) {
            if (line==loc.begin_line+1) {
                std::cerr << cyan << iden << " | " << reset << "...\n";
                std::cerr << cyan << iden << " | " << reset << "\n";
            }
            continue;
        }

        // if this line has nothing, skip
        if (!res[line-1].length() && line!=loc.end_line) {
            continue;
        }

        // line out of range
        if (line-1>=res.size()) {
            continue;
        }

        const auto& code = res[line-1];
        std::cerr << cyan << leftpad(line, maxlen) << " | " << reset << code << "\n";
        // output underline
        std::cerr << cyan << iden << " | " << reset;
        if (loc.begin_line==loc.end_line) {
            for (u64 i = 0; i<loc.begin_column; ++i) {
                std::cerr << char(" \t"[code[i]=='\t']);
            }
            for (u64 i = loc.begin_column; i<loc.end_column; ++i) {
                std::cerr << red << (code[i]=='\t'? "^^^^":"^") << reset;
            }
        } else if (line==loc.begin_line) {
            for (u64 i = 0; i<loc.begin_column; ++i) {
                std::cerr << char(" \t"[code[i]=='\t']);
            }
            for (u64 i = loc.begin_column; i<code.size(); ++i) {
                std::cerr << red << (code[i]=='\t'? "^^^^":"^") << reset;
            }
        } else if (loc.begin_line<line && line<loc.end_line) {
            for (u64 i = 0; i<code.size(); ++i) {
                std::cerr << red << (code[i]=='\t'? "^^^^":"^");
            }
        } else {
            for (u64 i = 0; i<loc.end_column; ++i) {
                std::cerr << red << (code[i]=='\t'? "^^^^":"^");
            }
        }
        if (line==loc.end_line) {
            std::cerr << reset;
        } else {
            std::cerr << reset << "\n";
        }
    }
    std::cerr << "\n\n";
}

}
