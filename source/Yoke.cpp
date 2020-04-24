#include "Yoke.h"

Yoke::Yoke(events::EventQueue& eventQueue) :
    eventQueue(eventQueue),
    led(LED1)
{
    // XXX test of event
    eventQueue.call_every(250, callback(this, &Yoke::toggleLed));
}