#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"

class SystemTestRenderer final : public virtual IRenderer
{
private:

	// Resources
	const Image& backgroundimage;
	const Image& circleimage;
	const Image& darkcircleimage;

	// State
	int inputcount;
	Text inputcounttext;
	bool gate1front;
	bool gate2front;
	bool gate3front;
	bool gate4front;
	bool gate1back;
	bool gate2back;
	bool gate3back;
	bool gate4back;
	bool bridge1sensor;
	bool bridge2sensor;
	bool leftbutton;
	bool rightbutton;
	bool acceptbutton;
	bool cancelbutton;

public:

	SystemTestRenderer();

	void Begin();
	virtual void Render(Canvas& canvas) override;

	void CountInput();
	void SetGate1Front(bool on) { gate1front = on; }
	void SetGate2Front(bool on) { gate2front = on; }
	void SetGate3Front(bool on) { gate3front = on; }
	void SetGate4Front(bool on) { gate4front = on; }
	void SetGate1Back(bool on) { gate1back = on; }
	void SetGate2Back(bool on) { gate2back = on; }
	void SetGate3Back(bool on) { gate3back = on; }
	void SetGate4Back(bool on) { gate4back = on; }
	void SetBridge1Sensor(bool on) { bridge1sensor = on; }
	void SetBridge2Sensor(bool on) { bridge2sensor = on; }
	void SetLeftButton(bool on) { leftbutton = on; }
	void SetRightButton(bool on) { rightbutton = on; }
	void SetAcceptButton(bool on) { acceptbutton = on; }
	void SetCancelButton(bool on) { cancelbutton = on; }
};
