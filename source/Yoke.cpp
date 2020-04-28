#include "Yoke.h"

Yoke::Yoke(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    led(LED1),
    joystickHID(eventQueue, bleInterface, joystickReportMap, sizeof(joystickReportMap), joystickInputReport, sizeof(joystickInputReport)),
    deviceInformationService(bleInterface, "Marcin", "NYBLE", "00001", "1.0", "1.0", "1.0"),
    batteryService(bleInterface)
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

    UUID service = GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE;

    Gap& gap = bleInterface.gap();
    ble_error_t error = gap.setAdvertisingPayload
    (
        ble::LEGACY_ADVERTISING_HANDLE,
        ble::AdvertisingDataSimpleBuilder<ble::LEGACY_ADVERTISING_MAX_SIZE>()
            .setName("Nucleo Yoke")
            .setAppearance(ble::adv_data_appearance_t::JOYSTICK)
            .setLocalServiceList(mbed::make_Span(&service, 1))
            .setAdvertisingInterval((ble::adv_interval_t)80)    // 80 * 0.625 = 50 ms
            .getAdvertisingData()
    );

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
        joystickHID.sendReport();
    }
}