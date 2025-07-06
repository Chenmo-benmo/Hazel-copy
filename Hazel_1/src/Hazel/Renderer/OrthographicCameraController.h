#pragma once

#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {

	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		float m_AspectRatio;		//�ݺ��
		float m_ZoomLevel = 1.0f;	//���ż���
		OrthographicCamera m_Camera;

		bool m_Rotation = false;	//�Ƿ�������ת

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//����任����	//���λ��
		float m_CameraRotation = 0.0f;						//����任�Ƕ�	//����Ƕ�
		float m_CameraTranslationSpeed = 1.0f;				//����任�ٶ�	//ƽ���ٶ�
		float m_CameraRotationSpeed = 180.0f;				//����Ƕ��ٶ�	//��ת�ٶ�
	};
}