#include "optimizer.h"

void optimizer::const_string(binary_operator* node) {

}

void optimizer::const_number(binary_operator* node) {
    
}

bool optimizer::visit_binary_operator(binary_operator* node) {
    return true;
}

bool optimizer::visit_unary_operator(unary_operator* node) {
    return true;
}

void optimizer::do_optimization(code_block* root) {
    root->accept(this);
}