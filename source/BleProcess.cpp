#include "BleProcess.h"

/**
* Construct a BleProcess from an event queue and a ble interface.
*
* Call start() to initiate ble processing.
*/
BleProcess::BleProcess(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface),
    postInitCb(),
    onConnectionChangeCb()
{
    printf("BLE process created\r\n");
}


/**
* Initialize the ble interface, configure it and start advertising.
*/
bool BleProcess::start(void)
{
    printf("BLE process started\r\n");

    if (bleInterface.hasInitialized())
    {
        printf("Error: the ble instance has already been initialized.\r\n");
        return false;
    }

    bleInterface.onEventsToProcess(makeFunctionPointer(this, &BleProcess::scheduleBleEvents));

    ble_error_t error = bleInterface.init(this, &BleProcess::whenInitComplete);

    if (error)
    {
        printf("Error: %u returned by BLE::init.\r\n", error);
        return false;
    }

    bleInterface.securityManager().init();

    return true;
}


/**
* Close existing connections and stop the process.
*/
void BleProcess::stop(void)
{
    if (bleInterface.hasInitialized())
    {
        bleInterface.shutdown();
        printf("BLE process stopped");
    }
}


/**
    * Sets up adverting payload and start advertising.
    *
    * This function is invoked when the ble interface is initialized.
    */
void BleProcess::whenInitComplete(BLE::InitializationCompleteCallbackContext* event)
{
    if (event->error)
    {
        printf("Error %u during the initialization\r\n", event->error);
        return;
    }
    else
    {
        printf("BLE instance initialized\r\n");
        Gap& gap = bleInterface.gap();
        gap.setEventHandler(this);

        if (postInitCb)
        {
            postInitCb();
            startAdvertising();
        }
    }
}

/*
Called when connection attempt ends or an advertising device has been connected
*/
void BleProcess::onConnectionComplete(const ble::ConnectionCompleteEvent &event)
{
    printf("Connected\r\n");
    if(onConnectionChangeCb)
    {
        onConnectionChangeCb(true);
    }
}

/*
Called when a connection has been disconnected
*/
void BleProcess::onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event)
{
    printf("Disconnected\r\n");
    if(onConnectionChangeCb)
    {
        onConnectionChangeCb(false);
    }
    startAdvertising();
}

bool BleProcess::startAdvertising(void)
{
    Gap& gap = bleInterface.gap();

    /* Start advertising the set */
    ble_error_t error = gap.startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error)
    {
        printf("Error %u during gap.startAdvertising\r\n", error);
        return false;
    }
    else
    {
        printf("Advertising started\r\n");
        return true;
    }
}