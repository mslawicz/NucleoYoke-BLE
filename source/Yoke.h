#ifndef YOKE_H_
#define YOKE_H_

#include <mbed.h>
#include "ble/BLE.h"

class Yoke
{
public:
    Yoke(events::EventQueue& eventQueue, BLE& bleInterface);
    void start(void);
private:
    void toggleLed(void) { led = !led; }
    events::EventQueue& eventQueue;
    BLE& bleInterface;
    DigitalOut led;
};

#endif /* YOKE_H_ */