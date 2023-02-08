#pragma once
#include "Configuration.h"
#include "IRenderer.h"
#include "Canvas.h"
#include "IGraphicsHAL.h"

class Graphics final
{
private:

	// The hardware interface to display the graphics
	IGraphicsHAL* hal;

	// The canvas to which a renderer renders.
	Canvas canvas;

	// The renderers which will render our image.
	// We swap out renderers for different screens/layouts/effects we want to display.
	// Note that we do not take ownership of the renderer instance.
	// Multiple renderers can modify the canvas in the order they were added.
	vector<IRenderer*> renderers;

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
	int GetBrightness() const { return hal->GetBrightness(); }
	void SetBrightness(int b) { hal->SetBrightness(b); }
	void Record(String path);

	// This renders the canvas and displays it
	void Present();
};
