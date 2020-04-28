#ifndef YOKE_H_
#define YOKE_H_

#include "HidService.h"
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/services/DeviceInformationService.h"
#include "ble/services/BatteryService.h"
#include "USBHID_Types.h"

ReportMap joystickReportMap =
{
    USAGE_PAGE(1),      0x01,         // Generic Desktop
    USAGE(1),           0x04,         // Joystick
    COLLECTION(1),      0x01,         // Application
    COLLECTION(1),      0x00,         //  Physical
    USAGE_PAGE(1),      0x09,         //   Buttons
    USAGE_MINIMUM(1),   0x01,
    USAGE_MAXIMUM(1),   0x03,
    LOGICAL_MINIMUM(1), 0x00,
    LOGICAL_MAXIMUM(1), 0x01,
    REPORT_COUNT(1),    0x03,         //   2 bits (Buttons)
    REPORT_SIZE(1),     0x01,
    INPUT(1),           0x02,         //   Data, Variable, Absolute
    REPORT_COUNT(1),    0x01,         //   6 bits (Padding)
    REPORT_SIZE(1),     0x05,
    INPUT(1),           0x01,         //   Constant
    USAGE_PAGE(1),      0x01,         //   Generic Desktop
    USAGE(1),           0x30,         //   X
    USAGE(1),           0x31,         //   Y
    USAGE(1),           0x32,         //   Z
    USAGE(1),           0x33,         //   Rx
    LOGICAL_MINIMUM(1), 0x81,         //   -127
    LOGICAL_MAXIMUM(1), 0x7f,         //   127
    REPORT_SIZE(1),     0x08,         //   Three bytes
    REPORT_COUNT(1),    0x04,
    INPUT(1),           0x02,         //   Data, Variable, Absolute (unlike mouse)
    END_COLLECTION(0),
    END_COLLECTION(0),
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
    DeviceInformationService deviceInformationService;  // BLE Device Information Service Object
    BatteryService batteryService;      // BLE Battery Service object
    uint8_t joystickInputReport[5];     // joystick input report to be sent to PC
    bool bleIsConnected{false};         // flag of active BLE connection
};

#endif /* YOKE_H_ */