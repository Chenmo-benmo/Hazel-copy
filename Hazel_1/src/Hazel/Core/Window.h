#pragma once
//创建窗口
#include "hzpch.h"

#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"

namespace Hazel {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "Hazel_1 Engine",
					unsigned int width = 1280,
					unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{}
	};

	//表示基于Window的桌面系统界面
	class  Window
	{
	public:
		//接受任何一个 Evnet 类型引用参数并返回 void 的函数 的模板
		//定义事件回调函数类型
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//窗口属性
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0; //实现设置事件回调
		virtual void SetVSync(bool enabled) = 0;							//实现垂直同步
		virtual bool IsVSync() const = 0;									//查询是否启用垂直同步

		virtual void* GetNativeWindow() const = 0;

		//创建并返回一个 Window 对象的指针
		//如果不指定则为默认值
		static Window* Create(const WindowProps& props = WindowProps());
	};
}