#include "Yoke.h"

Yoke::Yoke(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    led(LED1),
    joystickHID(eventQueue, bleInterface, joystickReportMap, sizeof(joystickReportMap), joystickInputReport, sizeof(joystickInputReport)),
    deviceInformationService(bleInterface, "Marcin", "NYBLE", "00001", "1.0", "1.0", "1.0"),
    batteryService(bleInterface),
    serviceUuid(GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE)
{
    printf("Yoke object created\r\n");
}

/*
start the yoke service
this function should be called as a callback on BLE initialization end
*/
void Yoke::start(void)
{
    printf("Yoke started\r\n");

    // setup joystick HID service
    joystickHID.init();

    Gap& gap = bleInterface.gap();

    ble_error_t error = gap.setAdvertisingParameters(ble::LEGACY_ADVERTISING_HANDLE, ble::AdvertisingParameters());

    if (error)
    {
        printf("Gap::setAdvertisingParameters() failed with error %d", error);
        return;
    }

    uint8_t advertisingBuffer[ble::LEGACY_ADVERTISING_MAX_SIZE];
    ble::AdvertisingDataBuilder advertisingDataBuilder(advertisingBuffer);
    advertisingDataBuilder.setFlags();
    advertisingDataBuilder.setName("Nucleo Yoke");
    advertisingDataBuilder.setAppearance(ble::adv_data_appearance_t::JOYSTICK);
    advertisingDataBuilder.setLocalServiceList(mbed::make_Span(&serviceUuid, 1));
    advertisingDataBuilder.setAdvertisingInterval((ble::adv_interval_t)80);    // 80 * 0.625 = 50 ms

    error = gap.setAdvertisingPayload(ble::LEGACY_ADVERTISING_HANDLE, advertisingDataBuilder.getAdvertisingData());

    if (error)
    {
        printf("Gap::setAdvertisingPayload() failed with error %d", error);
        return;
    }

    // XXX eventually this handler should be called on IMU interrupts
    eventQueue.call_every(250, callback(this, &Yoke::handler));
}


/*
* yoke handler should be called periodically
*/
void Yoke::handler(void)
{
    led = !led;

    if(bleIsConnected)
    {
        // XXX test of joystick
        static uint32_t phase = 0;
        int16_t axisValue = 30000 * sin(phase / 10.0);
        joystickInputReport[0] = joystickInputReport[6] = axisValue & 0xFF;
        joystickInputReport[1] = joystickInputReport[7] = (axisValue >> 8) & 0xFF;
        joystickInputReport[12] = phase % 9;
        joystickInputReport[13] = joystickInputReport [14] = (1 << (phase % 16));
        phase++;

        joystickHID.sendReport();
        batteryService.updateBatteryLevel(80 + rand() % 5);
    }
}