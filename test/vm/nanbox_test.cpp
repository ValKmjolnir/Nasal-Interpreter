// NaN boxing representation regression tests for nasal::var.
//
// Build & run (from the repository root):
//   g++ -std=c++17 -I src test/vm/nanbox_test.cpp -o nanbox_test && ./nanbox_test
//
// These tests are header-only: they exercise the inline bit-level API of
// `var` and never link against type.cpp, so any change to the NaN boxing
// representation, the tag layout, or `is_num()` is caught here.

#include "vm/type.hpp"

#include <cmath>
#include <cstdint>
#include <iostream>
#include <limits>

using nasal::var;
using nasal::vm_type;
// u8/u64/f64 are global aliases from util/type_alias.hpp

namespace {

int g_failures = 0;

#define CHECK(cond)                                                          \
    do {                                                                     \
        if (!(cond)) {                                                       \
            std::cerr << "CHECK failed: " << #cond << " (" << __FILE__ << ":" \
                      << __LINE__ << ")\n";                                  \
            ++g_failures;                                                    \
        }                                                                    \
    } while (0)

u64 bits(f64 v) {
    return nasal::bit_cast<u64>(v);
}

// The whole scheme rests on these exact bit constants and on type bits 000
// (vm_none == 0 is reserved for plain NaN) never being used as a tag.
void test_mask_constants() {
    CHECK(QNAN_BASE == 0x7ff8000000000000ULL);
    CHECK(TYPE_MASK == 0x0007000000000000ULL);
    CHECK(PAYLOAD_MASK == 0x0000ffffffffffffULL);
}

void test_vm_type_enum_layout() {
    CHECK(static_cast<u8>(vm_type::vm_none) == 1);
    CHECK(static_cast<u8>(vm_type::vm_addr) == 2);
    CHECK(static_cast<u8>(vm_type::vm_ret) == 3);
    CHECK(static_cast<u8>(vm_type::vm_nil) == 4);
    CHECK(static_cast<u8>(vm_type::vm_num) == 5);
    CHECK(static_cast<u8>(vm_type::vm_gcobj) == 6);
}

// Every finite/infinite double must classify as a number and round-trip.
void test_num_roundtrip() {
    const f64 vals[] = {
        0.0,
        -0.0,
        1.0,
        -1.0,
        3.141592653589793,
        2.718281828459045,
        42.0,
        -123456.789,
        1e308,
        -1e308,
        1e-308,
        1e-320,
        (std::numeric_limits<f64>::min)(),
        (std::numeric_limits<f64>::max)(),
        std::numeric_limits<f64>::lowest(),
        std::numeric_limits<f64>::denorm_min(),
        std::numeric_limits<f64>::epsilon(),
        std::numeric_limits<f64>::infinity(),
        -std::numeric_limits<f64>::infinity(),
    };

    for (f64 v : vals) {
        var x = var::num(v);
        CHECK(x.is_num());
        CHECK(x.type() == vm_type::vm_num);
        CHECK(x.num() == v);
    }
}

// var::num must be a perfect bit round-trip for *all* doubles, including NaN.
void test_num_bit_preservation() {
    const f64 vals[] = {
        0.0,
        -0.0,
        1.0,
        -1.0,
        3.141592653589793,
        1e308,
        1e-308,
        std::numeric_limits<f64>::denorm_min(),
        std::numeric_limits<f64>::infinity(),
        -std::numeric_limits<f64>::infinity(),
        std::nan(""),
        0.0 / 0.0,
        std::numeric_limits<f64>::quiet_NaN(),
    };

    for (f64 v : vals) {
        CHECK(bits(var::num(v).num()) == bits(v));
    }
}

// NaN is a first-class number: it must survive the box and stay a number.
void test_nan_is_first_class_number() {
    const f64 nans[] = {
        std::nan(""),
        0.0 / 0.0,
        std::numeric_limits<f64>::quiet_NaN(),
        std::numeric_limits<f64>::signaling_NaN(),
    };

    for (f64 v : nans) {
        var x = var::num(v);
        CHECK(x.is_num());
        CHECK(x.type() == vm_type::vm_num);
        CHECK(std::isnan(x.num()));
    }

    // canonical quiet NaN from std::nan("") and hardware NaN from 0/0 both
    // have type bits 000; both must classify as numbers.
    CHECK(var::num(std::nan("")).is_num());
    CHECK(var::num(0.0 / 0.0).is_num());
}

// Exercise the two-clause is_num() logic against raw bit patterns.
void test_is_num_bit_boundaries() {
    auto num_from_bits = [](u64 pattern) {
        return var::num(nasal::bit_cast<f64>(pattern));
    };

    // canonical quiet NaN (zero payload)
    CHECK(num_from_bits(0x7ff8000000000000ULL).is_num());
    // negative quiet NaN: sign bit must not affect classification
    CHECK(num_from_bits(0xfff8000000000000ULL).is_num());
    // +/- infinity
    CHECK(num_from_bits(0x7ff0000000000000ULL).is_num());
    CHECK(num_from_bits(0xfff0000000000000ULL).is_num());
    // signaling NaN (quiet bit clear)
    CHECK(num_from_bits(0x7ff0000000000001ULL).is_num());
    // quiet NaN with a payload below the type bits (bits 50-48 still zero)
    CHECK(num_from_bits(0x7ff8000000000001ULL).is_num());

    // A quiet NaN whose payload sets bit 48 (type bits = 001) is bit-wise
    // indistinguishable from a vm_none tag, so by design it is not a number.
    CHECK(!num_from_bits(0x7ff9000000000000ULL).is_num());
    CHECK(num_from_bits(0x7ff9000000000000ULL).type() == vm_type::vm_none);
}

// Tagged values must never be mistaken for numbers.
void test_tagged_types_are_not_numbers() {
    var none = var::none();
    var nil = var::nil();
    var ret = var::ret(0x1234);
    var addr = var::addr(reinterpret_cast<var*>(0x1000));
    var gc = var::gcobj(reinterpret_cast<nasal::nas_val*>(0x2000));

    CHECK(!none.is_num());
    CHECK(!nil.is_num());
    CHECK(!ret.is_num());
    CHECK(!addr.is_num());
    CHECK(!gc.is_num());
}

void test_type_discrimination() {
    CHECK(var::none().type() == vm_type::vm_none);
    CHECK(var::nil().type() == vm_type::vm_nil);
    CHECK(var::ret(0).type() == vm_type::vm_ret);
    CHECK(var::addr(nullptr).type() == vm_type::vm_addr);
    CHECK(var::num(0).type() == vm_type::vm_num);
    CHECK(var::gcobj(nullptr).type() == vm_type::vm_gcobj);
}

// Tagged payloads (ret pc, addr pointer, gcobj pointer) must round-trip.
void test_tagged_payload_roundtrip() {
    u64 pc = 0x0000deadbeefcafe; // 48-bit payload
    var r = var::ret(pc);
    CHECK(r.is_ret());
    CHECK(r.ret() == pc);

    // full 48-bit payload
    u64 full = 0x0000ffffffffffff;
    var rf = var::ret(full);
    CHECK(rf.ret() == full);

    var* target = reinterpret_cast<var*>(0x12345678);
    var a = var::addr(target);
    CHECK(a.is_addr());
    CHECK(a.addr() == target);

    nasal::nas_val* obj = reinterpret_cast<nasal::nas_val*>(0x87654321);
    var g = var::gcobj(obj);
    CHECK(g.is_gcobj());
    CHECK(g.get_gcobj_ptr() == obj);
}

void test_predicates() {
    var none = var::none();
    CHECK(none.is_none());
    CHECK(!none.is_nil());
    CHECK(!none.is_ret());
    CHECK(!none.is_addr());
    CHECK(!none.is_gcobj());

    var nil = var::nil();
    CHECK(!nil.is_none());
    CHECK(nil.is_nil());
    CHECK(!nil.is_ret());
    CHECK(!nil.is_addr());
    CHECK(!nil.is_gcobj());

    var ret = var::ret(5);
    CHECK(!ret.is_none());
    CHECK(!ret.is_nil());
    CHECK(ret.is_ret());
    CHECK(!ret.is_addr());
    CHECK(!ret.is_gcobj());

    var addr = var::addr(reinterpret_cast<var*>(0x8));
    CHECK(addr.is_addr());
    CHECK(!addr.is_none());

    var gc = var::gcobj(reinterpret_cast<nasal::nas_val*>(0x10));
    CHECK(gc.is_gcobj());
    CHECK(!gc.is_none());

    var num = var::num(1.0);
    CHECK(!num.is_none());
    CHECK(!num.is_nil());
    CHECK(!num.is_ret());
    CHECK(!num.is_addr());
    CHECK(!num.is_gcobj());
}

void test_equality() {
    CHECK(var::num(1.5) == var::num(1.5));
    CHECK(var::num(1.5) != var::num(2.5));
    CHECK(var::nil() == var::nil());
    CHECK(var::none() == var::none());
    CHECK(var::ret(7) == var::ret(7));
    CHECK(var::ret(7) != var::ret(8));
    CHECK(var::none() != var::nil());
    CHECK(var::num(0.0) != var::none());
    CHECK(var::nil() != var::num(0.0));
}

} // namespace

int main() {
    test_mask_constants();
    test_vm_type_enum_layout();
    test_num_roundtrip();
    test_num_bit_preservation();
    test_nan_is_first_class_number();
    test_is_num_bit_boundaries();
    test_tagged_types_are_not_numbers();
    test_type_discrimination();
    test_tagged_payload_roundtrip();
    test_predicates();
    test_equality();

    if (g_failures == 0) {
        std::cout << "nanbox_test: all tests passed\n";
        return 0;
    }
    std::cerr << "nanbox_test: " << g_failures << " check(s) failed\n";
    return 1;
}
