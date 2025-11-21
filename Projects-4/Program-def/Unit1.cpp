//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"

#include <System.IOUtils.hpp>


#include <ShlObj.h>
#include <ShellAPI.hpp>

#include <Windows.h>

#include <IdHashSHA.hpp>
#include <IdGlobal.hpp>
#include <memory>   // для std::unique_ptr

#include <System.NetEncoding.hpp>

#include <Wincrypt.h>
#include <Registry.hpp>   // TRegistry

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#pragma comment(lib, "Crypt32.lib")
TForm1 *Form1;


//---------------------------------------------------------------------------
UnicodeString GetUserNameSimple()
{
	wchar_t buffer[256];
	DWORD size = 256;
	GetUserNameW(buffer, &size);
	return ("User Name: " +UnicodeString(buffer));
}

UnicodeString GetComputerNameSimple()
{
	wchar_t buffer[256];
	DWORD size = 256;

	if (GetComputerNameW(buffer, &size))
		return ("Compuetr Name: " + UnicodeString(buffer));

	return "";
}
UnicodeString GetWindowsFolder()
{
	wchar_t buffer[MAX_PATH];
	UINT len = GetWindowsDirectoryW(buffer, MAX_PATH);
	return  ("Windows dirictory: " + UnicodeString(buffer));
}

UnicodeString GetSystem32Folder()
{
	wchar_t buffer[MAX_PATH];
	UINT len = GetSystemDirectoryW(buffer, MAX_PATH);
	return ("system dirictory: " + UnicodeString(buffer));
}


#include <Windows.h>

UnicodeString DecodeKeyboardType(int type)
{
	switch (type)
	{
		case 1:  return "IBM PC/XT або AT";
		case 2:  return "Olivetti keyboard";
		case 3:  return "IBM 101/102-клавішна";
		case 4:  return "Nokia 1050";
		case 5:  return "Nokia 9140";
		case 6:  return "Japanese keyboard";
		case 7:  return "Korean keyboard";
		case 81: return "USB HID keyboard";;
		default: return "Unknown type (" + IntToStr(type) + ")";
	}
}

UnicodeString DecodeKeyboardSubtype(int subtype)
{
	switch (subtype)
	{
		case 0:  return "Generic ";
		case 1:  return "Manufacturer specific";
		default: return "Unknown subtype (" + IntToStr(subtype) + ")";
	}
}

UnicodeString DecodeFunctionKeys(int fnCount)
{
	switch (fnCount)
	{
		case 12: return "12 function keys (F1–F12)";
		case 10: return "10 function keys (old style)";
		default: return IntToStr(fnCount) + " function keys";
	}
}


UnicodeString  GetKeyboardType(){

	int type    = GetKeyboardType(0);
	int subtype = GetKeyboardType(1);
	int fnKeys  = GetKeyboardType(2);

	UnicodeString msg;
	msg += "Keyboard Type:       " + DecodeKeyboardType(type);
	msg += "Keyboard Subtype:    " + DecodeKeyboardSubtype(subtype);
//	msg += "Function Keys:     " + DecodeFunctionKeys(fnKeys);

	return msg;
}

UnicodeString GetRamSize(){

	MEMORYSTATUSEX statex;
	statex.dwLength = sizeof(statex);

	if (GlobalMemoryStatusEx(&statex))
	{
		// Загальна кількість RAM (в байтах → ГБ)
		__int64 totalRAM = statex.ullTotalPhys;
		double totalGB = (double)totalRAM / (1024.0 * 1024.0 * 1024.0);

		// Вільна память
//        __int64 freeRAM = statex.ullAvailPhys;
//        double freeGB = (double)freeRAM / (1024.0 * 1024.0 * 1024.0);

		UnicodeString info;
		info += "all RAM: " + FloatToStrF(totalRAM, ffFixed, 17, 0) + " Bytes";
//		info += "all RAM: " + FloatToStrF(totalGB, ffFixed, 10, 2) + " GB\n";
//		info += "Вільна RAM:   " + FloatToStrF(freeGB, ffFixed, 10, 2) + " GB\n";
//		info += "Використано:  " + IntToStr((int)statex.dwMemoryLoad) + "%";

		return info;
	}
	else
	{
		return("error access for RAM!");
	}


}

UnicodeString GetDiskSpace(){
	// Отримуємо шлях до папки, де запущена програма
	UnicodeString appPath = ExtractFilePath(Application->ExeName);

	// Переводимо в формат WinAPI (WideString → WCHAR*)
	LPCWSTR path = appPath.w_str();

	// Змінні для дискового простору
	ULARGE_INTEGER freeBytesAvailable;  // доступно користувачу
	ULARGE_INTEGER totalBytes;          // загальний обсяг диску
	ULARGE_INTEGER totalFreeBytes;      // загальний вільний простір

	// Виклик API
	if (GetDiskFreeSpaceExW(path, &freeBytesAvailable, &totalBytes, &totalFreeBytes))
	{
		double totalGB = (double)totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
//		double totalGB = (double)totalBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
//		double freeGB  = (double)totalFreeBytes.QuadPart / (1024.0 * 1024.0 * 1024.0);
//		double availGB = (double)freeBytesAvailable.QuadPart / (1024.0 * 1024.0 * 1024.0);

		UnicodeString msg;
		msg += "Disck size: " + FloatToStrF(totalGB, ffFixed, 19, 0) + "GB";
//		msg += "📀 Диск запуску програми: " + appPath + "\n\n";
//		msg += "🔹 Загальний обсяг: " + FloatToStrF(totalGB, ffFixed, 10, 2) + " GB\n";
//		msg += "🔹 Вільно фізично:  " + FloatToStrF(freeGB, ffFixed, 10, 2) + " GB\n";
//		msg += "🔹 Доступно користувачу: " + FloatToStrF(availGB, ffFixed, 10, 2) + " GB";

		return msg;
	}
	else
	{
		return "❌error disk size!";
	}
}






UnicodeString GetMonitorHeight(){
	return ("Monitor height: " + IntToStr(GetSystemMetrics(SM_CYSCREEN)) + " pixels");
}

//////////////////////////////////////////////////////


UnicodeString BytesToHexReg(const BYTE* data, DWORD len)
{
	UnicodeString res;
	for (DWORD i = 0; i < len; ++i)
		res += IntToHex((int)data[i], 2);
	return res.UpperCase();
}

// HEX-рядок → байти (для перевірки підпису)
bool HexToBytesReg(const UnicodeString &hex, BYTE* &outData, DWORD &outLen)
{
	int hexLen = hex.Length();
	if (hexLen % 2 != 0) return false;

	outLen = hexLen / 2;
	outData = new BYTE[outLen];

	for (int i = 0; i < outLen; ++i)
    {
		UnicodeString pair = L"0x" + hex.SubString(i * 2 + 1, 2);
		outData[i] = (BYTE)StrToInt(pair);
	}
	return true;
}

/////////////////////////////////////////////////////////////

bool GetOrCreateUserKey(HCRYPTPROV &hProv)
{
	UnicodeString container = L"Software\\Kravchenko"; // можна замінити на своє

    // Спробуємо відкрити існуючий контейнер
	if (!CryptAcquireContextW(&hProv,
                              container.c_str(),
                              NULL,
                              PROV_RSA_FULL,
                              0))
    {
        // Якщо контейнера нема — створюємо новий
        if (GetLastError() == NTE_BAD_KEYSET)
        {
            if (!CryptAcquireContextW(&hProv,
                                      container.c_str(),
                                      NULL,
                                      PROV_RSA_FULL,
                                      CRYPT_NEWKEYSET))
                return false;
        }
        else
            return false;
    }

    // Переконаємось, що є ключ підпису (AT_SIGNATURE)
    HCRYPTKEY hKey = 0;
	if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hKey))
    {
        // Немає — створюємо
        if (!CryptGenKey(hProv, AT_SIGNATURE, 0, &hKey))
        {
            CryptReleaseContext(hProv, 0);
            return false;
        }
    }

    if (hKey)
        CryptDestroyKey(hKey);

    return true;
}


bool LoadFromRegistryAndVerify(const UnicodeString &data)
{
    // 1) Читаємо підпис з реєстру
    std::unique_ptr<TRegistry> reg(new TRegistry(KEY_READ));
    reg->RootKey = HKEY_CURRENT_USER;

    if (!reg->OpenKeyReadOnly(L"Software\\Kravchenko"))
    {
		ShowMessage(L"Розділ реєстру не знайдено");
		Application->Terminate();
        return false;
    }

    UnicodeString sigHex;
    try {
        sigHex = reg->ReadString(L"Signature");
    }
    catch (...)
    {
        reg->CloseKey();
		ShowMessage(L"Параметр Signature не знайдено");
		Application->Terminate();
        return false;
    }
    reg->CloseKey();

    // 2) Конвертуємо HEX → байти підпису
    BYTE *sigBuf = nullptr;
    DWORD sigLen = 0;
	if (!HexToBytesReg(sigHex, sigBuf, sigLen))
    {
		ShowMessage(L"Помилка формату підпису");
		Application->Terminate();
        return false;
    }

    // 3) Знову готуємо контекст + ключ
    HCRYPTPROV hProv = 0;
    if (!GetOrCreateUserKey(hProv))
    {
        delete[] sigBuf;
		ShowMessage(L"Помилка криптопровайдера");
		Application->Terminate();
        return false;
    }


    // 4) Отримуємо ПУБЛІЧНИЙ ключ (з контейнера)
    HCRYPTKEY hPubKey = 0;
    if (!CryptGetUserKey(hProv, AT_SIGNATURE, &hPubKey))
    {
        delete[] sigBuf;
        CryptReleaseContext(hProv, 0);
		ShowMessage(L"Не вдалося отримати ключ користувача");
		Application->Terminate();
        return false;
    }

    // 5) Створюємо той самий SHA-1 хеш від data
    HCRYPTHASH hHash = 0;
    if (!CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash))
    {
        CryptDestroyKey(hPubKey);
        CryptReleaseContext(hProv, 0);
        delete[] sigBuf;
        return false;
    }

    AnsiString aData = AnsiString(data);
    if (!CryptHashData(hHash,
                       (BYTE*)aData.c_str(),
                       aData.Length(),
                       0))
    {
        CryptDestroyHash(hHash);
        CryptDestroyKey(hPubKey);
        CryptReleaseContext(hProv, 0);
        delete[] sigBuf;
        return false;
    }

    // 6) Перевіряємо підпис
    BOOL ok = CryptVerifySignatureW(hHash,
                                    sigBuf,
                                    sigLen,
                                    hPubKey,
                                    NULL,
                                    0);

    delete[] sigBuf;
    CryptDestroyHash(hHash);
    CryptDestroyKey(hPubKey);
    CryptReleaseContext(hProv, 0);

    if (ok)
    {
        ShowMessage(L"Підпис вірний (дані не змінені)");
        return true;
    }
    else
    {
		ShowMessage(L"Підпис НЕ вірний (дані або ключ змінені)");
		Application->Terminate();
        return false;
    }
}


//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	UnicodeString devInfo = GetUserNameSimple();
	devInfo += GetComputerNameSimple();
	devInfo += GetWindowsFolder();
	devInfo += GetSystem32Folder();
	devInfo += GetKeyboardType();
	devInfo += GetMonitorHeight();
	devInfo += GetRamSize();
	devInfo += GetDiskSpace();

    UnicodeString msg1 = GetUserNameSimple();
	msg1 += "\n"+  GetComputerNameSimple();
	msg1 += "\n"+  GetWindowsFolder();
	msg1 += "\n"+  GetSystem32Folder();
	msg1 += "\n"+  GetKeyboardType();
	msg1 += "\n"+  GetMonitorHeight();
	msg1 += "\n"+  GetRamSize();
	msg1 += "\n"+  GetDiskSpace();
	ShowMessage(msg1);

	UnicodeString info = devInfo;


	LoadFromRegistryAndVerify(info);
}
//---------------------------------------------------------------------------
