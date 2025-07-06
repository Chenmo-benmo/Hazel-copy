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

		//P22  É¾³ý//	void OnAttach() override;
		//P22  É¾³ý//	void OnDetach() override;
		//P22  É¾³ý//	void OnUpdate() override;
		//P22  É¾³ý//	void OnEvent(Event& event) override;
		//P22  É¾³ý//
		//P22  É¾³ý//private:
		//P22  É¾³ý//	bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		//P22  É¾³ý//	bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		//P22  É¾³ý//	bool OnMouseMovedEvent(MouseMovedEvent& e);
		//P22  É¾³ý//	bool OnMouseScrolledEvent(MouseScrolledEvent& e);
		//P22  É¾³ý//	bool OnKeyPressedEvent(KeyPressedEvent& e);
		//P22  É¾³ý//	bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		//P22  É¾³ý//	bool OnKeyTypedEvent(KeyTypedEvent& e);
		//P22  É¾³ý//	bool OnWindowResizeEvent(WindowResizeEvent& e);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};

}