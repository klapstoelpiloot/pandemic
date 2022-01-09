#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "LoadingImage.h"

class LoadingScreen final : public virtual IRenderer
{
private:

	// Members
	LoadingImage image;

public:

	LoadingScreen();

	virtual void Render(Canvas& canvas) override;
};
