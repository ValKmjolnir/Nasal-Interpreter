#include "nasal_web.h"
#include "nasal_vm.h"
#include "nasal_parse.h"
#include "nasal_codegen.h"
#include "nasal_import.h"
#include "optimizer.h"
#include "nasal_err.h"
#include "nasal_lexer.h"

#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <chrono>

struct NasalContext {
    std::unique_ptr<nasal::vm> vm_instance;
    std::string last_result;
    std::string last_error;

    NasalContext() {
        vm_instance = std::make_unique<nasal::vm>();
    }
};

void* nasal_init() {
    return new NasalContext();
}

void nasal_cleanup(void* context) {
    delete static_cast<NasalContext*>(context);
}

const char* nasal_eval(void* context, const char* code, int show_time) {
    using clk = std::chrono::high_resolution_clock;
    const auto den = clk::duration::period::den;
    
    auto* ctx = static_cast<NasalContext*>(context);
    
    try {
        nasal::lexer lex;
        nasal::parse parse;
        nasal::linker ld;
        nasal::codegen gen;
        
        // Create a unique temporary file
        char temp_filename[] = "/tmp/nasal_eval_XXXXXX.nasal";
        int fd = mkstemp(temp_filename);
        if (fd == -1) {
            throw std::runtime_error("Failed to create temporary file");
        }

        // Write the code to the temporary file
        std::ofstream temp_file(temp_filename);
        if (!temp_file.is_open()) {
            close(fd);
            throw std::runtime_error("Failed to open temporary file for writing");
        }
        temp_file << code;
        temp_file.close();
        close(fd); // Close the file descriptor
        
        // Capture stdout and stderr
        std::stringstream output;
        std::stringstream error_output;
        std::streambuf* old_cout = std::cout.rdbuf(output.rdbuf());
        std::streambuf* old_cerr = std::cerr.rdbuf(error_output.rdbuf());

        // Process the code by scanning the temporary file
        if (lex.scan(std::string(temp_filename)).geterr()) {
            ctx->last_error = error_output.str();
            std::cout.rdbuf(old_cout);
            std::cerr.rdbuf(old_cerr);
            // Remove the temporary file
            std::remove(temp_filename);
            return ctx->last_error.c_str();
        }

        if (parse.compile(lex).geterr()) {
            ctx->last_error = error_output.str();
            std::cout.rdbuf(old_cout);
            std::cerr.rdbuf(old_cerr);
            // Remove the temporary file
            std::remove(temp_filename);
            return ctx->last_error.c_str();
        }

        ld.link(parse, false).chkerr();
        // optimizer does simple optimization on ast
        auto opt = std::make_unique<nasal::optimizer>();
        opt->do_optimization(parse.tree());
        gen.compile(parse, ld, false, true).chkerr(); // enable limit_mode for safety

        // Run the code with optional timing
        const auto start = show_time ? clk::now() : clk::time_point();
        ctx->vm_instance->run(gen, ld, {});
        const auto end = show_time ? clk::now() : clk::time_point();
        
        // Restore stdout and stderr and get the outputs
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);
        
        std::stringstream result;
        result << output.str();
        if (!error_output.str().empty()) {
            result << error_output.str();
        }
        if (result.str().empty()) {
            result << "Execution completed successfully.\n";
        }
        
        // Add execution time if requested
        if (show_time) {
            double execution_time = static_cast<double>((end-start).count())/den;
            result << "\nExecution time: " << execution_time << "s";
        }
        
        ctx->last_result = result.str();

        // Remove the temporary file
        std::remove(temp_filename);
        
        return ctx->last_result.c_str();
    } catch (const std::exception& e) {
        ctx->last_error = e.what();
        return ctx->last_error.c_str();
    }
}

const char* nasal_get_error(void* context) {
    auto* ctx = static_cast<NasalContext*>(context);
    return ctx->last_error.c_str();
}
