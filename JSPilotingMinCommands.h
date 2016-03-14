/*
 * JSPilotingCommands.h
 *
 *  Created on: 8 mars 2016
 *      Author: rbary
 */

#ifndef JSPILOTINGMINCOMMANDS_H_
#define JSPILOTINGMINCOMMANDS_H_

#include <libARSAL/ARSAL.h>
#include <libARController/ARController.h>
#include <libARDiscovery/ARDiscovery.h>

// called to create and init a discovery device; implies a JS discovery device creation with Wifi init
ARDISCOVERY_Device_t * InitDiscoveryDevice();

// called to create a device controller
ARCONTROLLER_Device_t * createDeviceController(ARDISCOVERY_Device_t *discoveryDevice);

// called to add a state change callback to a device controller to be notified when the device controller starts, stops
int addStateChangeCallbackToDeviceController(ARCONTROLLER_Device_t *deviceController, ARCONTROLLER_Device_StateChangedCallback_t *stateChangedCallback);

// called to add a command reception callback to a device controller to be notified when a command has been received from JS
int addCommandReceivedCallbackToDeviceController(ARCONTROLLER_Device_t *deviceController, ARCONTROLLER_DICTIONARY_CALLBACK_t commandReceivedCallback);

// called to start the device controller implies a wifi connection between our system (Unix laptop) and the JS as result
int startDeviceController(ARCONTROLLER_Device_t *deviceController);

// called to stop the device controller implies a wifi disconnection between our system(Unix laptop) and the JS as result
void stopDeviceController(ARCONTROLLER_Device_t *deviceController);

// called to get the state of the JS
eARCONTROLLER_DEVICE_STATE getDeviceState(ARCONTROLLER_Device_t *deviceController,ARSAL_Sem_t stateSem, eARCONTROLLER_DEVICE_STATE deviceState );
// called when a command has been received from the drone
//void commandReceived (eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData);

// called when the state of the device controller has changed
//void stateChanged (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData,ARSAL_Sem_t stateSem);

// called to send a jump command to the JS
int sendJumpCommand(ARCONTROLLER_Device_t *deviceController);

// called to send a forward command to the JS
int sendForwardCommand(ARCONTROLLER_Device_t *deviceController);

// called to send a back command to the JS
int sendBackCommand(ARCONTROLLER_Device_t *deviceController);

// called to send a right roll command to the JS
int sendRightRollCommand(ARCONTROLLER_Device_t *deviceController);

// called to send a left roll command to the JS
int sendLeftRollCommand(ARCONTROLLER_Device_t *deviceController);

#endif /* JSPILOTINGMINCOMMANDS_H_ */
