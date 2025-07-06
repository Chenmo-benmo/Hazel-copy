#pragma once

#include "Hazel/Core/Core.h"
#include "hzpch.h"

namespace Hazel {
	//当前的事件处理机制是阻塞式的，这意味着当事件发生时，它会立即被分发并需要立即处理

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, //窗口关闭、窗口大小调整、窗口获得/失去焦点、窗口移动
		AppTick, AppUpdate, AppRender, //应用程序的滴答事件（通常用于固定频率的更新）、应用程序更新、应用程序渲染
		KeyPressed, KeyReleased, KeyTyped, //键盘按键按下/释放/打字
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled //鼠标按钮按下/释放、鼠标移动和鼠标滚动
	};

	enum EventCategory
	{
		None = 0,
		EventCategoryApplication	= BIT(0),
		EventCategoryInput			= BIT(1),
		EventCategoryKeyboard		= BIT(2),
		EventCategoryMouse			= BIT(3),
		EventCategoryMouseButton	= BIT(4),
	};

//预处理器会将 ## 前后标识符连接成一个新标识符
//预处理器会将 # 后面的宏参数替换成对应的字符串表示
//只能在派生类里使用，不能在基类使用
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType()	{ return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class  Event
	{ //基类
		//只能访问声明友元的类，基类和派生类以及二层友元都无法进行访问
		//需要访问私有变量时，友元是个可行的选择
		friend class EventDispatcher;
	public:
		//纯虚函数意味着在派生类里必须被实现
		virtual EventType GetEventType() const = 0;
		virtual const char* GetName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string ToString() const { return GetName(); }

		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;
		}

	public:
		bool m_Handled = false;
	};

	class EventDispatcher
	{ //测试员
		//EventFn 是一个可以接受 任何一个T类型引用参数并返回bool值的函数 的模板
		//也可以接受 返回bool值的lambda函数
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//比较当前事件 m_Event 和模板T指定的类型
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//*(T*)& 将m_Event强制转换为 T* 类型
				//调用 func() 函数并将处理结果(bool)赋给 m_Handled
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator << (std::ostream& os, const Event& e)
	{ //HZ_CORE_... 宏内使用 << 打印日志，这里重载 << 即可以直接在宏内使用结构体 Event 和它的所有派生类
		return os << e.ToString();
	}
}