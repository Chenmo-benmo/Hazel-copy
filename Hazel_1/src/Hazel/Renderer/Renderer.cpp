#include "hzpch.h"
#include "Renderer.h"

#include "Platform/OpenGL/OpenGLShader.h"
#include "Renderer2D.h"

namespace Hazel {

	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData;

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer2D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}
	
	void Renderer::EndScene()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		
		//std::dynamic_pointer_cast<>() 将基类的 std::shared_ptr 转换为派生类的 std::shared_ptr，并在运行时检查类型兼容性，遇到失败行为返回空指针
		//std::static_pointer_cast<>() 和 std::dynamic_pointer_cast 作用用法一样，但是没有检查兼容性，更快但是不安全，遇到失败行为可能崩溃
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);	//相机变换矩阵
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);								//物体变换矩阵

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}