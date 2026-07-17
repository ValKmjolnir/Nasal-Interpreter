#pragma once

#include <vector>
#include <memory>

#include "ast/visitor.hpp"
#include "error/span.hpp"
#include "error/error.hpp"

namespace nasal::linter {

class checker : public visitor {
protected:
    error& err;
    const char* checker_name;

protected:
    void report(expr*, const std::string&);

public:
    checker(error& e, const char* n): err(e), checker_name(n) {}
};

class checker_manager {
private:
    std::vector<std::unique_ptr<checker>> checkers;

public:
    template <typename T>
    void regist_checker(error& err, const char* name) {
        checkers.push_back(std::make_unique<T>(err, name));
    }

    void check(expr*);
};

}