#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(
			glm::ortho(	//����mat4����ͶӰ����
				left,	//��ƽ������
				right,	//��ƽ������
				bottom, //��ƽ������
				top,	//��ƽ������
				-1.0f,	//���ü�����루ͨ��Ϊ������ʾ�����ǰ����
				1.0f)),	//Զ�ü������
		  m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		m_ProjectionMatrix = glm::ortho(left, right, bottom, top);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		//��������任����
		//����˷��Ǵ�������Ӧ�õģ����ʵ��˳��Ϊ ����ת��ƽ��
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *				//������λ���󣬲�������ƶ��� m_Position ������
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));	//��Z����ת m_Rotation ����

		//������ͼ���� 
		m_ViewMatrix = glm::inverse(transform);	
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}