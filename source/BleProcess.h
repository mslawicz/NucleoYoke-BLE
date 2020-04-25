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
 
#ifndef BLE_PROCESS_H_
#define BLE_PROCESS_H_
 
#include <mbed.h>
#include "ble/BLE.h"
#include "ble/Gap.h"
#include "ble/GapAdvertisingParams.h"
#include "ble/GapAdvertisingData.h"
#include "ble/FunctionPointerWithContext.h"
 
/**
 * Handle initialization adn shutdown of the BLE Instance.
 *
 * Setup advertising payload and manage advertising state.
 * Delegate to GattClientProcess once the connection is established.
 */
class BleProcess : private mbed::NonCopyable<BleProcess>, public ble::Gap::EventHandler
{
public:
    /**
     * Construct a BleProcess from an event queue and a ble interface.
     *
     * Call start() to initiate ble processing.
     */
    BleProcess(events::EventQueue& eventQueue, BLE& bleInterface) :
        eventQueue(eventQueue),
        bleInterface(bleInterface),
        postInitCb()
    {
    }
 
    ~BleProcess()
    {
        stop();
    }
 
   /**
     * Subscription to the ble interface initialization event.
     *
     * @param[in] cb The callback object that will be called when the ble
     * interface is initialized.
     */
    void onInit(mbed::Callback<void(BLE&, events::EventQueue&)> cb)
    {
        postInitCb = cb;
    }
 
    /**
     * Initialize the ble interface, configure it and start advertising.
     */
    bool start(void);

 
    /**
     * Close existing connections and stop the process.
     */
    void stop(void);
 
private:
 
    /**
     * Schedule processing of events from the BLE middleware in the event queue.
     */
    void schedule_ble_events(BLE::OnEventsToProcessCallbackContext* event)
    {
        eventQueue.call(mbed::callback(&event->ble, &BLE::processEvents));
    }
 
    /**
     * Sets up adverting payload and start advertising.
     *
     * This function is invoked when the ble interface is initialized.
     */
    void when_init_complete(BLE::InitializationCompleteCallbackContext* event);
 
    bool start_advertising(void);
 
    bool set_advertising_parameters()
    {
        Gap& gap = bleInterface.gap();
 
        ble_error_t error = gap.setAdvertisingParameters(
            ble::LEGACY_ADVERTISING_HANDLE,
            ble::AdvertisingParameters()
        );
 
        if (error) {
            printf("Gap::setAdvertisingParameters() failed with error %d", error);
            return false;
        }
 
        return true;
    }
 
    bool set_advertising_data();

    void onConnectionComplete(const ble::ConnectionCompleteEvent &event) override;
    void onDisconnectionComplete(const ble::DisconnectionCompleteEvent &event) override;
 
    events::EventQueue& eventQueue;
    BLE& bleInterface;
    mbed::Callback<void(BLE&, events::EventQueue&)> postInitCb;
};
 
#endif /* BLE_PROCESS_H_ */