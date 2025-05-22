#ifndef __C_DataBase_h
#define __C_DataBase_h

class C_DataBaseRegisterInt : public _C_Listable
{
	public:
		~C_DataBaseRegisterInt(void);

		UINT						uiDevice;
		UINT						uiRegister;
		_C_List					ValuesList;
		INT						iMinEver;
		INT						iMaxEver;
		UINT						uiAliasFlags;
};

class C_DataBaseRegisterFloat : public _C_Listable
{
	public:
		~C_DataBaseRegisterFloat(void);

		UINT						uiDevice;
		UINT						uiRegister;
		_C_List					ValuesList;
		FLOAT						fMinEver;
		FLOAT						fMaxEver;
};

class C_DataBaseValueInt : public _C_Listable
{
	public:
		UINT						uiCycle;
		BYTE					uiValue;
};

class C_DataBaseValueFloat : public _C_Listable
{
	public:
		UINT						uiCycle;
		FLOAT						fValue;
};

class C_DataBase
{
	protected:
		_C_List					RegisterListInt;
		_C_List					RegisterListFloat;

		C_DataBaseRegisterInt *RegisterFindInt(UINT uiDevice, UINT uiRegister);
		C_DataBaseRegisterFloat *RegisterFindFloat(UINT uiDevice, UINT uiRegister);

	public:
		BOOL Init(void);
		void DeInit(void);
		void ReInit(void);

		BOOL AddRegisterValue(UINT uiCycle, UINT uiDevice, UINT uiRegister, BYTE uiValue, UINT uiAliasFlags);
		BOOL AddRegisterValue(UINT uiCycle, UINT uiDevice, UINT uiRegister, FLOAT fValue);

		BOOL GetRegisterHistoryInt(UINT uiCycle, INT iXOffset, INT iYOffset, INT iXMax, UINT uiYMax, UINT uiDevice, UINT uiRegister, POINT *pPoints);
		BOOL GetRegisterHistoryFloat(UINT uiCycle, INT iXOffset, INT iYOffset, INT iXMax, UINT uiYMax, UINT uiDevice, UINT uiRegister, POINT *pPoints);
		BOOL GetRegisterMinMax(UINT uiDevice, UINT uiRegister, INT *piMin, INT *piMax);
		BOOL GetRegisterMinMax(UINT uiDevice, UINT uiRegister, FLOAT *pfMin, FLOAT *pfMax);
		void GetRegisterMinMaxEver(UINT uiDevice, UINT uiRegister, INT *piMin, INT *piMax);
		void GetRegisterMinMaxEver(UINT uiDevice, UINT uiRegister, FLOAT *pfMin, FLOAT *pfMax);
};

#endif
