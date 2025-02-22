#include "util.hpp"
#include <string.h>

void EventBuffer::push(Event event) {
    if (!is_full()) {
        buffer[tail] = static_cast<uint8_t>(event);
        tail = (tail + 1) % constants::event_buffer_size;
        count++;
    }
}

uint8_t EventBuffer::pop_id() {
    if (!is_empty()) {
        uint8_t event = buffer[head];
        head = (head + 1) % constants::event_buffer_size;
        count--;
        return event;
    }
    return -1;
}

bool EventBuffer::is_empty() {
    return count == 0;
}

bool EventBuffer::is_full() {
    return count == constants::event_buffer_size;
}

void EventBuffer::clear() {
    memset(buffer, 0, sizeof(buffer));
}