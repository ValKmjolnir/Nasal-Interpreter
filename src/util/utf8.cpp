#include "util/utf8.hpp"

namespace nasal::utf8 {

u32 utf8_hdchk(const char head) {
    // RFC-2279 but now we use RFC-3629 so nbytes is less than 4
    const auto c = static_cast<u8>(head);
    if ((c >> 5) == 0x06) { // 110x xxxx (10xx xxxx)^1
        return 1;
    }
    if ((c >> 4) == 0x0e) { // 1110 xxxx (10xx xxxx)^2
        return 2;
    }
    if ((c >> 3) == 0x1e) { // 1111 0xxx (10xx xxxx)^3
        return 3;
    }
    return 0;
}

}
