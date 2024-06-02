#include "main.h"
#include "rc.h"
#include <filesystem>
#include<string>
using tstring = std::basic_string<TCHAR>;

void main_window::on_paint(HDC hdc) 
{
	RECT rect;
	GetClientRect(*this, &rect);
	Gdiplus::RectF displayRect(rect.left, rect.top, rect.right, rect.bottom);
	HDC memDC = CreateCompatibleDC(hdc);
	HBITMAP memBitmap = CreateCompatibleBitmap(hdc, rect.right, rect.bottom);
	SelectObject(memDC, memBitmap);

	Gdiplus::Graphics graphics(memDC);
	graphics.DrawImage(img.get(), displayRect);

	Gdiplus::StringFormat t;
	t.SetAlignment(Gdiplus::StringAlignmentCenter);
	t.SetLineAlignment(Gdiplus::StringAlignmentFar);
	Gdiplus::Font font(_T("Arial"), 25, Gdiplus::FontStyleBold);
	Gdiplus::SolidBrush tForm(Gdiplus::Color::Black);
	graphics.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);

	displayRect.Width = rect.right - 5;
	displayRect.Height = rect.bottom - 5;
	tForm.SetColor(Gdiplus::Color::White);
	graphics.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);

	BitBlt(hdc, 0, 0, rect.right, rect.bottom, memDC, 0, 0, SRCCOPY);

	DeleteObject(memBitmap);
	DeleteDC(memDC);
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
	case ID_OPEN:
	{
		TCHAR path[MAX_PATH]; *path = 0;
		TCHAR filter[] = _T("Image Files (*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.bmp;*.emf)\0*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.bmp;*.emf\0All Files (*.*)\0*.*\0");
		OPENFILENAME ofn{ sizeof OPENFILENAME };
		ofn.hwndOwner = *this;
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = filter;
		ofn.Flags = OFN_HIDEREADONLY;
		if (::GetOpenFileName(&ofn)) {
			if (timer > 0) {
				::KillTimer(*this, 1);
				timer = 0;
			}
			img = std::make_unique<Gdiplus::Image>(path);
			fileName = std::filesystem::path(path).filename();
			if (isGIF(path)) {
				frameCount = img->GetFrameCount(&Gdiplus::FrameDimensionTime);
				::SetTimer(*this, 1, getFPS(), 0);
				timer = 1;
			}
		}
		break;
	}
		case ID_EXIT:
			DestroyWindow(*this);
			break;
	}
	InvalidateRect(*this, NULL, true);
}
void main_window::on_timer(int id)
{
	AdvanceToNextFrame();
	InvalidateRect(*this, NULL, false);
}


void main_window::on_destroy() 
{
	::PostQuitMessage(0);
}

int WINAPI _tWinMain(HINSTANCE instance, HINSTANCE, LPTSTR, int)
{
	gdiplus_application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP 7"), (int64_t)LoadMenu(instance, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
