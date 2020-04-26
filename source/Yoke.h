#ifndef YOKE_H_
#define YOKE_H_

#include "HidService.h"
#include <mbed.h>
#include "ble/BLE.h"

class Yoke
{
public:
    Yoke(events::EventQueue& eventQueue, BLE& bleInterface);
    void start(void);
private:
    void toggleLed(void) { led = !led; }
    events::EventQueue& eventQueue;     // event queue of the main thread
    BLE& bleInterface;                  // interface to BLE device
    DigitalOut led;                     // green LED
    HidService joystickHID;             // joystick HID Service object
    //GattServer* pGattServer{nullptr};   // pointer to BLE Gatt server
    //GattService* pHidService{nullptr};  // pointer to HID service of the Gatt server
};

#endif /* YOKE_H_ */