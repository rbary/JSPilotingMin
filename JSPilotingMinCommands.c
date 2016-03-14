/*
 * JSPilotingMinCommands.c
 *
 *  Created on: 8 mars 2016
 *      Author: rbary
 */

/*****************************************
 *
 *             include file :
 *
 *****************************************/
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <unistd.h>


#include "JSPilotingMinCommands.h"

/*****************************************
 *
 *             define :
 *
 *****************************************/
#define TAG "JSPilotingMinCommands"
#define JS_IP_ADDRESS "192.168.2.1"
#define JS_DISCOVERY_PORT 44444

/*****************************************
 *
 *             private implementation:
 *
 ****************************************/

ARDISCOVERY_Device_t * InitDiscoveryDevice()
{
	eARDISCOVERY_ERROR errorDiscovery = ARDISCOVERY_OK;
	ARDISCOVERY_Device_t *discoveryDevice = NULL;

	discoveryDevice = ARDISCOVERY_Device_New(&errorDiscovery);

	if(errorDiscovery == ARDISCOVERY_OK)
	{
		//create a specific JS discovery device
		errorDiscovery = ARDISCOVERY_Device_InitWifi(discoveryDevice, ARDISCOVERY_PRODUCT_JS,"JS",JS_IP_ADDRESS,JS_DISCOVERY_PORT);

		if(errorDiscovery != ARDISCOVERY_OK)
		{
            ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Discovery error :%s", ARDISCOVERY_Error_ToString(errorDiscovery));
			return NULL;
		}
	}
	else
	{
		 ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Discovery error :%s", ARDISCOVERY_Error_ToString(errorDiscovery));
		 return NULL;
	}

	ARSAL_PRINT (ARSAL_PRINT_INFO, TAG, "Creation of discovery device succeeded");
	return discoveryDevice;
}

ARCONTROLLER_Device_t * createDeviceController(ARDISCOVERY_Device_t *discoveryDevice)
{
    eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
    ARCONTROLLER_Device_t *deviceController = NULL;

    deviceController = ARCONTROLLER_Device_New (discoveryDevice, &errorController);

    if(errorController != ARCONTROLLER_OK)
    {
        ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "Creation of device controller failed.");
    	return NULL;
    }
    ARSAL_PRINT (ARSAL_PRINT_INFO, TAG, "Creation of device controller succeeded.");
    ARDISCOVERY_Device_Delete(&discoveryDevice);
    return deviceController;
}

/*int addStateChangeCallbackToDeviceController(ARCONTROLLER_Device_t *deviceController,ARCONTROLLER_Device_StateChangedCallback_t *stateChanged)
{
	 int failed = 0;
	 eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	 errorController = ARCONTROLLER_Device_AddStateChangedCallback(deviceController, stateChanged, deviceController);

	 if(errorController != ARCONTROLLER_OK)
	 {
         ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "add callback failed.");
         failed = 1;
	 }
	 return failed;
}

int addCommandReceivedCallbackToDeviceController(ARCONTROLLER_Device_t *deviceController,ARCONTROLLER_DICTIONARY_CALLBACK_t commandReceivedCallback)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	errorController = ARCONTROLLER_Device_AddCommandReceivedCallback(deviceController, commandReceivedCallback, deviceController);

	if(errorController != ARCONTROLLER_OK)
		{
	        ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "add callback failed.");
	        failed = 1;
		}
	return failed;
}*/

int startDeviceController(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "Start device controller and try connecting...");
	errorController = ARCONTROLLER_Device_Start(deviceController);

	if(errorController != ARCONTROLLER_OK)
	{
        ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, " start device controller failed ...");
        failed = 1;
	}
	return failed;
}

void stopDeviceController(ARCONTROLLER_Device_t *deviceController)
{
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "disconnecting ...");
	errorController = ARCONTROLLER_Device_Start(deviceController);

	if(errorController != ARCONTROLLER_OK)
	{
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "add callback failed.");
	}
}

eARCONTROLLER_DEVICE_STATE getDeviceState(ARCONTROLLER_Device_t *deviceController,ARSAL_Sem_t stateSem, eARCONTROLLER_DEVICE_STATE deviceState )
{
	// wait state update update
	ARSAL_Sem_Wait (&(stateSem));

	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	deviceState = ARCONTROLLER_Device_GetState(deviceController,&errorController);

	if ((errorController != ARCONTROLLER_OK) || (deviceState != ARCONTROLLER_DEVICE_STATE_RUNNING))
	{
		ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "- deviceState :%d", deviceState);
		ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "- error :%s", ARCONTROLLER_Error_ToString(errorController));
	}
	return deviceState;
}

int sendJumpCommand(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	errorController = deviceController->jumpingSumo->sendAnimationsJump (deviceController->jumpingSumo,ARCOMMANDS_JUMPINGSUMO_ANIMATIONS_JUMP_TYPE_HIGH);

	if(errorController != ARCONTROLLER_OK){
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "send jump command failed ...");
		failed = 1;
	}
	return failed;
}

int sendForwardCommand(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	// set the flag and speed value of the piloting command
	errorController = deviceController->jumpingSumo->setPilotingPCMDFlag (deviceController->jumpingSumo, 1);
	errorController = deviceController->jumpingSumo->setPilotingPCMDSpeed (deviceController->jumpingSumo, 50);

	deviceController->jumpingSumo->

	if(errorController != ARCONTROLLER_OK){
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "send forward command failed ...");
		failed = 1;
	}
	return failed;
}

int sendBackCommand(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	errorController = deviceController->jumpingSumo->setPilotingPCMDFlag (deviceController->jumpingSumo, 1);
	errorController = deviceController->jumpingSumo->setPilotingPCMDSpeed (deviceController->jumpingSumo, -50);

	if(errorController != ARCONTROLLER_OK){
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "send back command failed ...");
		failed = 1;
	}
	return failed;

}

int sendRightRollCommand(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	 errorController = deviceController->jumpingSumo->setPilotingPCMDFlag (deviceController->jumpingSumo, 1);
	 errorController = deviceController->jumpingSumo->setPilotingPCMDTurn (deviceController->jumpingSumo, 50);

	if(errorController != ARCONTROLLER_OK){
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "send right roll command failed");
		failed = 1;
	}
	return failed;
}

int sendLeftRollCommand(ARCONTROLLER_Device_t *deviceController)
{
	int failed = 0;
	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
	 errorController = deviceController->jumpingSumo->setPilotingPCMDFlag (deviceController->jumpingSumo, 1);
	 errorController = deviceController->jumpingSumo->setPilotingPCMDTurn (deviceController->jumpingSumo, -50);

	if(errorController != ARCONTROLLER_OK){
		ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "send left roll command failed");
		failed = 1;
	}
	return failed;
}
