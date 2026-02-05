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
#include <_C_StringTable.h>
#include <_C_Time.h>
#include <_C_CommandLine.h>
#include <_C_Language.h>
#include <_C_ProgressBar.h>
#include <_C_Clipboard.h>
#include <_C_MRU.h>
#include <_C_Buffer.h>
#include <_C_Debug.h>

#include "..\HART\_C_HART.h"

#include "C_Engine.h"
#include "C_Options.h"

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

#define USR_UPDATE_RESPONSE								(WM_APP + 1)		//	wParam: HART_RESULT_ENUM

#define SERIAL_PORT_USE_LAST_DEVICE						_T("***USE_LAST_DEVICE***")

class C_CommandName : public _C_Listable
{
	public:
		WORD														wCommand;
		_C_String												Name;
};

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
		_C_Log													Log;
		BYTE														bReceiveBuffer[100];
		UINT														uiReceiveBufferLength;
		_C_Buffer<BYTE>										SendBufferDebug;
		_C_List													CommandNameList;

		static BOOL ListViewCustomDraw(LPNMLVCUSTOMDRAW pNMLVCustomDraw, void *pvContext);
		static void ListViewKeyDown(INT iItem, LPARAM lParam, WORD wKey, void *pvContext);
		static BOOL MRUPayloadCallback(TCHAR *pszString, DWORD dwTime, UINT uiIndex, void *pvContext);
		static BOOL CommandNameXMLEnum(_C_XML_ENUM_CALLBACK_TYPE_ENUM Type, void *pvElement, _C_String *pName, _C_String *pText, void *pvContext, _C_XML *pXML);
		void CommandNameInit(void);
		void Save(void);
		void LogControl(void);
		void ShowStatistic(void);
		void ShowError(INT iResourceId);
		void ListViewAdd(TCHAR *pszName, TCHAR *pszValue, INT iImage);
		void DumpBuffer(BYTE *pbBuffer, UINT uiLength, _C_String *pString);
		void DecodeManufacturerAndDevice(WORD wManufacturer, WORD wDeviceType, _C_String *pManufacturer, _C_String *pDeviceType);
		void DecodeCommand(WORD wCommand, _C_String *pCommand);
		void DecodeResponseCode(BYTE bResponseCode, _C_String *pResponseCode);
		void DecodeDeviceVariableClassificationCode(BYTE bDeviceVariableClassificationCode, _C_String *pDeviceVariableClassificationCode);
		void DecodeUnitCode(BYTE bUnitCode, _C_String *pUnitCode);
		FLOAT DecodeFloat(BYTE *pbBuffer);
		DWORD DecodeDWord(BYTE *pbBuffer);
		void DecodePackedString(BYTE *pbBuffer, _C_String *pString);
		void DecodePayloadCommandUnknown(UINT uiOffset);
		void DecodePayloadCommand0(UINT uiOffset);
		void DecodePayloadCommand1(UINT uiOffset);
		void DecodePayloadCommand2(UINT uiOffset);
		void DecodePayloadCommand3(UINT uiOffset);
		void DecodePayloadCommand7(UINT uiOffset);
		void DecodePayloadCommand8(UINT uiOffset);
		void DecodePayloadCommand13(UINT uiOffset);
		void DecodePayloadCommand20And22(UINT uiOffset);
		void DecodePayloadCommand54(UINT uiOffset);
		void SetPollingButton(void);
		void Polling(void);
		void ListViewCopy(void);

	public:
		_C_Resources											Resources;
		UINT														uiFlags;
		UINT														uiRefresh;
		_C_String												SerialPort;
		C_Engine													Engine;
		_C_StringTable											StringTable;
		BOOL														bPolling;

		void SendCommand(void);
		void UpdateResponse(HART_RESULT_ENUM Result);

		BOOL _Handle_Init(void);
		BOOL _Handle_Exit(INT_PTR iResult);		// if this function returns FALSE, _EndDialog will not be executed
		BOOL _Handle_WM_COMMAND(UINT uiID, WPARAM wParam, LPARAM lParam);
		BOOL _Handle_MESSAGE(UINT uiMsg, WPARAM wParam, LPARAM lParam);
};

extern C_Hayman												Hayman;

#endif
