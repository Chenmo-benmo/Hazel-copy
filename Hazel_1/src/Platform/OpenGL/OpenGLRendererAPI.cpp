#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Hazel {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);									//���û�Ϲ���
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//���û������
		//R = (Rsrc * 0) + (Rdest * (1 - 0)) = Rdest
		//G = (Gsrc * 0) + (Gdest * (1 - 0)) = Gdest
		//B = (Bsrc * 0) + (Bdest * (1 - 0)) = Bdest
		//A = (Asrc * 0) + (Adest * (1 - 0)) = Adest
		//Result = Source.RGB * Source.Alpha + Destination.RGB * (1 - Source.Alpha)
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(		//���ڶ����ӿڣ�������Ⱦ�ӿڴ�С�봰�ڴ�Сһ��
			x,			//�ӿ����½ǵ�x����
			y,			//�ӿ����½ǵ�y����
			width,		//�ӿڿ�ȣ����أ�
			height);	//�ӿڸ߶ȣ����أ�
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		//�����ɫ�����ñ�����ɫ
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		//��� ��ɫ������ �� ��Ȼ�����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES,					//����ģʽ
			vertexArray->GetIndexBuffer()->GetCount(),	//���Ƶ���������
			GL_UNSIGNED_INT,							//������������
			nullptr);									//����������ʼ��ַ
	}
}