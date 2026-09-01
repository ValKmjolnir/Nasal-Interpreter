#include <cstring>
#include <type_traits>

namespace nasal {

// bit_cast impl from cppreference
template <class To, class From>
To bit_cast(const From& from) noexcept {
    static_assert(sizeof(To) == sizeof(From),
                  "Source and destination types must have the same size");

    static_assert(std::is_trivially_copyable<From>::value,
                  "Source type must be trivially copyable");
    static_assert(std::is_trivially_copyable<To>::value,
                  "Destination type must be trivially copyable");

    To to;
    std::memcpy(&to, &from, sizeof(To));
    return to;
}

}
