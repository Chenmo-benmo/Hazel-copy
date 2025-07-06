#pragma once
//��������
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

	//��ʾ����Window������ϵͳ����
	class  Window
	{
	public:
		//�����κ�һ�� Evnet �������ò��������� void �ĺ��� ��ģ��
		//�����¼��ص���������
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		//��������
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0; //ʵ�������¼��ص�
		virtual void SetVSync(bool enabled) = 0;							//ʵ�ִ�ֱͬ��
		virtual bool IsVSync() const = 0;									//��ѯ�Ƿ����ô�ֱͬ��

		virtual void* GetNativeWindow() const = 0;

		//����������һ�� Window �����ָ��
		//�����ָ����ΪĬ��ֵ
		static Window* Create(const WindowProps& props = WindowProps());
	};
}