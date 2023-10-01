# utils.nas
# 2023 by ValKmjolnir

var times_trigger = func(times, count) {
    return math.mod(times, count)==0;
}