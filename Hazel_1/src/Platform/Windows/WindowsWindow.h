#pragma once

#include "Hazel/Core/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

#include <GLFW/glfw3.h>

namespace Hazel {

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		//窗口属性
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; } //设置事件回调函数
		void SetVSync(bool enable) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;

		//存储所有实际数据的地方
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			//储存回调函数
			//回调函数灵活、可调，能再不同地方被赋予不同的回调函数
			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}