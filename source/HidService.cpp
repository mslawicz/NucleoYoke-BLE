#include "HidService.h"

HidService::HidService(events::EventQueue& eventQueue, BLE& bleInterface, ReportMap reportMap, uint16_t reportMapLength, uint8_t* inputReport, uint8_t inputReportLength) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    inputReport(inputReport),
    inputReportLength(inputReportLength),
    inputReportReferenceDescriptor(BLE_UUID_DESCRIPTOR_REPORT_REFERENCE, (uint8_t*)&inputReportReferenceData, 2, 2),
    hidInformationCharacteristic(GattCharacteristic::UUID_HID_INFORMATION_CHAR, &hidInformation),
    reportMapCharacteristic(GattCharacteristic::UUID_REPORT_MAP_CHAR,
            const_cast<uint8_t*>(reportMap), reportMapLength, reportMapLength,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    protocolModeCharacteristic(GattCharacteristic::UUID_PROTOCOL_MODE_CHAR, &protocolMode, 1, 1,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
    hidControlPointCharacteristic(GattCharacteristic::UUID_HID_CONTROL_POINT_CHAR,
            &controlPointCommand, 1, 1,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
    inputReportCharacteristic(GattCharacteristic::UUID_REPORT_CHAR,
            inputReport, inputReportLength, inputReportLength,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_NOTIFY | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE,
            inputReportDescriptors(), 1)
{

}

/*
initialize HID Service
*/
void HidService::init(void)
{
    printf("HID service initializing\r\n");

    static GattCharacteristic* characteristics[] =
    {
        &hidInformationCharacteristic,
        &reportMapCharacteristic,
        &protocolModeCharacteristic,
        &hidControlPointCharacteristic,
        &inputReportCharacteristic
    };

    GattService service(GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE, characteristics, sizeof(characteristics) / sizeof(characteristics[0]));

    ble_error_t error = bleInterface.gattServer().addService(service);

    if (error)
    {
        printf("Error %u during HID service registration\r\n", error);
        return;
    }
    else
    {
        printf("HID service registered\r\n");
    }
}


GattAttribute** HidService::inputReportDescriptors()
{
    inputReportReferenceData.ID = 0;
    inputReportReferenceData.type = ReportType::INPUT_REPORT;

    static GattAttribute* descs[] = { &inputReportReferenceDescriptor };
    return descs;
}

/*
* sends input report to PC
*/
ble_error_t HidService::sendReport(void)
{
    return bleInterface.gattServer().write(inputReportCharacteristic.getValueHandle(), inputReport, inputReportLength);
}