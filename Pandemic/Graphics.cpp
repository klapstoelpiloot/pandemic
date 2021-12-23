#include <utility>
#include <cassert>
#include <led-matrix.h>
#include <math.h>
#include "Tools.h"
#include "Graphics.h"
#include "Canvas.h"
#include "File.h"
#include "Main.h"
#include "lodepng.h"

Graphics::Graphics(const Configuration& cfg, bool showfps) :
	display(nullptr),
	displaycanvas(nullptr),
	brightness(cfg.GetInt("Graphics.Brightness", 100)),
	showfps(showfps),
	nextfpstime(Clock::now() + ch::seconds(10)),
	framescounted(0),
	frameindex(0)
{
	recordinterval = ch::microseconds(static_cast<int64_t>(std::roundl(1000000.0 / cfg.GetDouble("Graphics.RecordRate", 30))));

	// Check if running as root or with elevated privileges
	uid_t uid_me = getuid();
	uid_t uid_privs = geteuid();
	bool asroot = (uid_me < 0) || (uid_me != uid_privs);

	// Determine the type of hardware mapping to use. The PWM method can only be used with root privileges,
	// but we want to support 'normal' mode as well so that we can do remote debugging (which doesn't have root privileges).
	String hwmapping;
	if(asroot)
		hwmapping = "adafruit-hat-pwm";
	else
		hwmapping = "adafruit-hat";

	// Options for our display
	rgb_matrix::RGBMatrix::Options matrixoptions;
	rgb_matrix::RuntimeOptions runtimeoptions;
	matrixoptions.hardware_mapping = hwmapping;
	matrixoptions.rows = DISPLAY_HEIGHT;
	matrixoptions.cols = DISPLAY_WIDTH / DISPLAY_PANELS;
	matrixoptions.chain_length = DISPLAY_PANELS;
	matrixoptions.parallel = 1;
	matrixoptions.show_refresh_rate = false;
	matrixoptions.brightness = brightness;
	matrixoptions.pwm_bits = cfg.GetInt("Graphics.PWM_Bits", 11);
	matrixoptions.pwm_dither_bits = cfg.GetInt("Graphics.PWM_Dither_Bits", 0);
	matrixoptions.pwm_lsb_nanoseconds = cfg.GetInt("Graphics.PWM_LSB_Nanoseconds", 130);
	runtimeoptions.gpio_slowdown = cfg.GetInt("Graphics.GPIO_Slowdown", 4);

	// Initialize the matrix display
	std::cout << "Initializing graphics on " << hwmapping << "..." << std::endl;
	display = rgb_matrix::CreateMatrixFromOptions(matrixoptions, runtimeoptions);
	ENSURE(display != nullptr);
	display->set_luminance_correct(cfg.GetBool("Graphics.Luminance_Correct", true));

	// Get a canvas to draw on
	displaycanvas = display->CreateFrameCanvas();
	ENSURE(displaycanvas != nullptr);
}

Graphics::~Graphics()
{
	display->Clear();
	SAFE_DELETE(display);
	displaycanvas = nullptr;
}

void Graphics::SetBrightness(int b)
{
	brightness = std::clamp(b, 50, 100);
	display->SetBrightness(static_cast<uint8_t>(brightness));
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

	// Write all our renderbuffer pixels to the display canvas
	const Color* p = canvas.GetBuffer();
	for(int y = 0; y < DISPLAY_HEIGHT; y++)
	{
		for(int x = 0; x < DISPLAY_WIDTH; x++)
		{
			displaycanvas->SetPixel(x, y, p->r, p->g, p->b);
			p++;
		}
	}

	// Show the canvas on display
	displaycanvas = display->SwapOnVSync(displaycanvas);

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
