#include <iostream>

#include "repl/repl.hpp"
#include "util/filestream.hpp"
#include "util/colorful.hpp"
#include "util/virtual_source.hpp"

namespace nasal {

static void split_lines(const std::string& src, std::vector<std::string>& dst) {
    dst = {};
    size_t pos = 0, last = 0;
    while ((pos = src.find("\n", last)) != std::string::npos) {
        dst.push_back(src.substr(last, pos - last));
        last = pos + 1;
    }
    if (last < src.length()) {
        dst.push_back(src.substr(last));
    } else {
        dst.push_back("");
    }
}

void filestream::load(const std::string& f) {
    // don't need to load a loaded file
    if (file == f) {
        return;
    }

    // update file name
    file = f;

    // load virtual file if filename hits
    if (auto* src = virtual_source_registry::instance().get(f)) {
        split_lines(*src, res);
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
