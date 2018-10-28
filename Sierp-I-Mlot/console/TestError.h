#pragma once

inline void ErrorExit(LPTSTR lpszFunction, LPTSTR lpszFile, int nLine,
	const wchar_t *call)
{
	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();
	if (dw == 0)
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

	DWORD bufSize = (lstrlen((LPCTSTR)call)
		+ lstrlen((LPCTSTR)lpMsgBuf)
		+ lstrlen((LPCTSTR)lpszFunction)
		+ lstrlen((LPCTSTR)lpszFile)
		+ 80) * sizeof(TCHAR);
	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT, bufSize);
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		L"Call: %s\nFunction: %s\nFile: %s\nLine: %d\nCode: %d\nMessage: %s",
		call, lpszFunction, lpszFile, nLine, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

#define _CHECK_ERROR(CALL) ErrorExit(_T(__FUNCTION__), _T(__FILE__), __LINE__, CALL)

inline void _chkerr_BOOL(BOOL result, const wchar_t *call)
{
	if (result == 0) { _CHECK_ERROR(call); }
}

inline void _chkerr_HANDLE(HANDLE result, const wchar_t *call)
{
	if (result == INVALID_HANDLE_VALUE) { _CHECK_ERROR(call); }
}

inline void _chkerr_HRESULT(HRESULT result, const wchar_t *call)
{
	if (FAILED(result)) { _CHECK_ERROR(call); }
}

inline void _chkerr_DWORD(DWORD result, const wchar_t *call)
{
	if (result) { _CHECK_ERROR(call); }
}

inline void _chkerr_PVOID(PVOID result, const wchar_t *call)
{
	if (result == NULL) { _CHECK_ERROR(call); }
}


#define CHKERR_BOOL(x) { _chkerr_BOOL(x, _T(#x)); }
#define CHKERR_HANDLE(x) { _chkerr_HANDLE(x, _T(#x)); }
#define CHKERR_HRESULT(x) { _chkerr_HRESULT(x, _T(#x)); }
#define CHKERR_DWORD(x) { _chkerr_DWORD(x, _T(#x)); }
#define CHKERR_PVOID(x) { _chkerr_PVOID(x, _T(#x)); }
