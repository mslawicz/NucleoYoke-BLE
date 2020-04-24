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
#include <mbed.h>
#include "ble/BLE.h"

static DigitalOut systemLED(LED1);

// XXX blink function for test
void blinkLed(void)
{
    systemLED = !systemLED;
}

int main()
{
    printf("Nucleo Yoke BLE v1\r\n");

    // yoke event queue
    events::EventQueue eventQueue;

    // create Yoke object
    Yoke yoke;

    // obtain a reference to BLE object that includes the basic attributes of a spec-compatible BLE device
    BLE& bleInterface = BLE::Instance();

    // XXX test of event
    eventQueue.call_every(250, blinkLed);

    // process the event queue
    eventQueue.dispatch_forever();

    return 0;
}
