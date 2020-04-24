#include "BleProcess.h"

/**
* Initialize the ble interface, configure it and start advertising.
*/
bool BleProcess::start(void)
{
    printf("Ble process started.\r\n");

    if (bleInterface.hasInitialized())
    {
        printf("Error: the ble instance has already been initialized.\r\n");
        return false;
    }

    bleInterface.onEventsToProcess(makeFunctionPointer(this, &BleProcess::schedule_ble_events));

    ble_error_t error = bleInterface.init(this, &BleProcess::when_init_complete);

    if (error)
    {
        printf("Error: %u returned by BLE::init.\r\n", error);
        return false;
    }

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
        printf("Ble process stopped.");
    }
}


/**
    * Sets up adverting payload and start advertising.
    *
    * This function is invoked when the ble interface is initialized.
    */
void BleProcess::when_init_complete(BLE::InitializationCompleteCallbackContext* event)
{
    if (event->error)
    {
        printf("Error %u during the initialization\r\n", event->error);
        return;
    }
    printf("Ble instance initialized\r\n");

    Gap& gap = bleInterface.gap();
    gap.onConnection(this, &BleProcess::when_connection);
    gap.onDisconnection(this, &BleProcess::when_disconnection);

    if (!set_advertising_parameters())
    {
        return;
    }

    if (!set_advertising_data())
    {
        return;
    }

    if (!start_advertising())
    {
        return;
    }

    if (postInitCb)
    {
        postInitCb(bleInterface, eventQueue);
    }
}


void BleProcess::when_disconnection(const Gap::DisconnectionCallbackParams_t* event)
{
    printf("Disconnected.\r\n");
    start_advertising();
}


bool BleProcess::start_advertising(void)
{
    Gap& gap = bleInterface.gap();

    /* Start advertising the set */
    ble_error_t error = gap.startAdvertising(ble::LEGACY_ADVERTISING_HANDLE);

    if (error)
    {
        printf("Error %u during gap.startAdvertising.\r\n", error);
        return false;
    }
    else
    {
        printf("Advertising started.\r\n");
        return true;
    }
}


bool BleProcess::set_advertising_data()
{
    Gap& gap = bleInterface.gap();

    /* Use the simple builder to construct the payload; it fails at runtime
        * if there is not enough space left in the buffer */
    ble_error_t error = gap.setAdvertisingPayload(
        ble::LEGACY_ADVERTISING_HANDLE,
        ble::AdvertisingDataSimpleBuilder<ble::LEGACY_ADVERTISING_MAX_SIZE>()
            .setFlags()
            .setName("GattServer")
            .getAdvertisingData()
    );

    if (error)
    {
        printf("Gap::setAdvertisingPayload() failed with error %d", error);
        return false;
    }

    return true;
}