#include "main.h"

int screenWidth = 160;
int screenHeight = 80;
int fontw = 8;
int fonth = 8;

HANDLE hConsole;
SMALL_RECT recWindows;

CHAR_INFO* m_bufScreen;




std::atomic<bool> main::m_bAtomActive(false);
std::condition_variable main::m_cvGameFinished;
std::mutex main::m_muxGame;

int Error(const wchar_t* msg)
{
	wchar_t buf[256];
	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buf, 256, NULL);
	//SetConsoleActiveScreenBuffer(m_hOriginalConsole);
	wprintf(L"ERROR: %s\n\t%s\n", msg, buf);
	return 0;
}

static BOOL CloseHandler(DWORD evt)
{
	// Note this gets called in a seperate OS thread, so it must
	// only exit when the game has finished cleaning up, or else
	// the process will be killed before OnUserDestroy() has finished
	if (evt == CTRL_CLOSE_EVENT)
	{
		main::m_bAtomActive = false;

		// Wait for thread to be exited
		std::unique_lock<std::mutex> ul(main::m_muxGame);
		main::m_cvGameFinished.wait(ul);
	}
	return true;
}


int main()
{


	recWindows = { 0,0,1,1 };
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	COORD coord = { (short)screenWidth, (short)screenHeight };
	if (!SetConsoleScreenBufferSize(hConsole, coord))
		Error(L"SetConsoleScreenBufferSize");


	if (!SetConsoleActiveScreenBuffer(hConsole))
		return Error(L"SetConsoleActiveScreenBuffer");


	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = fontw;
	cfi.dwFontSize.Y = fonth;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;

	/*	DWORD version = GetVersion();
		DWORD major = (DWORD)(LOBYTE(LOWORD(version)));
		DWORD minor = (DWORD)(HIBYTE(LOWORD(version)));*/

		//if ((major > 6) || ((major == 6) && (minor >= 2) && (minor < 4)))		
		//	wcscpy_s(cfi.FaceName, L"Raster"); // Windows 8 :(
		//else
		//	wcscpy_s(cfi.FaceName, L"Lucida Console"); // Everything else :P

		//wcscpy_s(cfi.FaceName, L"Liberation Mono");
	wcscpy_s(cfi.FaceName, L"Consolas");
	if (!SetCurrentConsoleFontEx(hConsole , false, &cfi))
		return Error(L"SetCurrentConsoleFontEx");

	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (!GetConsoleScreenBufferInfo(hConsole, &csbi))
		return Error(L"GetConsoleScreenBufferInfo");
	if (screenHeight > csbi.dwMaximumWindowSize.Y)
		return Error(L"Screen Height / Font Height Too Big");
	if (screenWidth > csbi.dwMaximumWindowSize.X)
		return Error(L"Screen Width / Font Width Too Big");

	// Set Physical Console Window Size
	recWindows = { 0, 0, (short)screenWidth - 1, (short)screenHeight - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &recWindows))
		return Error(L"SetConsoleWindowInfo");

	// Allocate memory for screen buffer
	m_bufScreen = new CHAR_INFO[screenWidth * screenHeight];
	memset(m_bufScreen, 0, sizeof(CHAR_INFO) * screenWidth * screenHeight);

	SetConsoleCtrlHandler((PHANDLER_ROUTINE)CloseHandler, TRUE);
	return 1;




}