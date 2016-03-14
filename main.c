/*
 * main.c
 *
 *  Created on: 8 mars 2016
 *      Author: rbary
 */
#include<stdio.h>
#include<stdlib.h>

#include "JSPilotingMinCommands.h"
#define TAG "JSPilotingMinCommands"


ARSAL_Sem_t stateSem;

void stateChanged (eARCONTROLLER_DEVICE_STATE newState, eARCONTROLLER_ERROR error, void *customData)
{
	ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "    - stateChanged newState: %d .....", newState);

	switch (newState)
	{
		case ARCONTROLLER_DEVICE_STATE_STOPPED:
			ARSAL_Sem_Post (&(stateSem));
			break;

		case ARCONTROLLER_DEVICE_STATE_RUNNING:
			ARSAL_Sem_Post (&(stateSem));
			break;

		default:
			break;
	}
}

void commandReceived (eARCONTROLLER_DICTIONARY_KEY commandKey, ARCONTROLLER_DICTIONARY_ELEMENT_t *elementDictionary, void *customData)
{
	ARCONTROLLER_Device_t *deviceController = customData;

	if (deviceController != NULL)
	{
		// if the command received is a battery state changed
		if (commandKey == ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED)
		{
			ARCONTROLLER_DICTIONARY_ARG_t *arg = NULL;
			ARCONTROLLER_DICTIONARY_ELEMENT_t *singleElement = NULL;

			if (elementDictionary != NULL)
			{
				// get the command received in the device controller
				HASH_FIND_STR (elementDictionary, ARCONTROLLER_DICTIONARY_SINGLE_KEY, singleElement);

				if (singleElement != NULL)
				{
					// get the value
					HASH_FIND_STR (singleElement->arguments, ARCONTROLLER_DICTIONARY_KEY_COMMON_COMMONSTATE_BATTERYSTATECHANGED_PERCENT, arg);

					if (arg != NULL)
					{
						ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "command received :%d",arg->value.U8);
					}
					else
					{
						ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "arg is NULL");
					}
				}
				else
				{
					ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "singleElement is NULL");
				}
			}
			else
			{
				ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "elements is NULL");
			}
		}
	}
}



int main(int argc, char **argv) {
	//local declarations
	int error = 0;

    ARSAL_Sem_Init (&(stateSem), 0, 0);
    eARCONTROLLER_DEVICE_STATE deviceState = ARCONTROLLER_DEVICE_STATE_MAX;
    ARDISCOVERY_Device_t *dd = NULL;
    ARCONTROLLER_Device_t *dc = NULL;

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- Jumping Sumo Piloting Min--");


    if(!error){
    	dd = InitDiscoveryDevice();
    	if(dd == NULL){error = 1;}
    }

    if(!error){
    	dc = createDeviceController(dd);
    	if(dc == NULL){error = 1;}
    }

    //if(!error){error = addCommandReceivedCallbackToDeviceController(dc,commandReceived);}
    //if(!error){error = addStateChangeCallbackToDeviceController(dc, stateChanged);}

    if(!error){
    	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
    	errorController = ARCONTROLLER_Device_AddStateChangedCallback (dc, stateChanged, dc);

		if (errorController != ARCONTROLLER_OK)
		{
			ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "add State callback failed.");
			error = 1;
		}
    }

    if(!error)
    {
       	eARCONTROLLER_ERROR errorController = ARCONTROLLER_OK;
       	errorController = ARCONTROLLER_Device_AddCommandReceivedCallback(dc, commandReceived, dc);

   		if (errorController != ARCONTROLLER_OK)
   		{
   			ARSAL_PRINT (ARSAL_PRINT_ERROR, TAG, "add State callback failed.");
   			error = 1;
   		}
    }

    if(!error){
    	error = startDeviceController(dc);
    }

    // move forward
    if(!error){error = sendForwardCommand(dc);}
    sleep(1);
    // turn right
    if(!error){error = sendRightRollCommand(dc);}
    sleep(1);

    // move forward
    if(!error){error = sendForwardCommand(dc);}
    sleep(1);

    // jump
    if(!error){error = sendJumpCommand(dc);}
    sleep(1);


    stopDeviceController(dc);

    ARSAL_PRINT(ARSAL_PRINT_INFO, TAG, "-- END --");
    return EXIT_SUCCESS;
}



