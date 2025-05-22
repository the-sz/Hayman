#ifndef __C_Engine_h
#define __C_Engine_h

//XXX alis support
class C_EngineAddressAlias : public _C_Listable
{
	public:
		UINT												uiFunction;
		UINT												uiAddress;
		_C_String										Alias;
		INT												iMin;
		INT												iMax;
		UINT												uiFlags;
};

typedef enum
{
	ERT_SEND_COMMAND,

} ENGINE_REQUEST_TYPE_ENUM;

class C_EngineRequest : public _C_ThreadEngineRequest
{
	public:
		ENGINE_REQUEST_TYPE_ENUM					Type;
		WORD												wCommand;
		BYTE												bAdditionalBuffer[HART_MAX_REQUEST_BYTES];
		BYTE												bAdditionalBufferLength;
		void												*pvReceiveBuffer;
		UINT												*puiReceiveBufferLength;
		HWND												hWnd;
		UINT												uiMsg;
};

class C_Engine
{
	protected:
		_C_ThreadEngine								ThreadEngine;
//XXX alis support
		_C_List											AddressAliasList;

		static BOOL ThreadEngineCallback(THREAD_ENGINE_CALLBACK_RESON_ENUM Reason, _C_ThreadEngineRequest *pRequest, void *pvCallbackContext);
		static BOOL XMLAddressAliasEnum(_C_XML_ENUM_CALLBACK_TYPE_ENUM Type, void *pvElement, _C_String *pName, _C_String *pText, void *pvContext, _C_XML *pXML);

	public:
		_C_HART											HART;

		C_Engine(void);
		void Init(void);
		void DeInit(void);
		void Close(void);

		BOOL SendCommandAsync(WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg);
		void SendCommand(WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg);

//XXX alis support
		void AddressAliasAdd(UINT uiFunction, UINT uiAddress, INT iMin, INT iMax, TCHAR *pszAlias, UINT uiFlags);
		BOOL AddressAliasGet(UINT uiFunction, UINT uiAddress, _C_String *pAlias, INT *piMin, INT *piMax, UINT *puiFlags);
		void AddressAliasSet(UINT uiFunction, UINT uiAddress, TCHAR *pszAlias);
};

#endif
