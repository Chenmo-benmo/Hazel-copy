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
		float m_AspectRatio;		//纵横比
		float m_ZoomLevel = 1.0f;	//缩放级别
		OrthographicCamera m_Camera;

		bool m_Rotation = false;	//是否启用旋转

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//相机变换矩阵	//相机位置
		float m_CameraRotation = 0.0f;						//相机变换角度	//相机角度
		float m_CameraTranslationSpeed = 1.0f;				//相机变换速度	//平移速度
		float m_CameraRotationSpeed = 180.0f;				//相机角度速度	//旋转速度
	};
}