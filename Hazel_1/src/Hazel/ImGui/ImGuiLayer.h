#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {

	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		//P22  ɾ��//	void OnAttach() override;
		//P22  ɾ��//	void OnDetach() override;
		//P22  ɾ��//	void OnUpdate() override;
		//P22  ɾ��//	void OnEvent(Event& event) override;
		//P22  ɾ��//
		//P22  ɾ��//private:
		//P22  ɾ��//	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		//P22  ɾ��//	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		//P22  ɾ��//	bool OnMouseMovedEvent(MouseMovedEvent& e);
		//P22  ɾ��//	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		//P22  ɾ��//	bool OnKeyPressedEvent(KeyPressedEvent& e);
		//P22  ɾ��//	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//P22  ɾ��//	bool OnKeyTypedEvent(KeyTypedEvent& e);
		//P22  ɾ��//	bool OnWindowResizeEvent(WindowResizeEvent& e);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}