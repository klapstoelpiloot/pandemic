#include <utility>
#include <cassert>
#ifdef RPI
#include "DotMatrixGraphics.h"
#endif
#ifdef X86
#include "X11Graphics.h"
#endif
#include <math.h>
#include "Tools.h"
#include "Graphics.h"
#include "Canvas.h"
#include "File.h"
#include "lodepng.h"

Graphics::Graphics(const Configuration& cfg, bool showfps) :
	hal(nullptr),
	showfps(showfps),
	nextfpstime(Clock::now() + ch::seconds(10)),
	framescounted(0),
	frameindex(0)
{
	recordinterval = ch::microseconds(static_cast<int64_t>(std::roundl(1000000.0 / cfg.GetDouble("Graphics.RecordRate", 30))));

	// Choose the graphics implementation depending on the hardware it was built for.
	#ifdef RPI
		hal = new DotMatrixGraphics(cfg);
	#endif
	#ifdef X86
		hal = new X11Graphics(cfg);
	#endif
}

Graphics::~Graphics()
{
	SAFE_DELETE(hal);
}

void Graphics::Record(String path)
{
	recordpath = path;
	nextrecordtime = Clock::now() + recordinterval;
}

void Graphics::ClearRenderers()
{
	renderers.clear();
}

void Graphics::AddRenderer(IRenderer* r)
{
	REQUIRE(r != nullptr);
	auto it = std::find(renderers.begin(), renderers.end(), r);
	if(it == renderers.end())
		renderers.push_back(r);
}

void Graphics::RemoveRenderer(IRenderer* r)
{
	REQUIRE(r != nullptr);
	auto it = std::find(renderers.begin(), renderers.end(), r);
	if(it != renderers.end())
		renderers.erase(it);
}

// This renders the canvas and displays it
void Graphics::Present()
{
	// Clear the screen
	canvas.Clear(BLACK);

	// Let the renderers draw their art
	for(IRenderer* r : renderers)
		r->Render(canvas);

	// Show the canvas on display
	hal->Present(canvas);

	// Measure FPS
	framescounted++;
	if(showfps && (Clock::now() >= nextfpstime))
	{
		std::cout << "FPS: " << (static_cast<float>(framescounted) / 10.0f) << std::endl;
		framescounted = 0;
		nextfpstime += ch::seconds(10);
	}

	// Record frames
	if(recordpath.Length() > 0)
	{
		TimePoint t = Clock::now();
		if(t >= nextrecordtime)
		{
			// Repeat the last frame if we skipped over too much time
			while(t >= (nextrecordtime + recordinterval))
			{
				WriteRecordedFrame();
				nextrecordtime += recordinterval;
			}

			// Record a frame
			recordbuffer.clear();
			lodepng::encode(recordbuffer, reinterpret_cast<const unsigned char*>(canvas.GetBuffer()), DISPLAY_WIDTH, DISPLAY_HEIGHT);
			WriteRecordedFrame();
			nextrecordtime += recordinterval;
		}
	}
}

void Graphics::WriteRecordedFrame()
{
	String frameindexstr = String::From(frameindex);
	String strpadding('0', 8 - frameindexstr.Length());
	String filename = File::CombinePath(recordpath, "Frame-" + strpadding + frameindexstr + ".png");
	lodepng::save_file(recordbuffer, filename.stl());
	frameindex++;
}
