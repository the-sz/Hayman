#include <_PCH.h>

#include "Hayman.h"

BOOL C_Options::SerialPortEnum(_C_SerialPortEntry *pSerialPortEntry, void *pvContext, _C_SerialPort *pSerialPort)
{
	C_Options						*pThis;

	UNREF_PARAM(pSerialPort)

	pThis = (C_Options *)pvContext;

	if (pSerialPort->IsBlacklisted(&pSerialPortEntry->Name, &pSerialPortEntry->Hardware) == TRUE)
		return TRUE;

	pThis->ComboXDeviceList.AddItem(pSerialPortEntry->Name.Get(), 0, IDI_SERIAL_PORT, NULL);

	return TRUE;
}

BOOL C_Options::_Handle_Init(void)
{
	_C_SerialPort					SerialPort;
	INT_PTR							iIndex;

	_SetAppIcon(IDI_APP);
	_SetAppWindowText(Hayman.Resources.GetVersionTitlebar(APP_NAME, hInstance, NULL));

	_SetItemInt(IDC_REFRESH_RATE, Hayman.uiRefresh);

	ComboXDeviceList.Init(_GetItemHwnd(IDC_DEVICE_LIST), hInstance, TRUE);
	SerialPort.Init(0);
	SerialPort.Enum(SerialPortEnum, this);
	SerialPort.DeInit();
	ComboXDeviceList.AddItem(Hayman.StringTable.GetStringEx(IDS_USE_LAST_DEVICE), 0, IDI_SERIAL_PORT, &iIndex);
	if (Hayman.SerialPort.GetLength() != 0)
	{
		if (Hayman.SerialPort.Compare(SERIAL_PORT_USE_LAST_DEVICE) == 0)
			ComboXDeviceList.SetFocus(iIndex);
		else if (ComboXDeviceList.FindItem(Hayman.SerialPort.Get(), TRUE, -1, &iIndex) == TRUE)
			ComboXDeviceList.SetFocus(iIndex);
		else
		{
			ComboXDeviceList.AddItem(Hayman.SerialPort.Get(), 0, IDI_SERIAL_PORT, &iIndex);
			ComboXDeviceList.SetFocus(iIndex);
		}
	}
	else
		ComboXDeviceList.SetFocus(0);

	if ((Hayman.uiFlags & FLAG_AUTO_START) != 0)
		_CheckItem(IDC_CHECK_AUTO_START, TRUE);

	if ((Hayman.uiFlags & FLAG_AUTO_UPDATE) != 0)
		_CheckItem(IDC_CHECK_AUTO_UPDATE, TRUE);

	_SetItemText(IDC_DEVICE_LIST_INFO, Hayman.StringTable.GetStringEx(IDS_DEVICE_LIST));
	_SetItemText(IDC_REFRESH_RATE_INFO, Hayman.StringTable.GetStringEx(IDS_REFRESH_RATE));
	_SetItemText(IDC_REFRESH_RATE_MS, Hayman.StringTable.GetStringEx(IDS_REFRESH_RATE_MS));
	_SetItemText(IDC_CHECK_AUTO_START, Hayman.StringTable.GetStringEx(IDS_CHECK_AUTO_START));
	_SetItemText(IDC_CHECK_AUTO_UPDATE, Hayman.StringTable.GetStringEx(IDS_CHECK_AUTO_UPDATE));
	_SetItemText(IDC_OK, Hayman.StringTable.GetStringEx(IDS_OK));
	_SetItemText(IDC_CANCEL, Hayman.StringTable.GetStringEx(IDS_CANCEL));

	return TRUE;
}

BOOL C_Options::_Handle_Exit(INT_PTR iResult)
{
	if (iResult == 1)
	{
		//
		// save settings
		//
		if (_IsChecked(IDC_CHECK_AUTO_START) == TRUE)
			Hayman.uiFlags |= FLAG_AUTO_START;
		else
			Hayman.uiFlags &= ~FLAG_AUTO_START;

		if (_IsChecked(IDC_CHECK_AUTO_UPDATE) == TRUE)
			Hayman.uiFlags |= FLAG_AUTO_UPDATE;
		else
			Hayman.uiFlags &= ~FLAG_AUTO_UPDATE;

		ComboXDeviceList.GetText(Hayman.SerialPort.Get());
		Hayman.SerialPort.Update();
		if (Hayman.SerialPort.Compare(Hayman.StringTable.GetStringEx(IDS_USE_LAST_DEVICE)) == 0)
			Hayman.SerialPort = SERIAL_PORT_USE_LAST_DEVICE;

		_GetItemInt(IDC_REFRESH_RATE, (int *)&Hayman.uiRefresh, FALSE);

		Hayman.Engine.Close();
	}

	ComboXDeviceList.DeInit();

	return TRUE;
}

BOOL C_Options::_Handle_WM_COMMAND(UINT uiID, WPARAM wParam, LPARAM lParam)
{
	switch (uiID)
	{
		case IDC_CANCEL:
			_EndDialog(0);
			return TRUE;

		case IDC_OK:
			_EndDialog(1);
			return TRUE;
	}

	return _C_AppDialog::_Handle_WM_COMMAND(uiID, wParam, lParam);
}

BOOL C_Options::_Handle_MESSAGE(UINT uiMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uiMsg)
	{
		case WM_DRAWITEM:
			return ComboXDeviceList.Handle_WM_DRAWITEM(wParam, lParam);

		case WM_MEASUREITEM:
			return ComboXDeviceList.Handle_WM_MEASUREITEM(wParam, lParam);

		case WM_DPICHANGED:
			ComboXDeviceList.Handle_WM_DPICHANGED();
			break;
	}

	return _C_AppDialog::_Handle_MESSAGE(uiMsg, wParam, lParam);
}
