#include "error/error.hpp"
#include "repl/repl.hpp"
#include "util/colorful.hpp"

namespace nasal {

void filestream::load(const std::string& f) {
    if (file == f) { // don't need to load a loaded file
        return;
    }

    // update file name
    file = f;

    // REPL: load from memory
    if (repl::info::instance()->in_repl_mode &&
        repl::info::instance()->repl_file_name == file) {
        const auto& source = repl::info::instance()->repl_file_source;
        res = {};
        size_t pos = 0, last = 0;
        while ((pos = source.find("\n", last)) != std::string::npos) {
            res.push_back(source.substr(last, pos - last));
            last = pos + 1;
        }
        if (last < source.length()) {
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

    while (!in.eof()) {
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

    for (u64 line = loc.begin_line; line <= loc.end_line; ++line) {
        // skip line 0
        if (!line) {
            continue;
        }

        if (loc.begin_line < line && line < loc.end_line) {
            if (line == loc.begin_line + 1) {
                std::cerr << cyan << iden << " | " << reset << "...\n";
                std::cerr << cyan << iden << " | " << reset << "\n";
            }
            continue;
        }

        // if this line has nothing, skip
        if (!res[line - 1].length() && line != loc.end_line) {
            continue;
        }

        // line out of range
        if (line - 1 >= res.size()) {
            continue;
        }

        const auto& code = res[line - 1];
        std::cerr << cyan << leftpad(line, maxlen) << " | " << reset << code << "\n";
        // output underline
        std::cerr << cyan << iden << " | " << reset;
        if (loc.begin_line == loc.end_line) {
            for (u64 i = 0; i < loc.begin_column; ++i) {
                std::cerr << char(" \t"[code[i] == '\t']);
            }
            for (u64 i = loc.begin_column; i < loc.end_column; ++i) {
                std::cerr << red << (code[i] == '\t' ? "^^^^" : "^") << reset;
            }
        } else if (line == loc.begin_line) {
            for (u64 i = 0; i < loc.begin_column; ++i) {
                std::cerr << char(" \t"[code[i] == '\t']);
            }
            for (u64 i = loc.begin_column; i < code.size(); ++i) {
                std::cerr << red << (code[i] == '\t' ? "^^^^" : "^") << reset;
            }
        } else if (loc.begin_line < line && line < loc.end_line) {
            for (u64 i = 0; i < code.size(); ++i) {
                std::cerr << red << (code[i] == '\t' ? "^^^^" : "^");
            }
        } else {
            for (u64 i = 0; i < loc.end_column; ++i) {
                std::cerr << red << (code[i] == '\t' ? "^^^^" : "^");
            }
        }
        if (line == loc.end_line) {
            std::cerr << reset;
        } else {
            std::cerr << reset << "\n";
        }
    }
    std::cerr << "\n\n";
}

}
