#include <ZEngine/Window/ZWindow.h>
#include <Windows.h>

ZWindow::ZWindow()
{
	WNDCLASSEX wc = { };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpszClassName = L"ZenithWindow";
}

ZWindow::~ZWindow()
{
}
