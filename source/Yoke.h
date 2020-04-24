#ifndef YOKE_H_
#define YOKE_H_

#include <mbed.h>

class Yoke
{
public:
    Yoke(events::EventQueue& eventQueue);
private:
    void toggleLed(void) { led = !led; }
    events::EventQueue& eventQueue;
    DigitalOut led;
};

#endif /* YOKE_H_ */