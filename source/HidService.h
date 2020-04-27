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
#include "USBHID_Types.h"

#define BLE_UUID_DESCRIPTOR_REPORT_REFERENCE 0x2908

using ReportMap = const uint8_t[];

struct ReportReference
{
    uint8_t ID;
    uint8_t type;
};

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

enum ReportType
{
    INPUT_REPORT    = 0x1,
    OUTPUT_REPORT   = 0x2,
    FEATURE_REPORT  = 0x3,
};

class HidService
{
public:
    HidService(events::EventQueue& eventQueue, BLE& bleInterface, ReportMap reportMap, uint16_t reportMapLength, uint8_t* inputReport, uint8_t inputReportLength);
    void init(void);
    ble_error_t sendReport(void);       // sends input report to PC
private:
    GattAttribute** inputReportDescriptors();   // Creates the Gatt descriptor for a report characteristic
    GattAttribute** featureReportDescriptors(); // Creates the Gatt descriptor for a report characteristic
    events::EventQueue& eventQueue;     // event queue of the main thread
    BLE& bleInterface;                  // interface to BLE device
    uint8_t* inputReport;               // pointer to input report array
    uint8_t inputReportLength;          // input report length
    uint8_t protocolMode{REPORT_PROTOCOL};            // protocol mode boot or report
    uint8_t controlPointCommand;
    uint8_t featureReport{0};
    ReportReference inputReportReferenceData;
    ReportReference featureReportReferenceData;

    HidInformation hidInformation{HID_VERSION_1_11, 0x00, 0x03};

    GattAttribute inputReportReferenceDescriptor;
    GattAttribute featureReportReferenceDescriptor;

    // Required gatt characteristics: Report Map, Information, Control Point
    GattCharacteristic reportMapCharacteristic;
    ReadOnlyGattCharacteristic<HidInformation> hidInformationCharacteristic;
    GattCharacteristic hidControlPointCharacteristic;

    // Optional gatt characteristics:
    GattCharacteristic protocolModeCharacteristic;

    // Report characteristics
    GattCharacteristic inputReportCharacteristic;
    GattCharacteristic featureReportCharacteristic;
};


#endif /* HID_SERVICE_H_ */