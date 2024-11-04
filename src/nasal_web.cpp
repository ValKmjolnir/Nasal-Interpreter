#include "nasal_web.h"
#include "nasal_vm.h"
#include "nasal_parse.h"
#include "nasal_codegen.h"
#include "nasal_import.h"
#include "optimizer.h"
#include "nasal_err.h"
#include "nasal_lexer.h"
#include "repl/repl.h"

#include <string>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <stdexcept>
#include <chrono>
#include <vector>

namespace {
    // Helper function implementations inside anonymous namespace
    std::vector<std::string> split_string(const std::string& str, char delim) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;
        while (std::getline(ss, item, delim)) {
            result.push_back(item);
        }
        return result;
    }

    std::string join_string(const std::vector<std::string>& vec, const std::string& delim) {
        if (vec.empty()) return "";
        std::stringstream ss;
        ss << vec[0];
        for (size_t i = 1; i < vec.size(); ++i) {
            ss << delim << vec[i];
        }
        return ss.str();
    }
}

struct NasalContext {
    std::unique_ptr<nasal::vm> vm_instance;
    std::string last_result;
    std::string last_error;

    NasalContext() {
        vm_instance = std::make_unique<nasal::vm>();
    }
};

struct WebReplContext {
    std::unique_ptr<nasal::repl::repl> repl_instance;
    std::vector<std::string> source;
    std::string last_result;
    std::string last_error;
    bool allow_output;
    bool initialized;

    WebReplContext() : allow_output(false), initialized(false) {
        repl_instance = std::make_unique<nasal::repl::repl>();
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
        char temp_filename[256];
        snprintf(temp_filename, sizeof(temp_filename), "/tmp/nasal_eval_%d.nasal", getpid());
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
        close(fd);
        
        // Capture stdout and stderr
        std::stringstream output;
        std::stringstream error_output;
        auto old_cout = std::cout.rdbuf(output.rdbuf());
        auto old_cerr = std::cerr.rdbuf(error_output.rdbuf());

        // Process the code
        if (lex.scan(std::string(temp_filename)).geterr()) {
            ctx->last_error = error_output.str();
            std::cout.rdbuf(old_cout);
            std::cerr.rdbuf(old_cerr);
            std::remove(temp_filename);
            return ctx->last_error.c_str();
        }

        if (parse.compile(lex).geterr()) {
            ctx->last_error = error_output.str();
            std::cout.rdbuf(old_cout);
            std::cerr.rdbuf(old_cerr);
            std::remove(temp_filename);
            return ctx->last_error.c_str();
        }

        ld.link(parse, false).chkerr();
        auto opt = std::make_unique<nasal::optimizer>();
        opt->do_optimization(parse.tree());
        gen.compile(parse, ld, false, true).chkerr();

        const auto start = show_time ? clk::now() : clk::time_point();
        ctx->vm_instance->run(gen, ld, {});
        const auto end = show_time ? clk::now() : clk::time_point();
        
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
        
        if (show_time) {
            double execution_time = static_cast<double>((end-start).count())/den;
            result << "\nExecution time: " << execution_time << "s";
        }
        
        ctx->last_result = result.str();
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

void* nasal_repl_init() {
    auto* ctx = new WebReplContext();
    
    try {
        // Initialize environment silently
        nasal::repl::info::instance()->in_repl_mode = true;
        ctx->repl_instance->get_runtime().set_repl_mode_flag(true);
        ctx->repl_instance->get_runtime().set_detail_report_info(false);
        
        // Run initial setup
        ctx->repl_instance->set_source({});
        if (!ctx->repl_instance->run()) {
            ctx->last_error = "Failed to initialize REPL environment";
            return ctx;
        }
        
        // Enable output after initialization
        ctx->allow_output = true;
        ctx->repl_instance->get_runtime().set_allow_repl_output_flag(true);
        ctx->initialized = true;
    } catch (const std::exception& e) {
        ctx->last_error = std::string("Initialization error: ") + e.what();
    }
    
    return ctx;
}

void nasal_repl_cleanup(void* context) {
    delete static_cast<WebReplContext*>(context);
}

const char* nasal_repl_eval(void* context, const char* line) {
    auto* ctx = static_cast<WebReplContext*>(context);
    
    if (!ctx->initialized) {
        ctx->last_error = "REPL not properly initialized";
        return ctx->last_error.c_str();
    }

    try {
        std::string input_line(line);
        
        // Handle empty input
        if (input_line.empty()) {
            ctx->last_result = "";
            return ctx->last_result.c_str();
        }

        // Handle REPL commands
        if (input_line[0] == '.') {
            if (input_line == ".help" || input_line == ".h") {
                ctx->last_result = 
                    "Nasal REPL commands:\n"
                    "  .help  .h     show this help message\n"
                    "  .clear .c     clear screen\n"
                    "  .exit  .e     exit repl\n"
                    "  .quit  .q     exit repl\n"
                    "  .source .s    show source\n";
                return ctx->last_result.c_str();
            }
            else if (input_line == ".clear" || input_line == ".c") {
                ctx->last_result = "\033c"; // Special marker for clear screen
                return ctx->last_result.c_str();
            }
            else if (input_line == ".exit" || input_line == ".e" ||
                     input_line == ".quit" || input_line == ".q") {
                ctx->last_result = "__EXIT__"; // Special marker for exit
                return ctx->last_result.c_str();
            }
            else if (input_line == ".source" || input_line == ".s") {
                // Return accumulated source
                ctx->last_result = ctx->source.empty() ? 
                    "(no source)" : 
                    join_string(ctx->source, "\n");
                return ctx->last_result.c_str();
            }
            else {
                ctx->last_error = "no such command \"" + input_line + "\", input \".help\" for help";
                return ctx->last_error.c_str();
            }
        }

        // Add the line to source
        ctx->source.push_back(input_line);

        // Capture output
        std::stringstream output;
        auto old_cout = std::cout.rdbuf(output.rdbuf());
        auto old_cerr = std::cerr.rdbuf(output.rdbuf());

        // Update source in repl instance and run
        ctx->repl_instance->get_runtime().set_repl_mode_flag(true);
        ctx->repl_instance->get_runtime().set_allow_repl_output_flag(true);
        ctx->repl_instance->set_source(ctx->source);
        bool success = ctx->repl_instance->run();

        // Restore output streams
        std::cout.rdbuf(old_cout);
        std::cerr.rdbuf(old_cerr);

        // Get the output
        std::string result = output.str();

        if (!success) {
            ctx->last_error = result;
            ctx->source.pop_back(); // Remove failed line
            return ctx->last_error.c_str();
        }

        // // Process output
        // auto lines = split_string(result, '\n');
        // if (!lines.empty()) {
        //     // Remove empty lines from the end
        //     while (!lines.empty() && lines.back().empty()) {
        //         lines.pop_back();
        //     }
        //     result = join_string(lines, "\n");
        // }

        ctx->last_result = result;
        return ctx->last_result.c_str();

    } catch (const std::exception& e) {
        ctx->last_error = std::string("Error: ") + e.what();
        ctx->source.pop_back(); // Remove failed line
        return ctx->last_error.c_str();
    }
}

int nasal_repl_is_complete(void* context, const char* line) {
    auto* ctx = static_cast<WebReplContext*>(context);
    std::string input_line(line);
    
    // Handle empty input or single semicolon
    if (input_line.empty() || input_line == ";") {
        return 1;  // Input is complete
    }
    
    // Add the new line to source
    ctx->source.push_back(input_line);
    
    // Use existing REPL check_need_more_input functionality
    bool needs_more = ctx->repl_instance->check_need_more_input(ctx->source);
    
    ctx->source.pop_back();
    return needs_more;
}

// Add this function to expose version info
const char* nasal_repl_get_version() {
    static std::string version_info = 
        std::string("version ") + __nasver__ + 
        " (" + __DATE__ + " " + __TIME__ + ")";
    return version_info.c_str();
}
