#pragma once

inline void ErrorExit(LPCTSTR lpszFunction, LPCTSTR lpszFile, int nLine,
	const wchar_t *call, bool chck_last_err)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	if ((chck_last_err && dw == 0) || dw == ERROR_IO_PENDING)
	{
		return;
	}

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	DWORD bufSize = (lstrlen(call)
		+ lstrlen((LPCTSTR)lpMsgBuf)
		+ lstrlen(lpszFunction)
		+ lstrlen(lpszFile)
		+ 80) * sizeof(TCHAR);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, bufSize);
	StringCchPrintf((STRSAFE_LPWSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		L"Call: %s\nFunction: %s\nFile: %s\nLine: %d\nCode: %d\nMessage: %s",
		call, lpszFunction, lpszFile, nLine, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
 	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

#define _CHECK_ERROR(CALL, CHCK_LAST_ERR) ErrorExit(_T(__FUNCTION__), _T(__FILE__), __LINE__, CALL, CHCK_LAST_ERR)

inline void _chkerr_BOOL(BOOL result, const wchar_t *call, bool chck_last_err = true)
{
	if (result == 0) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_HANDLE(HANDLE result, const wchar_t *call, bool chck_last_err = true)
{
	if (result == INVALID_HANDLE_VALUE) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_HRESULT(HRESULT result, const wchar_t *call, bool chck_last_err = true)
{
	if (FAILED(result)) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_DWORD(DWORD result, const wchar_t *call, bool chck_last_err = true)
{
	if (result) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_PVOID(PVOID result, const wchar_t *call, bool chck_last_err = true)
{
	if (result == NULL) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_MCI(DWORD result, const wchar_t *call, bool chck_last_err = true)
{
	auto err = LOWORD(result);
	auto device_id = HIWORD(result);

	char error_message[512]{};

	mciGetErrorStringA(result, error_message, 512);

	if (result != 0) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_MUTEX(DWORD result, const wchar_t *call, bool chck_last_err = true)
{
	if (!result) { _CHECK_ERROR(call, chck_last_err); }
}

inline void _chkerr_CHKERR_DWORD_DeleteTimerQueueTimer(BOOL result, const wchar_t *call, bool chck_last_err = true)
{
	if (result == 0) { _CHECK_ERROR(call, chck_last_err); }
}

#define CHKERR_BOOL(x) { _chkerr_BOOL(x, _T(#x)); }
#define CHKERR_HANDLE(x) { _chkerr_HANDLE(x, _T(#x)); }
#define CHKERR_HRESULT(x) { _chkerr_HRESULT(x, _T(#x)); }
#define CHKERR_DWORD(x) { _chkerr_DWORD(x, _T(#x)); }
#define CHKERR_PVOID(x) { _chkerr_PVOID(x, _T(#x)); }
#define CHKERR_MCI(x) { _chkerr_MCI(x, _T(#x), false); }
#define CHKERR_MUTEX(x) { _chkerr_MUTEX(x, _T(#x)); }

#define CHKERR_DWORD_WLASTERR(x) { _chkerr_DWORD(x, _T(#x), false); }
#define CHKERR_DWORD_DeleteTimerQueueTimer(x) { _chkerr_CHKERR_DWORD_DeleteTimerQueueTimer(x, _T(#x), false); }
