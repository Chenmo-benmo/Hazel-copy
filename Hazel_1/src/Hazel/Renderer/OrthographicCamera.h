#pragma once

#include <glm/glm.hpp>

namespace Hazel {

	class OrthographicCamera
	{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjection(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
		//第一个 const 表示返回常量引用，防止外部通过返回值修改内部状态
		//第二个 const 声明常量成员函数，保证函数不修改对象状态 且支持 const 对象调用
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;					//投影矩阵
		glm::mat4 m_ViewMatrix;							//视图矩阵
		glm::mat4 m_ViewProjectionMatrix;				//VP矩阵

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };	//局部坐标
		float m_Rotation = 0.0f;						//旋转角度
	};
}