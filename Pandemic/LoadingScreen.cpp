#include "LoadingScreen.h"

LoadingScreen::LoadingScreen()
{
}

void LoadingScreen::Render(Canvas& canvas)
{
	Point p((canvas.GetSize().width - image.Width()) / 2,
	        (canvas.GetSize().height - image.Height()) / 2);
	canvas.DrawColorImage(p, image);
}
