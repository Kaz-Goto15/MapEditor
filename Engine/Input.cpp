#include "Input.h"

namespace Input
{
	LPDIRECTINPUT8   pDInput = nullptr;
	LPDIRECTINPUTDEVICE8 pKeyDevice = nullptr;
	BYTE keyState[256] = { 0 };
	BYTE prevState[256] = { 0 };

	XMVECTOR mousePosition;

	HRESULT Initialize(HWND hWnd)
	{
		HRESULT hr;
		hr = DirectInput8Create(GetModuleHandle(nullptr), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&pDInput, nullptr);
		if (FAILED(hr)) {
			MessageBox(NULL, "DirectInput8の作成に失敗しました", "エラー", MB_OK);
		}
		hr = pDInput->CreateDevice(GUID_SysKeyboard, &pKeyDevice, nullptr);
		if (FAILED(hr)) {
			MessageBox(NULL, "Input Deviceの作成に失敗しました", "エラー", MB_OK);
		}
		pKeyDevice->SetDataFormat(&c_dfDIKeyboard);
		pKeyDevice->SetCooperativeLevel(hWnd, DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

		return S_OK;
	}

	void Update()
	{
		int tes = sizeof(BYTE) * 256;
		int tes2 = sizeof(keyState) / sizeof(BYTE);
		memcpy(prevState, keyState, tes2);

		pKeyDevice->Acquire();
		pKeyDevice->GetDeviceState(sizeof(keyState), &keyState);
	}

	bool IsKey(int keyCode)
	{
		if (keyState[keyCode] & 0x80 )
		{
			return true;
		}
		return false;
	}

	bool IsKeyDown(int keyCode)
	{
		//今は押してて、前回は押してない
		if (!(prevState[keyCode] & 0x80) && IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	bool IsKeyUp(int keyCode)
	{
		//今は押してなく、前回は押している
		if ((prevState[keyCode] & 0x80) && !IsKey(keyCode))
		{
			return true;
		}
		return false;
	}

	XMVECTOR GetMousePosition()
	{
		return mousePosition;
	}

	void SetMousePosition(int x, int y)
	{
		mousePosition = XMVectorSet((float)x, (float)y, 0, 0);
	}

	void Release()
	{
		SAFE_RELEASE(pKeyDevice);
		SAFE_RELEASE(pDInput);
	}
}