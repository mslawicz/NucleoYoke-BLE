/* mbed Microcontroller Library
 * Copyright (c) 2006-2014 ARM Limited
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

#include "Yoke.h"
#include "BleProcess.h"
#include <mbed.h>
#include "ble/BLE.h"

int main()
{
    printf("Nucleo Yoke BLE v1\r\n");

    // yoke event queue
    events::EventQueue eventQueue;

    // obtain a reference to BLE object that includes the basic attributes of a spec-compatible BLE device
    BLE& bleInterface = BLE::Instance();

    // create Yoke object
    Yoke yoke(eventQueue, bleInterface);

    // Construct a BleProcess from an event queue and a ble interface
    BleProcess bleProcess(eventQueue, bleInterface);

    // Subscription to the ble interface initialization event
    bleProcess.onInit(callback(&yoke, &Yoke::start));

    // bind the event queue to the ble interface, initialize the interface and start advertising
    bleProcess.start();

    // process the event queue
    eventQueue.dispatch_forever();

    return 0;
}
