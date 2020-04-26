#include "HidService.h"

HidService::HidService(events::EventQueue& eventQueue, BLE& bleInterface, ReportMap reportMap, uint16_t reportMapLength) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    hidInformationCharacteristic(GattCharacteristic::UUID_HID_INFORMATION_CHAR, &hidInformation),
    reportMapCharacteristic(GattCharacteristic::UUID_REPORT_MAP_CHAR,
            const_cast<uint8_t*>(reportMap), reportMapLength, reportMapLength,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ),
    protocolModeCharacteristic(GattCharacteristic::UUID_PROTOCOL_MODE_CHAR, &protocolMode, 1, 1,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_READ | GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE),
    hidControlPointCharacteristic(GattCharacteristic::UUID_HID_CONTROL_POINT_CHAR,
            &controlPointCommand, 1, 1,
            GattCharacteristic::BLE_GATT_CHAR_PROPERTIES_WRITE_WITHOUT_RESPONSE)
{

}

/*
setup HID Service
*/
void HidService::setup(void)
{
    printf("HID service setup\r\n");

    static GattCharacteristic* characteristics[] =
    {
        &hidInformationCharacteristic,
        &reportMapCharacteristic,
        &protocolModeCharacteristic,
        &hidControlPointCharacteristic,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    };

    unsigned int charIndex = 4;
    /*
     * Report characteristics are optional, and depend on the reportMap descriptor
     * Note: at least one should be present, but we don't check that at the moment.
     */
    // if (inputReportLength)
    //     characteristics[charIndex++] = &inputReportCharacteristic;
    // if (outputReportLength)
    //     characteristics[charIndex++] = &outputReportCharacteristic;
    // if (featureReportLength)
    //     characteristics[charIndex++] = &featureReportCharacteristic;

    /* TODO: let children add some more characteristics, namely boot keyboard and mouse (They are
     * mandatory as per HIDS spec.) Ex:
     *
     * addExtraCharacteristics(characteristics, int& charIndex);
     */

    GattService service(GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE, characteristics, charIndex);

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