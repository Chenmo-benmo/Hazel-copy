#include "hzpch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel {

	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		: m_ProjectionMatrix(
			glm::ortho(	//返回mat4正交投影矩阵
				left,	//左平面坐标
				right,	//右平面坐标
				bottom, //下平面坐标
				top,	//上平面坐标
				-1.0f,	//近裁剪面距离（通常为负，表示相机正前方）
				1.0f)),	//远裁剪面距离
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
		//定义相机变换矩阵
		//矩阵乘法是从右向左应用的，因此实际顺序为 先旋转再平移
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) *				//创建单位矩阵，并将相机移动到 m_Position 坐标上
			glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));	//绕Z轴旋转 m_Rotation 弧度

		//计算视图矩阵 
		m_ViewMatrix = glm::inverse(transform);	
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}