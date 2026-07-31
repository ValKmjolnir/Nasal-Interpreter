#include "natives/registry.hpp"
#include "error/error.hpp"
#include "lexer/lexer.hpp"
#include "parse/parse.hpp"
#include "parse/linker.hpp"
#include "code/codegen.hpp"
#include "code/compilation.hpp"
#include "util/resource_manager.hpp"
#include "util/virtual_source.hpp"
#include "vm/vm.hpp"

#include <sstream>

namespace nasal {

var builtin_compile(context* ctx, gc* ngc) {
    auto input = ctx->localr[1];
    if (!input.is_str()) {
        return nas_err("compile", "input must be a string");
    }

    nasal::error err;
    nasal::resource_manager resm;
    nasal::lexer lex(err);
    nasal::parse parse(err);
    nasal::linker ld(err, resm);
    nasal::compilation comp(false);
    nasal::codegen gen(err, comp, resm);

    auto virt_src_name = "<compile-" + std::to_string(
        virtual_source_registry::instance().size()
    ) + ">";

    virtual_source_registry::instance().regist(
        virt_src_name, input.str()
    );

    if (lex.scan(virt_src_name).geterr()) {
        return nil;
    }
    if (parse.compile(lex.result()).geterr()) {
        return nil;
    }

    if (ld.link(parse, false).geterr()) {
        return nil;
    }

    if (gen.compile(parse.tree(), false).geterr()) {
        return nil;
    }

    std::stringstream ss;
    gen.print(ss);

    auto runtime = std::make_unique<vm>();
    runtime->run(comp, resm, {});

    return ngc->alloc_str(ss.str());
}

void load_eval_builtin() {
    nasal_builtin_info builtin[] = {
        {"__compile", builtin_compile}
    };

    auto& registry = nasal_builtin_registry::get();
    for (auto& info: builtin) {
        if (info.name) {
            registry.registered_builtin.insert(info.name);
            registry.builtin_table.push_back(info);
        }
    }
}

}