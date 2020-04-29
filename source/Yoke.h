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
            LOGICAL_MINIMUM(2), 0x01, 0x80,   //   -32767
            LOGICAL_MAXIMUM(2), 0xFF, 0x7f,   //   32767
            USAGE(1),           0x30,         //   X
            USAGE(1),           0x31,         //   Y
            USAGE(1),           0x32,         //   Z
            USAGE(1),           0x33,         //   Rx
            USAGE(1),           0x34,         //   Ry
            USAGE(1),           0x35,         //   Rz
            REPORT_SIZE(1),     0x10,         //   16 bits per axis
            REPORT_COUNT(1),    0x06,         //   6 axes
            INPUT(1),           0x02,         //   Data, Variable, Absolute
        END_COLLECTION(0),

        USAGE(1),           0x39,       // hat switch
        LOGICAL_MINIMUM(1), 0x01,       // logical minimum 1
        LOGICAL_MAXIMUM(1), 0x08,       // logical maximum 8
        PHYSICAL_MINIMUM(1),0x00,       // physical minimum 0
        PHYSICAL_MAXIMUM(2),0x3B, 0x01, // physical maximum 315
        UNIT(1),            0x14,       // rotation, angular position
        REPORT_SIZE(1),     0x04,       // 4 bits
        REPORT_COUNT(1),    0x01,       // 1 nibble
        INPUT(1),           0x42,       // data, var, abs, null
        REPORT_SIZE(1),     0x04,       // 4 bits
        REPORT_COUNT(1),    0x01,       // 1 nibble
        INPUT(1),           0x41,       // const, array, abs, null        

        USAGE_PAGE(1),      0x09,       // Buttons
        USAGE_MINIMUM(1),   0x01,       // first button 1
        USAGE_MAXIMUM(1),   0x10,       // last button 16
        LOGICAL_MINIMUM(1), 0x00,       // button off value
        LOGICAL_MAXIMUM(1), 0x01,       // button on value
        REPORT_COUNT(1),    0x10,       // 16 buttons
        REPORT_SIZE(1),     0x01,       // 1 bit per button
        INPUT(1),           0x02,       //   Data, Variable, Absolute
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
    DeviceInformationService deviceInformationService;  // BLE Device Information Service Object
    BatteryService batteryService;      // BLE Battery Service object
    UUID serviceUuid;                   // UUID of the main service
    uint8_t joystickInputReport[15];    // joystick input report to be sent to PC
    bool bleIsConnected{false};         // flag of active BLE connection
};

#endif /* YOKE_H_ */