#pragma once

#include "Core.h"

#include "Window.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/Core/Timestep.h"

#include "Hazel/ImGui/ImGuiLayer.h"


namespace Hazel {

	class  Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() { return *m_Window; } //���ش��ھ��

		inline static Application& Get() { return *s_Instance; } //���ظ� Application ʵ�� //�൱�� return this
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		std::unique_ptr<ImGuiLayer> m_ImGuiLayer;
		bool m_Running = true;
		bool m_Minimized = false;
		LayerStack m_LayerStack;
		Timestep m_Timestep;
		float m_LastFrameTime = 0.0f;

		/*unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer*/;
		//P36	ɾ��//std::shared_ptr<Shader> m_Shader;
		//P36	ɾ��//std::shared_ptr<VertexArray> m_VertexArray;
		//P32	ɾ��//std::shared_ptr<VertexBuffer> m_VertexBuffer;
		//P32	ɾ��//std::shared_ptr<IndexBuffer> m_IndexBuffer;

		//P36	ɾ��//std::shared_ptr<Shader> m_BlueShader;
		//P36	ɾ��//std::shared_ptr<VertexArray> m_SquareVA;

		//P36	ɾ��//OrthographicCamera m_Camera;
	private:
		//static �ڸýṹ���ⶨ����Ҫ�ٴ���������
		//�Ǿ�̬���ݳ�Ա�������ⲿ����
		static Application* s_Instance;
	};

	//To be define in CLIENT
	Application* CreateApplication();

}