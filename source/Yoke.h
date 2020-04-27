#ifndef YOKE_H_
#define YOKE_H_

#include "HidService.h"
#include <mbed.h>
#include "ble/BLE.h"
#include "USBHID_Types.h"

ReportMap joystickReportMap =
{
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x02,         // Mouse
    COLLECTION(1),      0x01,         // Application
    USAGE(1),           0x01,         //  Pointer
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   3 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   5 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x38,         //   Wheel
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x03,
    INPUT(1),           0x06,         //   Data, Variable, Relative
    END_COLLECTION(0),
    END_COLLECTION(0)
};

class Yoke
{
public:
    Yoke(events::EventQueue& eventQueue, BLE& bleInterface);
    void start(void);
    void setConnectionFlag(bool connected) { bleIsConnected = connected; }
private:
    void handler(void);
    events::EventQueue& eventQueue;     // event queue of the main thread
    BLE& bleInterface;                  // interface to BLE device
    DigitalOut led;                     // green LED
    HidService joystickHID;             // joystick HID Service object
    uint8_t joystickInputReport[4];     // joystick input report to be sent to PC
    bool bleIsConnected{false};         // flag of active BLE connection
};

#endif /* YOKE_H_ */