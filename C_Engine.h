#ifndef __C_Engine_h
#define __C_Engine_h

typedef enum
{
	ERT_SEND_COMMAND,

} ENGINE_REQUEST_TYPE_ENUM;

class C_EngineRequest : public _C_ThreadEngineRequest
{
	public:
		ENGINE_REQUEST_TYPE_ENUM					Type;
		BYTE												bShortAddress;
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

		static BOOL ThreadEngineCallback(THREAD_ENGINE_CALLBACK_RESON_ENUM Reason, _C_ThreadEngineRequest *pRequest, void *pvCallbackContext);

	public:
		_C_HART											HART;

		C_Engine(void);
		void Init(void);
		void DeInit(void);
		void Close(void);

		BOOL SendCommandAsync(BYTE bShortAddress, WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg);
		void SendCommand(BYTE bShortAddress, WORD wCommand, void *pvAdditionalBuffer, BYTE bAdditionalBufferLength, void *pvReceiveBuffer, UINT *puiReceiveBufferLength, HWND hWnd, UINT uiMsg);
};

#endif
