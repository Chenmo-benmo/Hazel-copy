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
		
		//std::dynamic_pointer_cast<>() ������� std::shared_ptr ת��Ϊ������� std::shared_ptr����������ʱ������ͼ����ԣ�����ʧ����Ϊ���ؿ�ָ��
		//std::static_pointer_cast<>() �� std::dynamic_pointer_cast �����÷�һ��������û�м������ԣ����쵫�ǲ���ȫ������ʧ����Ϊ���ܱ���
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection", m_SceneData->ViewProjectionMatrix);	//����任����
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);								//����任����

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}