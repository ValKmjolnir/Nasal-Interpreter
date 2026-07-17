#pragma once

#include <iostream>

namespace nasal {

std::ostream& clear_screen(std::ostream&);
std::ostream& set_cursor(std::ostream&);
std::ostream& back_white(std::ostream&);
std::ostream& red(std::ostream&);
std::ostream& cyan(std::ostream&);
std::ostream& orange(std::ostream&);
std::ostream& white(std::ostream&);
std::ostream& reset(std::ostream&);

}