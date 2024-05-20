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
	int timer{ 0 };
	UINT frameCount = 1;
	UINT currentFrame = 0;
	UINT frameDelay = 0;
	UINT lastFrameTime = 0;
	void AdvanceToNextFrame() {
		if (GetTickCount64() - lastFrameTime > frameDelay) {
			currentFrame = (currentFrame + 1) % frameCount;
			img->SelectActiveFrame(&FrameDimensionTime, currentFrame);
			lastFrameTime = GetTickCount64();
		}
	}
	UINT getFPS(){
		UINT totalDuration = 0;
		UINT size = img->GetPropertyItemSize(PropertyTagFrameDelay);
		PropertyItem* propertyItem = (PropertyItem*)malloc(size);
		img->GetPropertyItem(PropertyTagFrameDelay, size, propertyItem);
		for (UINT i = 0; i < frameCount; ++i) {
			totalDuration += ((UINT*)propertyItem->value)[i] * 10; 
		}
		return totalDuration / frameCount;
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
	virtual void on_timer(int id) override;
	void on_destroy() override;
private:
	bool isGif = false;
};
