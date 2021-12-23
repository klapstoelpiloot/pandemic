#pragma once
#include "Configuration.h"
#include "IRenderer.h"
#include "Canvas.h"

namespace rgb_matrix
{
	class RGBMatrix;
	class FrameCanvas;
}

class Graphics final
{
private:

	// Matrix display
	rgb_matrix::RGBMatrix* display;

	// The buffer to which we convert our rasterized image.
	rgb_matrix::FrameCanvas* displaycanvas;

	// The canvas to which a renderer renders.
	Canvas canvas;

	// The renderers which will render our image.
	// We swap out renderers for different screens/layouts/effects we want to display.
	// Note that we do not take ownership of the renderer instance.
	// Multiple renderers can modify the canvas in the order they were added.
	vector<IRenderer*> renderers;

	// Brightness (0-100)
	int brightness;

	// FPS measuring
	bool showfps;
	TimePoint nextfpstime;
	int framescounted;

	// Recording
	String recordpath;
	vector<byte> recordbuffer;
	TimePoint nextrecordtime;
	ch::microseconds recordinterval;
	int frameindex;

	// Methods
	void WriteRecordedFrame();

public:

	// Constructor/destructor
	Graphics(const Configuration& cfg, bool showfps);
	~Graphics();

	// Methods
	inline Canvas& GetCanvas() { return canvas; }
	inline const Canvas& GetCanvas() const { return canvas; }
	void ClearRenderers();
	void AddRenderer(IRenderer* r);
	void RemoveRenderer(IRenderer* r);
	int GetBrightness() const { return brightness; }
	void SetBrightness(int b);
	void Record(String path);

	// This renders the canvas and displays it
	void Present();
};
