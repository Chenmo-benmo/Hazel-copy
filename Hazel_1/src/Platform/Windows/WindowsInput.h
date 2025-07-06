#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Input.h"

namespace Hazel {

	class WindowsInput : public Input
	{
	protected:
		//此处 virtual 无任何实际意义，仅作标记
		virtual bool IsKeyPressedImpl(int keycode) override;

		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}