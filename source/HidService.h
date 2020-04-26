/* mbed Microcontroller Library
 * Copyright (c) 2017 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 
#ifndef HID_SERVICE_H_
#define HID_SERVICE_H_
 
#include <mbed.h>
#include "ble/BLE.h"

#define HID_VERSION_1_11    0x0111

using ReportMap = const uint8_t[];

struct HidInformation
{
    uint16_t bcdHID;
    uint8_t  bCountryCode;
    uint8_t  flags;
};

enum ProtocolMode
{
    BOOT_PROTOCOL   = 0x0,
    REPORT_PROTOCOL = 0x1
};

class HidService
{
public:
    HidService(events::EventQueue& eventQueue, BLE& bleInterface, ReportMap reportMap, uint16_t reportMapLength);
    void setup(void);
private:
    events::EventQueue& eventQueue;     // event queue of the main thread
    BLE& bleInterface;                  // interface to BLE device
    uint8_t protocolMode{REPORT_PROTOCOL};            // protocol mode boot or report
    uint8_t controlPointCommand;

    HidInformation hidInformation{HID_VERSION_1_11, 0x00, 0x03};

    // Required gatt characteristics: Report Map, Information, Control Point
    GattCharacteristic reportMapCharacteristic;
    ReadOnlyGattCharacteristic<HidInformation> hidInformationCharacteristic;
    GattCharacteristic hidControlPointCharacteristic;

    // Optional gatt characteristics:
    GattCharacteristic protocolModeCharacteristic;
};


#endif /* HID_SERVICE_H_ */