# utils.nas
# 2023 by ValKmjolnir
use std.math;

# when count can be divided exactly by times, return true
var times_trigger = func(times, count) {
    return math.mod(times, count)==0;
}