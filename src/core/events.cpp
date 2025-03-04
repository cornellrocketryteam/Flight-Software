

#include "events.hpp"

void EventBuffer::push(Event event) {
    buffer |= (1u << static_cast<uint8_t>(event));
}

uint32_t EventBuffer::get() {
    return buffer;
}

void EventBuffer::clear() {
    buffer = 0;
}

bool EventBuffer::is_empty() {
    return buffer == 0;
}