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

#include <mbed.h>
#include "ble/BLE.h"

static DigitalOut systemLED(LED1);

int main()
{
    const uint32_t BlinkPeriod = 500;   // LED blink period [ms] 
    printf("Nucleo Yoke BLE v1\r\n");
    BLE& ble = BLE::Instance();

    while(1)
    {
        systemLED = !systemLED;
        ThisThread::sleep_for(BlinkPeriod);
    }
}
