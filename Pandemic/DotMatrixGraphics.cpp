#include <utility>
#include <cassert>
#include <math.h>
#include "Tools.h"
#include "DotMatrixGraphics.h"
#include "Configuration.h"

DotMatrixGraphics::DotMatrixGraphics(const Configuration& cfg) :
	display(nullptr),
	displaycanvas(nullptr),
	brightness(cfg.GetInt("Graphics.Brightness", 100))
{
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

DotMatrixGraphics::~DotMatrixGraphics()
{
	display->Clear();
	SAFE_DELETE(display);
	displaycanvas = nullptr;
}

void DotMatrixGraphics::Present(Canvas& sourcecanvas)
{
	// Write all our renderbuffer pixels to the display canvas
	const Color* p = sourcecanvas.GetBuffer();
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
}

void DotMatrixGraphics::SetBrightness(int b)
{
	brightness = std::clamp(b, 50, 100);
	display->SetBrightness(static_cast<uint8_t>(brightness));
}
