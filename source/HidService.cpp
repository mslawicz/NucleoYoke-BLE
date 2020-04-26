#include "HidService.h"

HidService::HidService(events::EventQueue& eventQueue, BLE& bleInterface) :
    eventQueue(eventQueue),
    bleInterface(bleInterface)
{

}