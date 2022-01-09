#pragma once
#include "Tools.h"
#include "IImage.h"

class LoadingImage final : public virtual IImage
{
private:

public:

	LoadingImage();

	virtual bool HasColors() const override final { return true; }
	virtual int Width() const override final;
	virtual int Height() const override final;
	virtual Size GetSize() const override final { return Size(Width(), Height()); }
	virtual const byte* ByteData() const override final;
	virtual const Color* ColorData() const override final;
	virtual MonoSampler GetMonoSampler() const override final;
	virtual ColorSampler GetColorSampler() const override final;
};
