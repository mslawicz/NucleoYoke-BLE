#include "Yoke.h"

Yoke::Yoke(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    led(LED1)
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

    // // get the pointer to BLE Gatt server
    // pGattServer = &bleInterface.gattServer();

    // // register the HID service
    // ble_error_t error = pGattServer->addService(hidService);

    // if (error)
    // {
    //     printf("Error %u during HID service registration\r\n", error);
    //     return;
    // }
    // else
    // {
    //     printf("HID service registered\r\n");
    // }
}