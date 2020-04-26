#include "Yoke.h"

Yoke::Yoke(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    led(LED1),
    joystickHID(eventQueue, bleInterface, joystickReportMap, sizeof(joystickReportMap), joystickInputReport, sizeof(joystickInputReport))
{
    printf("Yoke object created\r\n");

    //hidService(GattService::UUID_HUMAN_INTERFACE_DEVICE_SERVICE, hidCharacteristics, sizeof(hidCharacteristics) / sizeof(hidCharacteristics[0]))
    //GattCharacteristic** hidCharacteristics;  // list of the characteristics of the HID service
}

/*
start the yoke service
this function should be called as a callback on BLE initialization end
*/
void Yoke::start(void)
{
    printf("Yoke started\r\n");
    // XXX test of event
    eventQueue.call_every(250, callback(this, &Yoke::toggleLed));

    // setup joystick HID service
    joystickHID.setup();
}