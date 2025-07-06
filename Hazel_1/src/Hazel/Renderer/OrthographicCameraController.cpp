#include "hzpch.h"
#include "OrthographicCameraController.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCodes.h"

namespace Hazel {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		: m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		//�÷������AD������������� ÿ֡��ʱ�� * �ٶ� ���ɻ�ù̶����ٶ�
		if (Input::IsKeyPressed(HZ_KEY_A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_D))
			m_CameraPosition.x += m_CameraTranslationSpeed * ts;
		if (Input::IsKeyPressed(HZ_KEY_S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(HZ_KEY_W))
			m_CameraPosition.y += m_CameraTranslationSpeed * ts;

		if (m_Rotation)
		{ 
			if (Input::IsKeyPressed(HZ_KEY_Q))
				m_CameraRotation += m_CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(HZ_KEY_E))
				m_CameraRotation -= m_CameraRotationSpeed * ts;

			m_Camera.SetRotation(m_CameraRotation);
		}

		m_Camera.SetPosition(m_CameraPosition);

		//��Ϊ����ٶ�����ͼ�ٶ����������Ӿ�Ч��������ͨ���ı���ͼ�ٶȿ��Դﵽ����ٶȲ����Ŀ��
		m_CameraTranslationSpeed = 1.0f * m_ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event& e)
	{ //�ύ�����ֺʹ��ڴ�С����
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		//ͨ���������ı����ż��������ż���ĸı� ���ı�����ӽǱ߽�Ĵ�С���Դﵽ��ͼ��С��������������Ӿ�Ч��
		m_ZoomLevel -= e.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
	{
		//������ӽǱ߽���ݺ�ȸı�Ϊ�����ݺ�ȣ��������ӽ��ݺ�Ⱥʹ����ݺ�Ȳ���������� ��Ⱦѹ���������������
		//��Ϊ�ı�����ݺ�ȣ����������ʱ��Ⱦ����ı䣬�������ʱ��Ⱦ���ɻ�ȱ�������
		//�������ʱ����Ϊ������Ⱦȫ�����������򰴱�����С
		//����������ʱ���� ����ӽ������С�ʹ��������Сһ�£��Ϳ���ʵ����������������ı���Ⱦ��С
		//m_ZoomLevel = (float)e.GetHeight() / 720.0f;	
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		return false;
	}
}