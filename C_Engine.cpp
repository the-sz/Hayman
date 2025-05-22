#include <_PCH.h>

#include "Hayman.h"

#undef DPrintfCore
#define DPrintfCore					DPrintf_

C_Engine::C_Engine(void)
{
}

void C_Engine::AddressAliasAdd(UINT uiFunction, UINT uiAddress, INT iMin, INT iMax, TCHAR *pszAlias, UINT uiFlags)
{
	C_EngineAddressAlias					*pAlias;

	if ((pAlias = new C_EngineAddressAlias()) == NULL)
		return;

	pAlias->uiFunction = uiFunction;
	pAlias->uiAddress = uiAddress;
	pAlias->iMin = iMin;
	pAlias->iMax = iMax;
	pAlias->Alias = pszAlias;
	pAlias->uiFlags = uiFlags;

	AddressAliasList.AddBottom(pAlias);
}

BOOL C_Engine::AddressAliasGet(UINT uiFunction, UINT uiAddress, _C_String *pAlias, INT *piMin, INT *piMax, UINT *puiFlags)
{
	C_EngineAddressAlias				*pAliasItem;

	pAliasItem = (C_EngineAddressAlias *)AddressAliasList.GetFirstListable();
	while (pAliasItem != NULL)
	{
		if (pAliasItem->uiFunction == uiFunction)
		{
			if (pAliasItem->uiAddress == uiAddress)
			{
				if (pAlias != NULL)
					pAlias->Set(pAliasItem->Alias.Get());
				if (piMin != NULL)
					(*piMin) = pAliasItem->iMin;
				if (piMax != NULL)
					(*piMax) = pAliasItem->iMax;
				if (puiFlags != NULL)
					(*puiFlags) = pAliasItem->uiFlags;

				return TRUE;
			}
		}

		pAliasItem = (C_EngineAddressAlias *)pAliasItem->GetNextListable();
	}

	return FALSE;
}

void C_Engine::AddressAliasSet(UINT uiFunction, UINT uiAddress, TCHAR *pszAlias)
{
	C_EngineAddressAlias				*pAliasItem;

	pAliasItem = (C_EngineAddressAlias *)AddressAliasList.GetFirstListable();
	while (pAliasItem != NULL)
	{
		if (pAliasItem->uiFunction == uiFunction)
		{
			if (pAliasItem->uiAddress == uiAddress)
			{
				pAliasItem->Alias.Set(pszAlias);
				break;
			}
		}

		pAliasItem = (C_EngineAddressAlias *)pAliasItem->GetNextListable();
	}

	if (pAliasItem == NULL)
		AddressAliasAdd(uiFunction, uiAddress, 0, 0, pszAlias, 0);	// iMax = 0 means find auto max value
}

BOOL C_Engine::XMLAddressAliasEnum(_C_XML_ENUM_CALLBACK_TYPE_ENUM Type, void *pvElement, _C_String *pName, _C_String *pText, void *pvContext, _C_XML *pXML)
{
	C_Engine									*pThis;

	UNREF_PARAM(pName)
	UNREF_PARAM(pText)

	pThis = (C_Engine *)pvContext;

	if (Type == ECT_NODE)
	{
		pThis->AddressAliasAdd(	(UINT)pXML->GetValue(_T("Function"), 0, pvElement),
										(UINT)pXML->GetValue(_T("Address"), 0, pvElement),
										(INT)pXML->GetValue(_T("Min"), 0, pvElement),
										(INT)pXML->GetValue(_T("Max"), 0, pvElement),
										pText->Get(),
										(UINT)pXML->GetValue(_T("Flags"), 0, pvElement)
									);
	}

	return FALSE;
}

void C_Engine::Init(void)
{
	_C_XML							XML;
	_C_String						Buffer;

	Hayman.Resources.GetExePath(Hayman.hInstance, Buffer.Get());
	Buffer.Update();
	Buffer.Add(_T("Hayman.xml"));

	if (XML.Init() == TRUE)
	{
		if (XML.LoadFromFile(&Buffer) == TRUE)
		{
			XML.Enum(_T("Hayman\\AddressAlias"), XMLAddressAliasEnum, this);

			if (XML.GetValue(_T("Hayman\\AutoSaved"), FALSE) == FALSE)
				Hayman.uiFlags |= FLAG_SETTINGS_XML_IS_READ_ONLY;
		}

		XML.DeInit();
	}

	ThreadEngine.Init(ThreadEngineCallback, this);
}

void C_Engine::DeInit(void)
{
	_C_XML							XML;
	_C_String						Buffer;
	_C_String						Name;
	_C_String						Value;
	C_EngineAddressAlias			*pAliasItem;
	void								*pvNode;
	void								*pvAliasNode;

	ThreadEngine.DeInit();

	Close();

	// save alias list
	if ((Hayman.uiFlags & FLAG_SETTINGS_XML_IS_READ_ONLY) == 0)
	{
		Hayman.Resources.GetExePath(Hayman.hInstance, Buffer.Get());
		Buffer.Update();
		Buffer.Add(_T("Hayman.xml"));

		if (XML.Init() == TRUE)
		{
			Name = _T("Hayman");
			if (XML.AddNode(NULL, &Name, NULL, &pvNode) == TRUE)
			{
				Name = _T("AddressAlias");
				if (XML.AddNode(pvNode, &Name, NULL, &pvNode) == TRUE)
				{
					pAliasItem = (C_EngineAddressAlias *)AddressAliasList.GetFirstListable();
					while (pAliasItem != NULL)
					{
						Name = _T("Alias");
						if (XML.AddNode(pvNode, &Name, &pAliasItem->Alias, &pvAliasNode) == TRUE)
						{
							Name = _T("Function");
							Value.Printf(_T("%u"), pAliasItem->uiFunction);
							XML.AddAttribute(pvAliasNode, &Name, &Value);

							Name = _T("Address");
							Value.Printf(_T("%u"), pAliasItem->uiAddress);
							XML.AddAttribute(pvAliasNode, &Name, &Value);

							Name = _T("Min");
							Value.Printf(_T("%d"), pAliasItem->iMin);
							XML.AddAttribute(pvAliasNode, &Name, &Value);

							Name = _T("Max");
							Value.Printf(_T("%d"), pAliasItem->iMax);
							XML.AddAttribute(pvAliasNode, &Name, &Value);

							Name = _T("Flags");
							Value.Printf(_T("%u"), pAliasItem->uiFlags);
							XML.AddAttribute(pvAliasNode, &Name, &Value);
						}

						pAliasItem = (C_EngineAddressAlias *)pAliasItem->GetNextListable();
					}
				}
			}

			XML.SetValue(_T("Hayman\\AutoSaved"), TRUE);

			XML.SaveToFile(&Buffer);

			XML.DeInit();
		}
	}

	AddressAliasList.DeleteAllListable();
}

void C_Engine::Close(void)
{
	HART.Close();
}

void C_Engine::SendCommand(WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg)
{
	BOOL						bResult;
	BYTE						bCommand0ReceiveBuffer[100];
	UINT						uiCommand0ReceiveBufferLength;
	UINT						uiRetry;

	bResult = TRUE;

	// open HART
	if (HART.IsOpen() == FALSE)
	{
		if ((bResult = HART.Open(Hayman.SerialPort.Get())) == FALSE)
		{
			DPrintf("C_Engine::SendCommand: Open %s failed.\n", Hayman.SerialPort.Get());
		}

		if (bResult == TRUE)
		{
			if (wCommand != 0)
			{
				// send command 0 to get device address
				for (uiRetry = 0; uiRetry < 3; uiRetry++)
				{
					uiCommand0ReceiveBufferLength = sizeof(bCommand0ReceiveBuffer);
					if (HART.SendCommand(0, NULL, 0, bCommand0ReceiveBuffer, &uiCommand0ReceiveBufferLength) == TRUE)
						break;
				}
			}
		}
	}

	// send command
	if (bResult == TRUE)
		bResult = HART.SendCommand(wCommand, pvAdditionalBuffer, bAdditionalBufferLength, pvReceiveBuffer, puiReceiveBufferLength);

	// send result message
	if (hWnd != NULL)
		PostMessage(hWnd, uiMsg, bResult, 0);
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
