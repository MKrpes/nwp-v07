#include "main.h"
#include "rc.h"
#include <filesystem>
#include<string>
#include <iostream>

using namespace Gdiplus;




void main_window::on_paint(HDC hdc) 
{
	RECT rc;
	GetClientRect(*this, &rc);
	Graphics graphics(hdc);
	//HDC memDC = CreateCompatibleDC(hdc);
	Pen pen(Color(255, 255, 0, 0), 2);
	Gdiplus::RectF displayRect(rc.left, rc.top, rc.right, rc.bottom);
	graphics.DrawImage(img, displayRect);

	Gdiplus::StringFormat t;
	t.SetAlignment(Gdiplus::StringAlignmentCenter);
	t.SetLineAlignment(Gdiplus::StringAlignmentFar);
	Gdiplus::Font font(_T("Arial"), 25, Gdiplus::FontStyleBold);
	Gdiplus::SolidBrush tForm(Gdiplus::Color::Black);
	graphics.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);


	displayRect.Width = rc.right - 5;
	displayRect.Height = rc.bottom - 5;
	tForm.SetColor(Gdiplus::Color::White);
	graphics.DrawString(fileName.c_str(), -1, &font, displayRect, &t, &tForm);
	if (img != NULL && isGif) {
		AdvanceToNextFrame();
	}
}

void main_window::on_command(int id) 
{
	switch (id) 
	{
	case ID_OPEN:
	{
		if (timer > 0) {
			::KillTimer(*this, 1);
			timer = 0;
		}
		TCHAR path[MAX_PATH]; *path = 0;
		TCHAR filter[] = _T("Image Files (*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.bmp;*.emf)\0*.jpg;*.jpeg;*.png;*.gif;*.tiff;*.bmp;*.emf\0All Files (*.*)\0*.*\0");
		OPENFILENAME ofn{ sizeof OPENFILENAME };
		ofn.hwndOwner = *this;
		ofn.lpstrFile = path;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = filter;
		ofn.Flags = OFN_HIDEREADONLY;
		if (::GetOpenFileName(&ofn)) {
			img = Image::FromFile(path);
			fileName = std::filesystem::path(path).filename();
			if (isGIF(path)) {
				frameCount = img->GetFrameCount(&FrameDimensionTime);

				::SetTimer(*this, 1, getFPS(), 0);
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
