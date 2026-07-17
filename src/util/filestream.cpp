#include <iostream>

#include "repl/repl.hpp"
#include "util/filestream.hpp"
#include "util/colorful.hpp"

namespace nasal {

void filestream::load(const std::string& f) {
    // don't need to load a loaded file
    if (file == f) {
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

}
