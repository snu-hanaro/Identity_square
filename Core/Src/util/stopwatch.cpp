#include "utils/stopwatch.h"

void stopwatch_millis_t::set() {
    isset = true;
    start_millis = millis();
}

void stopwatch_millis_t::unset() {
    isset = false;
}

bool stopwatch_millis_t::is_set() const {
    return isset;
}

uint32_t stopwatch_millis_t::elapsed() const {
    if (!isset) {
        return 0;
    }
    return millis() - start_millis;
}