#include <_PCH.h>

#include "Hayman.h"

#undef DPrintfCore
#define DPrintfCore					DPrintf_

_C_Debug									Debug;
C_Hayman									Hayman;

void C_Hayman::SendCommand(void)
{
	_C_String							Buffer;
	_C_String							Part;
	UINT									uiCommand;
	BYTE									bAdditionalBufferLength;
	BYTE									bAdditionalBuffer[HART_MAX_REQUEST_BYTES];
	UINT									uiByte;

	for (;;)
	{
		// stop update timer
		KillTimer(hWnd, TIMER_ID_REFRESH);

		//
		// get values from controls
		//
		ComboXCommand.GetText(Buffer.Get());
		Buffer.Update();
		_stscanf(Buffer.Get(), _T("%u"), &uiCommand);

		ComboXPayload.GetText(Buffer.Get());
		Buffer.Update();
		if (Buffer.GetLength() > 0)
			MRUPayload.Add(Buffer.Get());
		bAdditionalBufferLength = 0;
		while (Buffer.StrTokEx(&Part, _T(" ")) == TRUE)
		{
			_stscanf(Part.Get(), _T("%x"), &uiByte);
			bAdditionalBuffer[bAdditionalBufferLength] = LO_BYTE(LO_WORD(uiByte));
			bAdditionalBufferLength++;
		}

		//
		// send command async
		//
		uiReceiveBufferLength = sizeof(bReceiveBuffer);
		if (Engine.SendCommandAsync((WORD)uiCommand, bAdditionalBuffer, bAdditionalBufferLength, bReceiveBuffer, &uiReceiveBufferLength, hWnd, USR_UPDATE_RESPONSE) == FALSE)
			break;

		return;
	}
}

FLOAT C_Hayman::DecodeFloat(BYTE *pbBuffer)
{
	BYTE									bArray[4];

	bArray[0] = pbBuffer[3];
	bArray[1] = pbBuffer[2];
	bArray[2] = pbBuffer[1];
	bArray[3] = pbBuffer[0];

	return *(FLOAT *)&bArray;
}

void C_Hayman::DecodeManufacturerAndDevice(WORD wManufacturer, WORD wDeviceType, _C_String *pManufacturer, _C_String *pDeviceType)
{
	TCHAR				*pszManufacturer;
	TCHAR				*pszDeviceType;

	pszManufacturer = NULL;
	pszDeviceType = NULL;

	switch (wManufacturer)
	{
		case 0x6008:
			pszManufacturer = _T("MSA");
			switch (wDeviceType)
			{
				case 0xE0F3:			pszDeviceType = _T("PrimaX");					break;
				case 0xE0F6:			pszDeviceType = _T("PrimaXIR");				break;
				case 0xE08C:			pszDeviceType = _T("UltimaXL/XT");			break;
				case 0xE09F:			pszDeviceType = _T("UltimaXEH");				break;
				case 0xE0BC:			pszDeviceType = _T("Flamegard");				break;
			}
			break;

		case 0x6067:
			pszManufacturer = _T("Senscient");
			switch (wDeviceType)
			{
				case 0xE1D0:			pszDeviceType = _T("ELDS");					break;
			}
			break;
	}

	if (pszManufacturer != NULL)
		pManufacturer->Printf(_T("0x%04X - %s"), wManufacturer, pszManufacturer);
	else
		pManufacturer->Printf(_T("0x%04X"), wManufacturer);

	if (pszDeviceType != NULL)
		pDeviceType->Printf(_T("0x%04X - %s"), wDeviceType, pszDeviceType);
	else
		pDeviceType->Printf(_T("0x%04X"), wManufacturer);
}

void C_Hayman::DecodeCommand(WORD wCommand, _C_String *pCommand)
{
	TCHAR				*pszCommand;

	pszCommand = NULL;

	switch (wCommand)
	{
		case 0:		pszCommand = _T("Read Transmitter Unique Identifier");					break;
		case 1:		pszCommand = _T("Read Primary Variable");										break;
		case 2:		pszCommand = _T("Read Current and Percent of Range");						break;
		case 3:		pszCommand = _T("Read all dynamic Variables and Current");				break;
		case 6:		pszCommand = _T("Write Polling Address");										break;
		case 11:		pszCommand = _T("Read Unique Identifier Associated With Tag");			break;
		case 12:		pszCommand = _T("Read Message");													break;
		case 13:		pszCommand = _T("Read Tag, Descriptor, Date");								break;
		case 14:		pszCommand = _T("Read Primary Variable Sensor Information");			break;
		case 15:		pszCommand = _T("Read Primary Variable Output Information");			break;
		case 16:		pszCommand = _T("Read Final Assembly Number");								break;
		case 17:		pszCommand = _T("Write Message");												break;
		case 18:		pszCommand = _T("Write Tag, Descriptor, Date");								break;
		case 19:		pszCommand = _T("Write Final Assembly Number");								break;
	}

	if (pszCommand != NULL)
		pCommand->Printf(_T("%u - %s"), wCommand, pszCommand);
	else
		pCommand->Printf(_T("%u"), wCommand);
}

// Spec307.pdf page 17
void C_Hayman::DecodeResponseCode(BYTE bResponseCode, _C_String *pResponseCode)
{
	TCHAR				*pszResponseCode;

	pszResponseCode = NULL;

	switch (bResponseCode)
	{
		case 0:		pszResponseCode = _T("Success");													break;
		case 5:		pszResponseCode = _T("Too Few Data Bytes Received");						break;
		case 16:		pszResponseCode = _T("Access Restricted");									break;
	}

	if (pszResponseCode != NULL)
		pResponseCode->Printf(_T("%u - %s"), bResponseCode, pszResponseCode);
	else
		pResponseCode->Printf(_T("%u"), bResponseCode);
}

// Spec127.pdf page 12
void C_Hayman::DecodePayloadCommand0(UINT uiOffset)
{
	_C_String							Buffer;
	_C_String							Temp;
	WORD									wManufacturer;
	WORD									wDeviceType;
	BYTE									bDeviceStatus;

	wManufacturer = MAKE_WORD(bReceiveBuffer[uiOffset + 18], bReceiveBuffer[uiOffset + 17]);
	wDeviceType = MAKE_WORD(bReceiveBuffer[uiOffset + 2], bReceiveBuffer[uiOffset + 1]);
	DecodeManufacturerAndDevice(wManufacturer, wDeviceType, &Buffer, &Temp);
	ListViewAdd(_T("Manufacturer"), Buffer.Get(), 1);
	ListViewAdd(_T("Device Type"), Temp.Get(), 1);

	Buffer.Printf(_T("0x%02X 0x%02X 0x%02X"), bReceiveBuffer[uiOffset + 9], bReceiveBuffer[uiOffset + 10], bReceiveBuffer[uiOffset + 11]);
	ListViewAdd(_T("Device ID"), Buffer.Get(), 1);
	
	bDeviceStatus = bReceiveBuffer[uiOffset + 16];
	Buffer.Printf(_T("0x%02X"), bDeviceStatus);
	Temp = _T("");
	// Spec183.pdf page 82
	if ((bDeviceStatus & 0x01) != 0)
		Temp.AddWithDelimiter(_T("Maintenance Required"), _T(" "));
	if ((bDeviceStatus & 0x02) != 0)
		Temp.AddWithDelimiter(_T("Device Variable Alert"), _T(" "));
	if ((bDeviceStatus & 0x04) != 0)
		Temp.AddWithDelimiter(_T("Critical Power Failure"), _T(" "));
	if ((bDeviceStatus & 0x08) != 0)
		Temp.AddWithDelimiter(_T("Failure"), _T(" "));
	if ((bDeviceStatus & 0x10) != 0)
		Temp.AddWithDelimiter(_T("Out of Specification"), _T(" "));
	if ((bDeviceStatus & 0x20) != 0)
		Temp.AddWithDelimiter(_T("Function Check"), _T(" "));
	Buffer.AddWithDelimiter(Temp.Get(), _T(" - "));
	ListViewAdd(_T("Device Status"), Buffer.Get(), 1);

	Buffer.Printf(_T("0x%02X"), bReceiveBuffer[uiOffset + 4]);
	ListViewAdd(_T("HART Revision"), Buffer.Get(), 1);

	Buffer.Printf(_T("0x%02X"), bReceiveBuffer[uiOffset + 5]);
	ListViewAdd(_T("Device Revision"), Buffer.Get(), 1);

	Buffer.Printf(_T("0x%02X"), bReceiveBuffer[uiOffset + 6]);
	ListViewAdd(_T("Software Revision"), Buffer.Get(), 1);

	Buffer.Printf(_T("0x%02X"), bReceiveBuffer[uiOffset + 7]);
	ListViewAdd(_T("Hardware Revision"), Buffer.Get(), 1);
	
	Buffer.Printf(_T("%u"), bReceiveBuffer[uiOffset + 13]);
	ListViewAdd(_T("Device Variables"), Buffer.Get(), 1);

	Buffer.Printf(_T("%u"), bReceiveBuffer[uiOffset + 3]);
	ListViewAdd(_T("Minimum Numbers of Preambles"), Buffer.Get(), 1);
}

// Spec127.pdf page 15
void C_Hayman::DecodePayloadCommand2(UINT uiOffset)
{
	_C_String							Buffer;

	Buffer.Printf(_T("%.1f mA"), DecodeFloat(&bReceiveBuffer[uiOffset + 0]));
	ListViewAdd(_T("Current"), Buffer.Get(), 1);

	Buffer.Printf(_T("%.1f %%"), DecodeFloat(&bReceiveBuffer[uiOffset + 4]));
	ListViewAdd(_T("Percent of Range"), Buffer.Get(), 1);
}

void C_Hayman::UpdateResponse(BOOL bResult)
{
	_C_String							Buffer;
	_C_String							Temp;
	UINT									uiIndex;
	INT									iSelectedItem;
	UINT									uiOffset;
	BYTE									bCommand;
	BYTE									bFieldDeviceStatus;

	// get currently selected item
	ListView.GetSelectedItem(&iSelectedItem, NULL, NULL);

	// disable ui
	ListView.Update(FALSE);
	// reset everything
	ListView.DeleteAllItems();

	Buffer = _T("");
	for (uiIndex = 0; uiIndex < uiReceiveBufferLength; uiIndex++)
	{
		if (((uiIndex % 8) == 0) && (uiIndex != 0))
			Buffer.Add(_T(" |"));

		Temp.Printf(_T("0x%02X"), bReceiveBuffer[uiIndex]);
		Buffer.AddWithDelimiter(Temp.Get(), _T(" "));
	}

	ListViewAdd(_T("Received"), Buffer.Get(), ((bResult == TRUE) ? 0 : 2));

	if (bResult == TRUE)
	{
		uiOffset = 0;
		if (bReceiveBuffer[0] == HART_DELIMITER_FIELD_SACK)
		{
			Buffer.Printf(_T("0x%02X"), bReceiveBuffer[1]);
			ListViewAdd(_T("Short Address"), Buffer.Get(), 0);
			uiOffset = 2;
		}
		else if (bReceiveBuffer[0] == HART_DELIMITER_FIELD_LACK)
		{
			Buffer.Printf(_T("0x%02X 0x%02X 0x%02X 0x%02X 0x%02X"), bReceiveBuffer[1], bReceiveBuffer[2], bReceiveBuffer[3], bReceiveBuffer[4], bReceiveBuffer[5]);
			ListViewAdd(_T("Long Address"), Buffer.Get(), 0);
			uiOffset = 6;
		}
		else
		{
			Buffer.Printf(_T("Unknown Delimiter 0x%02X"), bReceiveBuffer[0]);
			ListViewAdd(_T("Delimiter"), Buffer.Get(), 2);
			uiOffset = 6;
		}

		bCommand = bReceiveBuffer[uiOffset];
		DecodeCommand(bCommand, &Buffer);
		ListViewAdd(_T("Command"), Buffer.Get(), 0);
		uiOffset++;

		Buffer.Printf(_T("%u"), bReceiveBuffer[uiOffset]);
		ListViewAdd(_T("Payload Length"), Buffer.Get(), 0);
		uiOffset++;

		Buffer.Printf(_T("0x%02X"), bReceiveBuffer[uiReceiveBufferLength - 1]);
		ListViewAdd(_T("Checksum"), Buffer.Get(), 0);

		// The first byte is multiplexed and contains either the Communication Status (most significant bit is set) or the Response Code (most significant bit is reset)
		DecodeResponseCode(bReceiveBuffer[uiOffset], &Buffer);
		ListViewAdd(_T("Response Code"), Buffer.Get(), 0);
		uiOffset++;

		// The second byte of a slave response message always contains Field Device Status.
		// Spec099.pdf page 33
		bFieldDeviceStatus = bReceiveBuffer[uiOffset];
		Buffer.Printf(_T("0x%02X"), bFieldDeviceStatus);
		Temp = _T("");
		if ((bFieldDeviceStatus & 0x01) != 0)
			Temp.AddWithDelimiter(_T("Primary Variable Out of Limits"), _T(" "));
		if ((bFieldDeviceStatus & 0x02) != 0)
			Temp.AddWithDelimiter(_T("Non-Primary Variable Out of Limits"), _T(" "));
		if ((bFieldDeviceStatus & 0x04) != 0)
			Temp.AddWithDelimiter(_T("Loop Current Saturated"), _T(" "));
		if ((bFieldDeviceStatus & 0x08) != 0)
			Temp.AddWithDelimiter(_T("Loop Current Fixed"), _T(" "));
		if ((bFieldDeviceStatus & 0x10) != 0)
			Temp.AddWithDelimiter(_T("More Status Available"), _T(" "));
		if ((bFieldDeviceStatus & 0x20) != 0)
			Temp.AddWithDelimiter(_T("Cold Start"), _T(" "));
		if ((bFieldDeviceStatus & 0x40) != 0)
			Temp.AddWithDelimiter(_T("Configuration Changed"), _T(" "));
		if ((bFieldDeviceStatus & 0x80) != 0)
			Temp.AddWithDelimiter(_T("Device Malfunction"), _T(" "));
		Buffer.AddWithDelimiter(Temp.Get(), _T(" - "));
		ListViewAdd(_T("Field Device Status"), Buffer.Get(), 0);
		uiOffset++;

		switch (bCommand)
		{
			case 0:
				DecodePayloadCommand0(uiOffset);
				break;

			case 2:
				DecodePayloadCommand2(uiOffset);
				break;
		}
	}

  ListView.SetFocus(iSelectedItem, TRUE);

	// enable ui
	ListView.Update(TRUE);

//xxx better erorr code?
	if (bResult == FALSE)
		ShowError(IDS_COMMAND_FAILED);
	else
		ShowStatistic();

	if (bPolling == TRUE)
		SetTimer(hWnd, TIMER_ID_REFRESH, uiRefresh, NULL);	// re-arm update timer
}

void C_Hayman::ShowStatistic(void)
{
	_SetItemText(IDC_INFO_TEXT, StringTable.FormatStringEx(IDS_STATISTIC, Engine.HART.Statistic.uiSuccessCounter, Engine.HART.Statistic.uiFailureCounter));

	_ShowItem(IDC_ERROR_ICON, FALSE);
	_ShowItem(IDC_INFO_ICON, TRUE);
}

void C_Hayman::ShowError(INT iResourceId)
{
	_SetItemText(IDC_INFO_TEXT, StringTable.GetStringEx(iResourceId));

	_ShowItem(IDC_ERROR_ICON, TRUE);
	_ShowItem(IDC_INFO_ICON, FALSE);
}

void C_Hayman::ListViewAdd(TCHAR *pszName, TCHAR *pszValue, INT iImage)
{
	INT									iItem;
	_C_Time								Time;

	iItem = ListView.AddItem(pszName, iImage, 0);
	ListView.SetSubItemText(pszValue, 1, iItem);

	Time.TakeTime();
	Log.Write(_T("%u.%u.%u;%u:%02u:%02u;%s;%s"),	Time.SavedTime.wDay, Time.SavedTime.wMonth, Time.SavedTime.wYear, Time.SavedTime.wHour, Time.SavedTime.wMinute, Time.SavedTime.wSecond,
				 pszName,
				 pszValue
			 );
}

void C_Hayman::Save(void)
{
	_C_Files								Files;
	_C_String							FileName;
	_C_String							Buffer;
	_C_String							Name;
	_C_String							Value;
	INT									iItem;
	_C_XML								XML;
	void									*pvNode;
	void									*pvSubNode;
	void									*pvSubSubNode;
	DWORD									dwFilterIndex;

	for (;;)
	{
		pvNode = NULL;
		pvSubNode = NULL;

		FileName = _T("Values");
		Buffer = StringTable.GetStringEx(IDS_FILTER_SAVE);
		dwFilterIndex = 1;
		if (Files.SelectFile(hWnd, Buffer.Get(), OFN_OWN_CSIDL_NO_DIRECTORY, StringTable.GetStringEx(IDS_SELECT_CURRENT_VALUES), OFN_OWN_FILE_SAVE_DIALOG, FileName.Get(), FileName.GetMaxLength(), &dwFilterIndex, _T("csv")) == FALSE)
			break;
		FileName.Update();

		if (dwFilterIndex == 1)
		{
			if (Files.FileOpen(FileName.Get(), FA_WRITE_NEW) == FALSE)
				break;

			Buffer.Printf(_T("Name;Value%s"), _T(_C_STRING_NEW_LINE));
			Files.FileWrite(Buffer.GetLength(), Buffer.GetAnsi());

			iItem = 0;
			// get name
			while (ListView.GetItem(iItem, Name.Get(), NULL, 0, Name.GetMaxLength()) == TRUE)
			{
				Name.Update();

				// get value
				if (ListView.GetItem(iItem, Value.Get(), NULL, 1, Value.GetMaxLength()) == FALSE)
					break;
				Value.Update();

				Buffer.Printf(_T("%s;%s%s"), Name.Get(), Value.Get(), _T(_C_STRING_NEW_LINE));
				Files.FileWrite(Buffer.GetLength(), Buffer.GetAnsi());

				iItem++;
			}
		}
		else if (dwFilterIndex == 2)
		{
			if (XML.Init() == FALSE)
				break;

			Buffer = "Hayman";
			XML.AddNode(NULL, &Buffer, NULL, &pvNode);

			Buffer = "Items";
			XML.AddNode(pvNode, &Buffer, NULL, &pvSubNode);

			iItem = 0;
			// get device
			while (ListView.GetItem(iItem, Name.Get(), NULL, 0, Name.GetMaxLength()) == TRUE)
			{
				Name.Update();

				// get value
				if (ListView.GetItem(iItem, Value.Get(), NULL, 1, Value.GetMaxLength()) == FALSE)
					break;
				Value.Update();

				Buffer = _T("Item");
				if (XML.AddNode(pvSubNode, &Buffer, &Value, &pvSubSubNode) == TRUE)
				{
					Buffer = _T("Name");
					XML.AddAttribute(pvSubSubNode, &Buffer, &Name);

					Buffer = _T("Value");
					XML.AddAttribute(pvSubSubNode, &Buffer, &Value);

					((IUnknown *)pvSubSubNode)->Release();
				}

				iItem++;
			}

			XML.SaveToFile(FileName.Get());
		}

		break;
	}

	if (pvSubNode != NULL)
		((IUnknown *)pvSubNode)->Release();
	if (pvNode != NULL)
		((IUnknown *)pvNode)->Release();

	Files.FileClose();

	XML.DeInit();
}

void C_Hayman::LogControl(void)
{
	_C_Files								Files;
	_C_String							FileName;
	_C_String							Buffer;

	if ((Log.GetFlags() & _C_LOG_FLAG_DISABLED) != 0)
	{
		FileName = _T("Log.csv");
		Buffer = StringTable.GetStringEx(IDS_FILTER_LOG);
		if (Files.SelectFile(hWnd, Buffer.Get(), OFN_OWN_CSIDL_NO_DIRECTORY, StringTable.GetStringEx(IDS_SELECT_LOG_FILE), OFN_OWN_FILE_SAVE_DIALOG, FileName.Get(), FileName.GetMaxLength()) == FALSE)
			return;
		FileName.Update();

		Log.NewFilename(_T(""), FileName.Get());
		Log.SetFlags(Log.GetFlags() & ~_C_LOG_FLAG_DISABLED);

		ToolBar.SetButton(IDC_TOOL_LOG, TBIF_STATE, 0, 0, 0, TBSTATE_PRESSED | TBSTATE_ENABLED, _T(""));
	}
	else
	{
		Log.SetFlags(Log.GetFlags() | _C_LOG_FLAG_DISABLED);

		ToolBar.SetButton(IDC_TOOL_LOG, TBIF_STATE, 0, 0, 0, TBSTATE_ENABLED, _T(""));
	}
}

BOOL C_Hayman::ListViewCustomDraw(LPNMLVCUSTOMDRAW pNMLVCustomDraw, void *pvContext)
{
	C_Hayman						*pThis;

	pThis = (C_Hayman *)pvContext;
//XXX draw graph / color
	switch (pNMLVCustomDraw->nmcd.dwDrawStage)
	{
		case CDDS_PREPAINT:
			// request notification for every item
			SetWindowLongPtr(pThis->hWnd, DWLP_MSGRESULT, CDRF_NOTIFYITEMDRAW);
			return TRUE;

		case CDDS_ITEMPREPAINT:
			// request notification for every sub item
			SetWindowLongPtr(pThis->hWnd, DWLP_MSGRESULT, CDRF_NOTIFYSUBITEMDRAW);
			return TRUE;

		case CDDS_ITEMPREPAINT | CDDS_SUBITEM:
			// Tag: Column
			if (pNMLVCustomDraw->iSubItem == 8)
			{
				//
				// % colored background
				//
				COLOR_HSL_STRUCT			ColorHSL;
				COLOR_RGB_STRUCT			ColorRGB;
				int							iDegree;
				_C_String					Buffer;
				FLOAT							fFloat;

				pThis->ListView.GetSubItemText(Buffer.Get(), pNMLVCustomDraw->iSubItem, pNMLVCustomDraw->nmcd.dwItemSpec);
				Buffer.Update();
				fFloat = 0;
				_stscanf(Buffer.Get(), _T("%f"), &fFloat);

				// hue: 85-0 -> 120'-0° -> 0%-100%
				if (fFloat < 0)
					fFloat = 0;
				if (fFloat > 100)
					fFloat = 100;

				iDegree = (int)(fFloat * 120 / 100);
				ColorHSL.h = 120 - iDegree;
				ColorHSL.s = 1.0;
				ColorHSL.l = 0.7;
				ColorRGB = _C_Color::HSLtoRGB(ColorHSL);
				pNMLVCustomDraw->clrTextBk = RGB(ColorRGB.r * 255, ColorRGB.g * 255, ColorRGB.b * 255);

				SetWindowLongPtr(pThis->hWnd, DWLP_MSGRESULT, CDRF_DODEFAULT);
			}
			// Tag: Column
			else if (pNMLVCustomDraw->iSubItem == 7)
			{
				//
				// history graph
				//
				POINT							*pPoints;
				UINT							uiEntries;
				HPEN							hPen;
				HGDIOBJ						hPrevious;
				RECT							Rect;
				LPARAM						lParam;
				_C_String					Buffer;
				BOOL							bResult;

				// get sub item rect
				if (pThis->ListView.GetSubItemRect(pNMLVCustomDraw->nmcd.dwItemSpec, pNMLVCustomDraw->iSubItem, &Rect) == TRUE)
				{
					Rect.left++;
					Rect.top++;
					Rect.bottom -= 2;

					if (Rect.right > Rect.left)
					{
						uiEntries = (Rect.right - Rect.left) - 1;
						if ((pPoints = (POINT *)malloc(sizeof(POINT) * uiEntries)) != NULL)
						{
							if (pThis->ListView.GetItem(pNMLVCustomDraw->nmcd.dwItemSpec, NULL, &lParam) == TRUE)
							{
								// check if a float value is shown
								pThis->ListView.GetSubItemText(Buffer.Get(), 5, pNMLVCustomDraw->nmcd.dwItemSpec);
								Buffer.Update();

								// get register history
								if (Buffer.GetLength() > 0)
									bResult = pThis->DataBase.GetRegisterHistoryFloat(pThis->uiCurrentCycle, Rect.left, Rect.top, (INT)uiEntries, (UINT)(Rect.bottom - Rect.top), LO_WORD(lParam), HI_WORD(lParam), pPoints);
								else
									bResult = pThis->DataBase.GetRegisterHistoryInt(pThis->uiCurrentCycle, Rect.left, Rect.top, (INT)uiEntries, (UINT)(Rect.bottom - Rect.top), LO_WORD(lParam), HI_WORD(lParam), pPoints);
								if (bResult == TRUE)
								{
									// draw graph
									hPen = CreatePen(PS_SOLID, 1, RGB(130, 130, 130));
									hPrevious = SelectObject(pNMLVCustomDraw->nmcd.hdc, hPen);

									Polyline(pNMLVCustomDraw->nmcd.hdc, pPoints, uiEntries);

									SelectObject(pNMLVCustomDraw->nmcd.hdc, hPrevious);
									DeleteObject(hPen);
								}
							}

							free(pPoints);
						}
					}
				}

				SetWindowLongPtr(pThis->hWnd, DWLP_MSGRESULT, CDRF_SKIPDEFAULT);
			}
			else
			{
				//
				// change the color of every second item
				//
				if ((pNMLVCustomDraw->nmcd.dwItemSpec & 1) == 1)
					pNMLVCustomDraw->clrTextBk = RGB(236, 236, 236);
				else
					pNMLVCustomDraw->clrTextBk = RGB(255, 255, 255);	// default white background color

				SetWindowLongPtr(pThis->hWnd, DWLP_MSGRESULT, CDRF_DODEFAULT);
			}
			return TRUE;
	}

	return FALSE;
}

BOOL C_Hayman::MRUPayloadCallback(TCHAR *pszString, DWORD dwTime, UINT uiIndex, void *pvContext)
{
	C_Hayman								*pThis;

	UNREF_PARAM(dwTime)
	UNREF_PARAM(uiIndex)

	pThis = (C_Hayman *)pvContext;

	pThis->ComboXPayload.AddItem(pszString, 0, IDI_MRU, NULL);

	return TRUE;
}

void C_Hayman::Polling(void)
{
	if (bPolling == TRUE)
	{
		KillTimer(hWnd, TIMER_ID_REFRESH);

		bPolling = FALSE;

		ToolBar.SetButton(IDC_TOOL_POLLING, TBIF_IMAGE | TBIF_TEXT, 1, 0, 0, 0, StringTable.GetStringEx(IDS_POLLING));
		ToolBar.SetButton(IDC_TOOL_SAVE, TBIF_STATE, 0, 0, 0, TBSTATE_ENABLED, _T(""));
	}
	else
	{
		bPolling = TRUE;

		ToolBar.SetButton(IDC_TOOL_POLLING, TBIF_IMAGE | TBIF_TEXT, 2, 0, 0, 0, StringTable.GetStringEx(IDS_STOP));
		ToolBar.SetButton(IDC_TOOL_SAVE, TBIF_STATE, 0, 0, 0, TBSTATE_INDETERMINATE, _T(""));

		SendCommand();
	}
}

BOOL C_Hayman::_Handle_Init(void)
{
	_C_String							Buffer;
	_C_String							Temp;
	INT_PTR								iIndex;
	_C_CommandLine						CommandLine;
	_C_Language							Language;

	_SetAppIcon(IDI_APP);
	_SetAppWindowText(Resources.GetVersionTitlebar(APP_NAME, hInstance, NULL));

	NOVATO_REPORT_SERVER_COMPSOFT

	uiCurrentCycle = 0;
	bPolling = FALSE;

	DataBase.Init();

	Log.SetFlags(_C_LOG_FLAG_DISABLED);

	CommandLine.AddString(_T("l"));
	CommandLine.Process();
	if (CommandLine.GetString(_T("l")) != NULL)
		StringTable.SetLanguage(Language.GetLanguageFromString(CommandLine.GetString(_T("l"))));

	ImageList.Init(C_IMAGELIST_FLAG_USE_SMALL_ICONS, hWnd);
	SendMessage(_GetItemHwnd(IDC_ERROR_ICON), STM_SETIMAGE, IMAGE_ICON, (LPARAM)ImageList.GetIcon(ImageList.AddIcon(hInstance, IDI_EXCLAMATION_MARK)));
	SendMessage(_GetItemHwnd(IDC_INFO_ICON), STM_SETIMAGE, IMAGE_ICON, (LPARAM)ImageList.GetIcon(ImageList.AddIcon(hInstance, IDI_INFO)));

	_SetItemText(IDC_INFO_TEXT, _T(""));
	_ShowItem(IDC_ERROR_ICON, FALSE);
	_ShowItem(IDC_INFO_ICON, FALSE);

	Settings.Init(_C_SETTINGS_USE_REGISTRY, REGISTRY_KEY, REGISTRY_SUB_KEY);
	uiFlags = (UINT)Settings.GetDWORD(_T("Flags"), FLAG_AUTO_START | FLAG_AUTO_UPDATE);

	uiRefresh = (UINT)Settings.GetDWORD(_T("Refresh"), 1000);
	Settings.GetString(_T("SerialPort"), &SerialPort, SERIAL_PORT_USE_LAST_DEVICE);

	ComboXCommand.Init(_GetItemHwnd(IDC_COMBO_COMMAND), hInstance, TRUE);
	DecodeCommand(0, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(1, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(2, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(3, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(6, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(11, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(12, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(13, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(14, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(15, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(16, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(17, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(18, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);
	DecodeCommand(19, &Buffer);
	ComboXCommand.AddItem(Buffer.Get(), 0, IDI_COMMAND, NULL);

	DecodeCommand(0, &Temp);
	Settings.GetString(_T("Command"), &Buffer, Temp.Get());
	if (ComboXCommand.FindItem(Buffer.Get(), FALSE, -1, &iIndex) == TRUE)
		ComboXCommand.SetFocus(iIndex);
	else
		ComboXCommand.SetText(Buffer.Get());

	ComboXPayload.Init(_GetItemHwnd(IDC_COMBO_PAYLOAD), hInstance, TRUE);
	MRUPayload.Init(REGISTRY_KEY, REGISTRY_SUB_KEY _T("\\MRU"), 100);
	MRUPayload.Enum(MRUPayloadCallback, this);
	Settings.GetString(_T("Payload"), &Buffer);
	ComboXPayload.SetText(Buffer.Get());

	ListView.Init(_GetItemHwnd(IDC_LIST_VIEW), hInstance, _C_LIST_VIEW_FLAG_ALLOW_COLUMN_SELECTION | _C_LIST_VIEW_FLAG_EDIT_ALL);
	ListView.SetNotificationFunctions(NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, this);
	ListView.SetNotificationFunctionsEx(NULL, NULL, NULL, NULL, NULL, ListViewCustomDraw);
	ListView.SetExStyles(LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP | LVS_EX_HEADERDRAGDROP, LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP | LVS_EX_HEADERDRAGDROP);
	ListView.AddIcon(IDI_DOT_BLUE);
	ListView.AddIcon(IDI_DOT_GREEN);
	ListView.AddIcon(IDI_DOT_RED);
	ListView.AddIcon(IDI_DOT_YELLOW);
	// Tag: Column
	ListView.AddColumn(StringTable.GetStringEx(IDS_NAME), 0, 300);
	ListView.AddColumn(StringTable.GetStringEx(IDS_VALUE), 1, 1500);
	ListView.ColumnSettingsRestore(REGISTRY_KEY, REGISTRY_SUB_KEY, _T("ListView"));

	Grip.Init(hWnd);

	Link.Init(_GetItemHwnd(IDC_LINK), _T("www.the-sz.com"), _T("http://www.the-sz.com/"), IDC_MY_HAND, C_LINK_FLAG_IS_OVER_TOOLBAR | C_LINK_FLAG_USE_DIALOG_FONT);

	Rebar.Init(hWnd, hInstance);

	ToolBar.Init(Rebar.hWnd, hInstance, 0, 24, 24);
	ToolBar.AddIcon(IDI_OPTIONS, FALSE);
	ToolBar.AddIcon(IDI_OPTIONS, TRUE);
	ToolBar.AddIcon(IDI_START, FALSE);
	ToolBar.AddIcon(IDI_START, TRUE);
	ToolBar.AddIcon(IDI_STOP, FALSE);
	ToolBar.AddIcon(IDI_STOP, TRUE);
	ToolBar.AddIcon(IDI_SAVE, FALSE);
	ToolBar.AddIcon(IDI_SAVE, TRUE);
	ToolBar.AddIcon(IDI_LOG, FALSE);
	ToolBar.AddIcon(IDI_LOG, TRUE);
	ToolBar.AddIcon(IDI_SEND_COMMAND, FALSE);
	ToolBar.AddIcon(IDI_SEND_COMMAND, TRUE);

	Buffer = StringTable.GetStringEx(IDS_OPTIONS);
	Buffer.Add('\0');
	Buffer.Update();
	ToolBar.AddButton(0, IDC_TOOL_OPTIONS, 0, Buffer.Get(), TBSTYLE_BUTTON);
	ToolBar.AddButton(0, 1, 0, NULL, TBSTYLE_SEP);

	Buffer = StringTable.GetStringEx(IDS_SAVE);
	Buffer.Add('\0');
	Buffer.Update();
	ToolBar.AddButton(3, IDC_TOOL_SAVE, 0, Buffer.Get(), TBSTYLE_BUTTON);
	ToolBar.SetButton(IDC_TOOL_SAVE, TBIF_STATE, 0, 0, 0, TBSTATE_INDETERMINATE, _T(""));

	Buffer = StringTable.GetStringEx(IDS_LOG);
	Buffer.Add('\0');
	Buffer.Update();
	ToolBar.AddButton(4, IDC_TOOL_LOG, 0, Buffer.Get(), TBSTYLE_BUTTON);
	ToolBar.AddButton(0, 1, 0, NULL, TBSTYLE_SEP);

	Buffer = StringTable.GetStringEx(IDS_POLLING);
	Buffer.Add('\0');
	Buffer.Update();
	ToolBar.AddButton(1, IDC_TOOL_POLLING, 0, Buffer.Get(), TBSTYLE_BUTTON | BTNS_SHOWTEXT);

	Buffer = StringTable.GetStringEx(IDS_SEND_COMMAND);
	Buffer.Add('\0');
	Buffer.Update();
	ToolBar.AddButton(5, IDC_TOOL_SEND_COMMAND, 0, Buffer.Get(), TBSTYLE_BUTTON | BTNS_SHOWTEXT);

	Rebar.AddBand(ToolBar.hWnd, ToolBar.GetWidth(), ToolBar.GetWidth(), ToolBar.GetHeight(), 0);
	Rebar.AddBand(Link.hwndControl, Link.GetSize()->cx + (UINT)(20 * _C_HDC::GetDpiFactor(hWnd)), Link.GetSize()->cx + (UINT)(20 * _C_HDC::GetDpiFactor(hWnd)), Link.GetSize()->cy, 1, RBBS_CHILDEDGE | RBBS_NOGRIPPER | RBBS_FIXEDSIZE);

	_SetItemText(IDC_COMMAND_INFO, Hayman.StringTable.GetStringEx(IDS_COMMAND));
	_SetItemText(IDC_PAYLOAD_INFO, Hayman.StringTable.GetStringEx(IDS_PAYLOAD));

	_SetMinSize(400, 250, TRUE);

	AlignContainer.Add(_GetItemHwnd(IDC_LIST_VIEW), ALIGN_WIDTH_FILL | ALIGN_HEIGHT_FILL);
	AlignContainer.Add(_GetItemHwnd(IDC_COMBO_PAYLOAD), ALIGN_WIDTH_FILL);

	_WindowPositionRestore(REGISTRY_KEY, REGISTRY_SUB_KEY, _T("Main"), 0);

	Engine.Init();

	if ((uiFlags & FLAG_AUTO_START) != 0)
		Polling();

	if ((uiFlags & FLAG_AUTO_UPDATE) != 0)
		AutoUpdate.Init(AUTO_UPDATE_URL, hWnd, REGISTRY_KEY, REGISTRY_SUB_KEY, _T("LastCheck"));

	return TRUE;
}

BOOL C_Hayman::_Handle_Exit(INT_PTR iResult)
{
	_C_String					Buffer;

	UNREF_PARAM(iResult)

	_ShowWindow(SW_HIDE);

	KillTimer(hWnd, TIMER_ID_REFRESH);
	bPolling = FALSE;

	//
	// save settings
	//
	Settings.SetDWORD(_T("Flags"), uiFlags & FLAG_SAVE_MASK);

	ComboXCommand.GetText(Buffer.Get());
	Buffer.Update();
	Settings.SetString(_T("Command"), Buffer.Get());

	ComboXPayload.GetText(Buffer.Get());
	Buffer.Update();
	if (Buffer.GetLength() > 0)
		MRUPayload.Add(Buffer.Get());
	MRUPayload.DeInit();

	Settings.SetDWORD(_T("Refresh"), uiRefresh);

	Settings.SetString(_T("SerialPort"), SerialPort.Get());

	_WindowPositionSave(REGISTRY_KEY, REGISTRY_SUB_KEY, _T("Main"));

	ListView.ColumnSettingsSave(REGISTRY_KEY, REGISTRY_SUB_KEY, _T("ListView"));
	ListView.DeInit();

	AutoUpdate.DeInit();

	ComboXCommand.DeInit();
	ComboXPayload.DeInit();

	DataBase.DeInit();

	Engine.DeInit();

	return TRUE;
}

BOOL C_Hayman::_Handle_WM_COMMAND(UINT uiID, WPARAM wParam, LPARAM lParam)
{
	switch (uiID)
	{
		case IDC_TOOL_OPTIONS:
			Options.Run(hInstance, IDD_OPTIONS, 0, hWnd);
			return TRUE;

		case IDC_TOOL_POLLING:
			Polling();
			return TRUE;

		case IDC_TOOL_SEND_COMMAND:
			SendCommand();
			return TRUE;

		case IDC_TOOL_SAVE:
			Save();
			return TRUE;

		case IDC_TOOL_LOG:
			LogControl();
			return TRUE;

		default:
			return _C_AppDialog::_Handle_WM_COMMAND(uiID, wParam, lParam);
	}
}

BOOL C_Hayman::_Handle_MESSAGE(UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	BOOL								bResult;

	switch (uiMsg)
	{
		case WM_TIMER:
			if (wParam == TIMER_ID_REFRESH)
				SendCommand();
			return TRUE;

		case USR_UPDATE_RESPONSE:
			UpdateResponse(wParam);
			return TRUE;

		case WM_NCHITTEST:
			return Grip.Handle_WM_NCHITTEST(wParam, lParam);

		case WM_PAINT:
			return Grip.Handle_WM_PAINT();

		case WM_SIZE:
			AlignContainer.Handle_WM_SIZE(wParam, lParam);
			Grip.Handle_WM_SIZE();
			Rebar.Resize();
			return TRUE;

		case WM_DRAWITEM:
			if (ComboXPayload.Handle_WM_DRAWITEM(wParam, lParam) == TRUE)
					return TRUE;
			return ComboXCommand.Handle_WM_DRAWITEM(wParam, lParam);

		case WM_MEASUREITEM:
			if (ComboXPayload.Handle_WM_MEASUREITEM(wParam, lParam) == TRUE)
					return TRUE;
			return ComboXCommand.Handle_WM_MEASUREITEM(wParam, lParam);

		case WM_NOTIFY:
			if (ListView.ProcessNotifyMessage(hWnd, wParam, lParam, &bResult) == TRUE)
				return bResult;
			break;
	}

	return _C_AppDialog::_Handle_MESSAGE(uiMsg, wParam, lParam);
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nCmdShow);

	return (int)Hayman.Run(hInstance, IDD_MAIN, 0);
}
