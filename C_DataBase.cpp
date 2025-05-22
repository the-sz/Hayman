#include <_PCH.h>

#include "Hayman.h"

#undef DPrintfCore
#define DPrintfCore					DPrintf_

#define MAX_VALUES_IN_LIST			100

//
// C_DataBaseRegisterInt
//
C_DataBaseRegisterInt::~C_DataBaseRegisterInt(void)
{
	ValuesList.DeleteAllListable();
}

//
// C_DataBaseRegisterFloat
//
C_DataBaseRegisterFloat::~C_DataBaseRegisterFloat(void)
{
	ValuesList.DeleteAllListable();
}

//
// C_DataBase
//
BOOL C_DataBase::Init(void)
{
	RegisterListInt.Init();
	RegisterListFloat.Init();

	return TRUE;
}

void C_DataBase::DeInit(void)
{
	RegisterListInt.DeleteAllListable();
	RegisterListFloat.DeleteAllListable();
}

void C_DataBase::ReInit(void)
{
	DeInit();

	Init();
}

C_DataBaseRegisterInt *C_DataBase::RegisterFindInt(UINT uiDevice, UINT uiRegister)
{
	C_DataBaseRegisterInt					*pRegister;

	pRegister = (C_DataBaseRegisterInt *)RegisterListInt.GetFirstListable();
	while (pRegister != NULL)
	{
		if ((pRegister->uiDevice == uiDevice) && (pRegister->uiRegister == uiRegister))
			break;

		pRegister = (C_DataBaseRegisterInt *)pRegister->GetNextListable();
	}

	return pRegister;
}

C_DataBaseRegisterFloat *C_DataBase::RegisterFindFloat(UINT uiDevice, UINT uiRegister)
{
	C_DataBaseRegisterFloat					*pRegister;

	pRegister = (C_DataBaseRegisterFloat *)RegisterListFloat.GetFirstListable();
	while (pRegister != NULL)
	{
		if ((pRegister->uiDevice == uiDevice) && (pRegister->uiRegister == uiRegister))
			break;

		pRegister = (C_DataBaseRegisterFloat *)pRegister->GetNextListable();
	}

	return pRegister;
}

BOOL C_DataBase::AddRegisterValue(UINT uiCycle, UINT uiDevice, UINT uiRegister, BYTE uiValue, UINT uiAliasFlags)
{
	C_DataBaseRegisterInt					*pRegister;
	C_DataBaseValueInt						*pValue;

	// find register entry
	if ((pRegister = RegisterFindInt(uiDevice, uiRegister)) == NULL)
	{
		if ((pRegister = new C_DataBaseRegisterInt()) == NULL)
			return FALSE;

		pRegister->uiDevice = uiDevice;
		pRegister->uiRegister = uiRegister;
		pRegister->iMinEver = INT_MAX;
		pRegister->iMaxEver = INT_MAX;
		pRegister->uiAliasFlags = uiAliasFlags;
		pRegister->ValuesList.Init();
		RegisterListInt.AddTop(pRegister);
	}

	if ((pRegister->iMinEver == INT_MAX) || (pRegister->iMinEver > uiValue))
		pRegister->iMinEver = uiValue;
	if ((pRegister->iMaxEver == INT_MAX) || (pRegister->iMaxEver < uiValue))
		pRegister->iMaxEver = uiValue;

	// create new value entry
	if ((pValue = new C_DataBaseValueInt()) == NULL)
		return FALSE;
	pValue->uiCycle = uiCycle;
	pValue->uiValue = uiValue;
	pRegister->ValuesList.AddBottom(pValue);

	// remove too old entries
	while (pRegister->ValuesList.GetCount() > MAX_VALUES_IN_LIST)
	{
		if ((pValue = (C_DataBaseValueInt *)pRegister->ValuesList.GetFirstListable()) == NULL)
			break;
		pRegister->ValuesList.Remove(pValue);
		delete pValue;
	}

	return TRUE;
}

BOOL C_DataBase::AddRegisterValue(UINT uiCycle, UINT uiDevice, UINT uiRegister, FLOAT fValue)
{
	C_DataBaseRegisterFloat					*pRegister;
	C_DataBaseValueFloat						*pValue;

	// find register entry
	if ((pRegister = RegisterFindFloat(uiDevice, uiRegister)) == NULL)
	{
		if ((pRegister = new C_DataBaseRegisterFloat()) == NULL)
			return FALSE;

		pRegister->uiDevice = uiDevice;
		pRegister->uiRegister = uiRegister;
		pRegister->fMinEver = FLT_MAX;
		pRegister->fMaxEver = (-FLT_MAX);
		pRegister->ValuesList.Init();
		RegisterListFloat.AddTop(pRegister);
	}

	if (pRegister->fMinEver > fValue)
		pRegister->fMinEver = fValue;
	if (pRegister->fMaxEver < fValue)
		pRegister->fMaxEver = fValue;

	// create new value entry
	if ((pValue = new C_DataBaseValueFloat()) == NULL)
		return FALSE;
	pValue->uiCycle = uiCycle;
	pValue->fValue = fValue;
	pRegister->ValuesList.AddBottom(pValue);

	// remove too old entries
	while (pRegister->ValuesList.GetCount() > MAX_VALUES_IN_LIST)
	{
		if ((pValue = (C_DataBaseValueFloat *)pRegister->ValuesList.GetFirstListable()) == NULL)
			break;
		pRegister->ValuesList.Remove(pValue);
		delete pValue;
	}

	return TRUE;
}

BOOL C_DataBase::GetRegisterHistoryInt(UINT uiCycle, INT iXOffset, INT iYOffset, INT iXMax, UINT uiYMax, UINT uiDevice, UINT uiRegister, POINT *pPoints)
{
	C_DataBaseRegisterInt				*pRegister;
	C_DataBaseValueInt					*pValue;
	INT										iMin;
	INT										iMax;
	INT										iIndex;
	UINT										uiXOffset;
	UINT										uiAliasFlags;

	// pre-fill all slots with line at the bottom
	for (iIndex = 0; iIndex < iXMax; iIndex++)
	{
		pPoints[iIndex].x = iXOffset + iIndex;
		pPoints[iIndex].y = iYOffset + uiYMax;
	}

	if (iXMax > MAX_VALUES_IN_LIST)
	{
		uiXOffset = iXMax - MAX_VALUES_IN_LIST;
		iXMax = MAX_VALUES_IN_LIST;
	}
	else
		uiXOffset = 0;

	// get min and max values
	iMin = 0;
	iMax = 0;
	Hayman.Engine.AddressAliasGet(Hayman.uiLastFunction, uiRegister, NULL, &iMin, &iMax, &uiAliasFlags);
	if ((iMin == 0) && (iMax == 0))
		if (GetRegisterMinMax(uiDevice, uiRegister, &iMin, &iMax) == FALSE)
			return FALSE;

	// find register entry
	if ((pRegister = RegisterFindInt(uiDevice, uiRegister)) == NULL)
		return FALSE;

	// search all values
	pValue = (C_DataBaseValueInt *)pRegister->ValuesList.GetFirstListable();
	while (pValue != NULL)
	{
		if (pValue->uiCycle < uiCycle)
		{
			// calc index from cycle
			iIndex = iXMax - (uiCycle - pValue->uiCycle);

			if ((iIndex >= 0) && (iIndex < iXMax))
			{
				// calc y from value and uiYMax
				if (uiYMax != 0)
				{
				}
				else
					pPoints[uiXOffset + iIndex].y = (iYOffset + uiYMax);
			}
			else
			{
				// we have the case where iIndex == iXMax!
				DPrintfCore("C_DataBase::GetRegisterHistoryInt: Something really fucked up in the database.\n");
			}
		}

		pValue = (C_DataBaseValueInt *)pValue->GetNextListable();
	}

	return TRUE;
}

BOOL C_DataBase::GetRegisterHistoryFloat(UINT uiCycle, INT iXOffset, INT iYOffset, INT iXMax, UINT uiYMax, UINT uiDevice, UINT uiRegister, POINT *pPoints)
{
	C_DataBaseRegisterFloat				*pRegister;
	C_DataBaseValueFloat					*pValue;
	FLOAT										fMin;
	FLOAT										fMax;
	INT										iIndex;
	UINT										uiXOffset;
	FLOAT										fFloat;
	INT										iMin;
	INT										iMax;
	UINT										uiAliasFlags;

	// pre-fill all slots with line at the bottom
	for (iIndex = 0; iIndex < iXMax; iIndex++)
	{
		pPoints[iIndex].x = iXOffset + iIndex;
		pPoints[iIndex].y = iYOffset + uiYMax;
	}

	if (iXMax > MAX_VALUES_IN_LIST)
	{
		uiXOffset = iXMax - MAX_VALUES_IN_LIST;
		iXMax = MAX_VALUES_IN_LIST;
	}
	else
		uiXOffset = 0;

	// get min and max values
	iMin = 0;
	iMax = 0;
	Hayman.Engine.AddressAliasGet(Hayman.uiLastFunction, uiRegister, NULL, &iMin, &iMax, &uiAliasFlags);
	fMin = (FLOAT)iMin;
	fMax = (FLOAT)iMax;
	if ((fMin == 0) && (fMax == 0))
		if (GetRegisterMinMax(uiDevice, uiRegister, &fMin, &fMax) == FALSE)
			return FALSE;

	// find register entry
	if ((pRegister = RegisterFindFloat(uiDevice, uiRegister)) == NULL)
		return FALSE;

	// search all values
	pValue = (C_DataBaseValueFloat *)pRegister->ValuesList.GetFirstListable();
	while (pValue != NULL)
	{
		if (pValue->uiCycle < uiCycle)
		{
			// calc index from cycle
			iIndex = iXMax - (uiCycle - pValue->uiCycle);

			if ((iIndex >= 0) && (iIndex < iXMax))
			{
				// calc y from value and uiYMax
				if (uiYMax != 0)
				{
					fFloat = pValue->fValue;

					if (fFloat > fMax)
						fFloat = fMax;
					if (fFloat < fMin)
						fFloat = fMin;
					pPoints[uiXOffset + iIndex].y = iYOffset - (INT)((fFloat - fMax) / (fMax - fMin) * uiYMax);
				}
				else
					pPoints[uiXOffset + iIndex].y = (iYOffset + uiYMax);
			}
			else
			{
				// we have the case where iIndex == iXMax!
				DPrintfCore("C_DataBase::GetRegisterHistoryFloat: Something really fucked up in the database.\n");
			}
		}

		pValue = (C_DataBaseValueFloat *)pValue->GetNextListable();
	}

	return TRUE;
}

BOOL C_DataBase::GetRegisterMinMax(UINT uiDevice, UINT uiRegister, INT *piMin, INT *piMax)
{
	C_DataBaseRegisterInt					*pRegister;
	C_DataBaseValueInt						*pValue;

	// find register entry
	if ((pRegister = RegisterFindInt(uiDevice, uiRegister)) == NULL)
	{
		// use default values
		(*piMin) = 0x0000;
		(*piMax) = 0xFFFF;

		return FALSE;
	}

	// search all values
	pValue = (C_DataBaseValueInt *)pRegister->ValuesList.GetFirstListable();
	while (pValue != NULL)
	{
		{
			if (pValue->uiValue < (*piMin))
				(*piMin) = pValue->uiValue;

			if (pValue->uiValue > (*piMax))
				(*piMax) = pValue->uiValue;
		}

		pValue = (C_DataBaseValueInt *)pValue->GetNextListable();
	}

	return TRUE;
}

BOOL C_DataBase::GetRegisterMinMax(UINT uiDevice, UINT uiRegister, FLOAT *pfMin, FLOAT *pfMax)
{
	C_DataBaseRegisterFloat					*pRegister;
	C_DataBaseValueFloat						*pValue;

	// find register entry
	if ((pRegister = RegisterFindFloat(uiDevice, uiRegister)) == NULL)
	{
		// use default values
		(*pfMin) = 0.0;
		(*pfMax) = 100.0;

		return FALSE;
	}

	// search all values
	(*pfMin) = FLT_MAX;
	(*pfMax) = (-FLT_MAX);
	pValue = (C_DataBaseValueFloat *)pRegister->ValuesList.GetFirstListable();
	while (pValue != NULL)
	{
		if (pValue->fValue < (*pfMin))
			(*pfMin) = pValue->fValue;

		if (pValue->fValue > (*pfMax))
			(*pfMax) = pValue->fValue;

		pValue = (C_DataBaseValueFloat *)pValue->GetNextListable();
	}

	return TRUE;
}

void C_DataBase::GetRegisterMinMaxEver(UINT uiDevice, UINT uiRegister, INT *piMin, INT *piMax)
{
	C_DataBaseRegisterInt					*pRegister;

	(*piMin) = INT_MAX;
	(*piMax) = INT_MAX;

	// find register entry
	if ((pRegister = RegisterFindInt(uiDevice, uiRegister)) != NULL)
	{
		(*piMin) = pRegister->iMinEver;
		(*piMax) = pRegister->iMaxEver;
	}
}

void C_DataBase::GetRegisterMinMaxEver(UINT uiDevice, UINT uiRegister, FLOAT *pfMin, FLOAT *pfMax)
{
	C_DataBaseRegisterFloat					*pRegister;

	(*pfMin) = FLT_MAX;
	(*pfMax) = (-FLT_MAX);

	// find register entry
	if ((pRegister = RegisterFindFloat(uiDevice, uiRegister)) != NULL)
	{
		(*pfMin) = pRegister->fMinEver;
		(*pfMax) = pRegister->fMaxEver;
	}
}
