#pragma once
#include "Tools.h"
#include "IRenderer.h"
#include "Text.h"

class RemovePuckRenderer final : public virtual IRenderer
{
private:

	// Members
	const Image& bigcross;
	Text bigtext;
	Text bottomtext;
	const Image& texture;

public:

	RemovePuckRenderer();

	virtual void Render(Canvas& canvas) override;
};
