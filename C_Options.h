#ifndef __C_Options_h
#define __C_Options_h

class C_Options : public _C_AppDialog
{
	protected:
		_C_ComboBoxX				ComboXDeviceList;

		static BOOL SerialPortEnum(_C_SerialPortEntry *pSerialPortEntry, void *pvContext, _C_SerialPort *pSerialPort);

	public:
		BOOL _Handle_Init(void);
		BOOL _Handle_Exit(INT_PTR iResult);		// if this function returns FALSE, _EndDialog will not be executed
		BOOL _Handle_WM_COMMAND(UINT uiID, WPARAM wParam, LPARAM lParam);
		BOOL _Handle_MESSAGE(UINT uiMsg, WPARAM wParam, LPARAM lParam);
};

#endif
