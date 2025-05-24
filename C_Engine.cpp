#include <_PCH.h>

#include "Hayman.h"

#undef DPrintfCore
#define DPrintfCore					DPrintf_

C_Engine::C_Engine(void)
{
}

void C_Engine::Init(void)
{
	ThreadEngine.Init(ThreadEngineCallback, this);
}

void C_Engine::DeInit(void)
{
	ThreadEngine.DeInit();

	Close();
}

void C_Engine::Close(void)
{
	HART.Close();
}

void C_Engine::SendCommand(WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg)
{
	HART_RESULT_ENUM		Result;
	BYTE						bCommand0ReceiveBuffer[100];
	UINT						uiCommand0ReceiveBufferLength;
	UINT						uiRetry;

	Result = HR_SUCCESS;

	// open HART
	if (HART.IsOpen() == FALSE)
	{
		if (HART.Open(Hayman.SerialPort.Get()) == FALSE)
		{
			DPrintf("C_Engine::SendCommand: Open %s failed.\n", Hayman.SerialPort.Get());
			(*puiReceiveBufferLength) = 0;
			Result = HR_ERROR_CANT_OPEN_PORT;
		}

		if (Result == HR_SUCCESS)
		{
			if (wCommand != 0)
			{
				// send command 0 to get device address
				for (uiRetry = 0; uiRetry < 3; uiRetry++)
				{
					uiCommand0ReceiveBufferLength = sizeof(bCommand0ReceiveBuffer);
					if (HART.SendCommand(0, NULL, 0, bCommand0ReceiveBuffer, &uiCommand0ReceiveBufferLength) == HR_SUCCESS)
						break;
				}
			}
		}
	}

	// send command
	if (Result == HR_SUCCESS)
		Result = HART.SendCommand(wCommand, pvAdditionalBuffer, bAdditionalBufferLength, pvReceiveBuffer, puiReceiveBufferLength);

	// send result message
	if (hWnd != NULL)
		PostMessage(hWnd, uiMsg, Result, 0);
}

BOOL C_Engine::SendCommandAsync(WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg)
{
	C_EngineRequest					*pEngineRequest;

	if ((pEngineRequest = new C_EngineRequest()) == NULL)
		return FALSE;

	pEngineRequest->Type = ERT_SEND_COMMAND;
	pEngineRequest->wCommand = wCommand;
	memcpy(pEngineRequest->bAdditionalBuffer, pvAdditionalBuffer, bAdditionalBufferLength);
	pEngineRequest->bAdditionalBufferLength = bAdditionalBufferLength;
	pEngineRequest->pvReceiveBuffer = pvReceiveBuffer;
	pEngineRequest->puiReceiveBufferLength = puiReceiveBufferLength;
	pEngineRequest->hWnd = hWnd;
	pEngineRequest->uiMsg = uiMsg;

	if (ThreadEngine.Insert(pEngineRequest) == FALSE)
	{
		delete pEngineRequest;
		return FALSE;
	}

	return TRUE;
}

BOOL C_Engine::ThreadEngineCallback(THREAD_ENGINE_CALLBACK_RESON_ENUM Reason, _C_ThreadEngineRequest *pRequest, void *pvCallbackContext)
{
	C_EngineRequest					*pEngineRequest;
	C_Engine								*pThis;

	pThis = (C_Engine *)pvCallbackContext;

	switch (Reason)
	{
		case TECR_START:
			break;

		case TECR_STOP:
			break;

		case TECR_REQUEST:
			{
				pEngineRequest = (C_EngineRequest *)pRequest;

				DPrintfCore("C_Engine::ThreadEngineCallback: Type: %u.\n", pEngineRequest->Type);
				switch (pEngineRequest->Type)
				{
					case ERT_SEND_COMMAND:
						pThis->SendCommand(pEngineRequest->wCommand, pEngineRequest->bAdditionalBuffer, pEngineRequest->bAdditionalBufferLength, pEngineRequest->pvReceiveBuffer, pEngineRequest->puiReceiveBufferLength, pEngineRequest->hWnd, pEngineRequest->uiMsg);
						break;
				}
			}
			break;
	}

	return TRUE;
}
