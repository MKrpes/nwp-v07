#include <windows.h>
#include <tchar.h>
#include "nwpwin.h"

#include <gdiplus.h>
using namespace Gdiplus;

class gdiplus_application : public vsite::nwp::application
{
	Gdiplus::GdiplusStartupInput startup_input;
	ULONG_PTR           token;
public:
	gdiplus_application() {
		Gdiplus::GdiplusStartup(&token, &startup_input, nullptr);
	}
	~gdiplus_application() {
		Gdiplus::GdiplusShutdown(token);
	}
};

class main_window : public vsite::nwp::window {
public:
	Image* img;
	std::wstring fileName;
	UINT frameCount = 1;
	UINT currentFrame = 0;
	UINT frameDelay = 0;
	UINT lastFrameTime = 0;
	void AdvanceToNextFrame() {
		if (GetTickCount64() - lastFrameTime > frameDelay) {
			currentFrame = (currentFrame + 1) % frameCount;
			img->SelectActiveFrame(&FrameDimensionTime, currentFrame);
			lastFrameTime = GetTickCount64();
			InvalidateRect(*this, NULL, false);
		}
	}
	bool isGIF(const TCHAR* filePath) {
		const TCHAR* dot = _tcsrchr(filePath, _T('.'));
		if (_tcsicmp(dot, _T(".gif")) == 0) {
			isGif = true;
			return true;
		}
		else {
			isGif = false;
			return false;
		}
	}
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
private:
	bool isGif = false;
};
