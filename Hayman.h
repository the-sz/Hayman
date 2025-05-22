#ifndef __C_Hayman_h
#define __C_Hayman_h

#include <_C_AppDialog.h>
#include <_C_Resources.h>
#include <_C_Settings.h>
#include <_C_AutoUpdate.h>
#include <_C_ComboBoxX.h>
#include <_C_ListView.h>
#include <_C_ToolBar.h>
#include <_C_Rebar.h>
#include <_C_Align.h>
#include <_C_Link.h>
#include <_C_Grip.h>
#include <_C_SerialPort.h>
#include <_C_ThreadEngine.h>
#include <_C_Color.h>
#include <_C_DataBase.h>
#include <_C_StringTable.h>
#include <_C_Time.h>
#include <_C_CommandLine.h>
#include <_C_Language.h>
#include <_C_ProgressBar.h>
#include <_C_MRU.h>
#include <_C_Debug.h>

#include "..\HART\_C_HART.h"

#include "C_Engine.h"
#include "C_Options.h"
#include "C_DataBase.h"

#include "Resource.h"

#include <float.h>

#define APP_NAME												_T("Hayman - HART Viewer")

#define REGISTRY_KEY											HKEY_CURRENT_USER
#define REGISTRY_SUB_KEY									_T("Software\\the sz development\\Hayman")

#define AUTO_UPDATE_URL										_T("http://www.the-sz.com/common/update.php?p=hayman")

#define IDC_TOOL_POLLING									5000
#define IDC_TOOL_OPTIONS									5001
#define IDC_TOOL_SAVE										5002
#define IDC_TOOL_LOG											5003
#define IDC_TOOL_SEND_COMMAND								5004

#define FLAG_AUTO_START										0x00000001
#define FLAG_AUTO_UPDATE									0x00000002
#define FLAG_SAVE_MASK										0x0000FFFF
#define FLAG_SETTINGS_XML_IS_READ_ONLY					0x80000000

#define TIMER_ID_REFRESH									0x01

#define USR_UPDATE_RESPONSE								(WM_APP + 1)		//	wParam: bResult

#define SERIAL_PORT_USE_LAST_DEVICE						_T("***USE_LAST_DEVICE***")

class C_Hayman : public _C_AppDialog
{
	protected:
		_C_Settings												Settings;
		_C_ComboBoxX											ComboXCommand;
		_C_ComboBoxX											ComboXPayload;
		_C_MRU													MRUPayload;
		_C_ListView												ListView;
		_C_ToolBar												ToolBar;
		_C_Rebar													Rebar;
		_C_Link													Link;
		_C_Grip													Grip;
		_C_AutoUpdate											AutoUpdate;
		_C_AlignContainer										AlignContainer;
		_C_ImageList											ImageList;
		C_Options												Options;
		C_DataBase												DataBase;
//XXX draw graph
		UINT														uiCurrentCycle;
		_C_Log													Log;
		BYTE														bReceiveBuffer[100];
		UINT														uiReceiveBufferLength;

		static BOOL ListViewCustomDraw(LPNMLVCUSTOMDRAW pNMLVCustomDraw, void *pvContext);
		static BOOL MRUPayloadCallback(TCHAR *pszString, DWORD dwTime, UINT uiIndex, void *pvContext);
		void Save(void);
		void LogControl(void);
		void ShowStatistic(void);
		void ShowError(INT iResourceId);
		void ListViewAdd(TCHAR *pszName, TCHAR *pszValue, INT iImage);
		void DecodeManufacturerAndDevice(WORD wManufacturer, WORD wDeviceType, _C_String *pManufacturer, _C_String *pDeviceType);
		void DecodeCommand(WORD wCommand, _C_String *pCommand);
		void DecodeResponseCode(BYTE bResponseCode, _C_String *pResponseCode);
		FLOAT DecodeFloat(BYTE *pbBuffer);
		void DecodePayloadCommand0(UINT uiOffset);
		void DecodePayloadCommand2(UINT uiOffset);
		void Polling(void);

	public:
		_C_Resources											Resources;
		UINT														uiFlags;
		UINT														uiRefresh;
		_C_String												SerialPort;
		C_Engine													Engine;
		_C_StringTable											StringTable;
		BOOL														bPolling;
//XXX draw graph
		UINT														uiLastFunction;

		void SendCommand(void);
		void UpdateResponse(BOOL bResult);

		BOOL _Handle_Init(void);
		BOOL _Handle_Exit(INT_PTR iResult);		// if this function returns FALSE, _EndDialog will not be executed
		BOOL _Handle_WM_COMMAND(UINT uiID, WPARAM wParam, LPARAM lParam);
		BOOL _Handle_MESSAGE(UINT uiMsg, WPARAM wParam, LPARAM lParam);
};

extern C_Hayman												Hayman;

#endif
