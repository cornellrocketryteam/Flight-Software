
#ifndef UTIL_HPP
#define UTIL_HPP

#include "constants.hpp"
#include "types.hpp"

#ifdef VERBOSE
#define logf(...) printf(__VA_ARGS__)
#define SPACER ("                        ")
#else
#define logf(...)
#endif

class EventBuffer {
public:
    void push(Event event);
    uint8_t pop_id();
    bool is_empty();
    bool is_full();
    void clear();

private:
    uint8_t buffer[constants::event_buffer_size];
    int head = 0, tail = 0, count = 0;
};

#endif // UTIL_HPP