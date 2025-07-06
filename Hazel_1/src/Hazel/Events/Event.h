#pragma once

#include "Hazel/Core/Core.h"
#include "hzpch.h"

namespace Hazel {
	//��ǰ���¼��������������ʽ�ģ�����ζ�ŵ��¼�����ʱ�������������ַ�����Ҫ��������

	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved, //���ڹرա����ڴ�С���������ڻ��/ʧȥ���㡢�����ƶ�
		AppTick, AppUpdate, AppRender, //Ӧ�ó���ĵδ��¼���ͨ�����ڹ̶�Ƶ�ʵĸ��£���Ӧ�ó�����¡�Ӧ�ó�����Ⱦ
		KeyPressed, KeyReleased, KeyTyped, //���̰�������/�ͷ�/����
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled //��갴ť����/�ͷš�����ƶ���������
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

//Ԥ�������Ὣ ## ǰ���ʶ�����ӳ�һ���±�ʶ��
//Ԥ�������Ὣ # ����ĺ�����滻�ɶ�Ӧ���ַ�����ʾ
//ֻ������������ʹ�ã������ڻ���ʹ��
#define EVENT_CLASS_TYPE(type) static EventType GetStaticType()	{ return EventType::##type; }\
								virtual EventType GetEventType() const override { return GetStaticType(); }\
								virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class  Event
	{ //����
		//ֻ�ܷ���������Ԫ���࣬������������Լ�������Ԫ���޷����з���
		//��Ҫ����˽�б���ʱ����Ԫ�Ǹ����е�ѡ��
		friend class EventDispatcher;
	public:
		//���麯����ζ��������������뱻ʵ��
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
	{ //����Ա
		//EventFn ��һ�����Խ��� �κ�һ��T�������ò���������boolֵ�ĺ��� ��ģ��
		//Ҳ���Խ��� ����boolֵ��lambda����
		template<typename T>
		using EventFn = std::function<bool(T&)>;
	public:
		EventDispatcher(Event& event)
			: m_Event(event)
		{}

		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			//�Ƚϵ�ǰ�¼� m_Event ��ģ��Tָ��������
			if (m_Event.GetEventType() == T::GetStaticType())
			{
				//*(T*)& ��m_Eventǿ��ת��Ϊ T* ����
				//���� func() ��������������(bool)���� m_Handled
				m_Event.m_Handled = func(*(T*)&m_Event);
				return true;
			}
			return false;
		}
	private:
		Event& m_Event;
	};

	inline std::ostream& operator << (std::ostream& os, const Event& e)
	{ //HZ_CORE_... ����ʹ�� << ��ӡ��־���������� << ������ֱ���ں���ʹ�ýṹ�� Event ����������������
		return os << e.ToString();
	}
}