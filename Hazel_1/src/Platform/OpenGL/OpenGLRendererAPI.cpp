#include "hzpch.h"
#include "OpenGLRendererAPI.h"

#include "glad/glad.h"

namespace Hazel {

	void OpenGLRendererAPI::Init()
	{
		glEnable(GL_BLEND);									//启用混合功能
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//设置混合因子
		//R = (Rsrc * 0) + (Rdest * (1 - 0)) = Rdest
		//G = (Gsrc * 0) + (Gdest * (1 - 0)) = Gdest
		//B = (Bsrc * 0) + (Bdest * (1 - 0)) = Bdest
		//A = (Asrc * 0) + (Adest * (1 - 0)) = Adest
		//Result = Source.RGB * Source.Alpha + Destination.RGB * (1 - Source.Alpha)
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(		//用于定义视口，保持渲染视口大小与窗口大小一致
			x,			//视口左下角的x坐标
			y,			//视口左下角的y坐标
			width,		//视口宽度（像素）
			height);	//视口高度（像素）
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color)
	{
		//清除颜色并设置背景颜色
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear()
	{
		//清除 颜色缓冲区 和 深度缓冲区
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES,					//绘制模式
			vertexArray->GetIndexBuffer()->GetCount(),	//绘制的索引数量
			GL_UNSIGNED_INT,							//索引数据类型
			nullptr);									//索引数据起始地址
	}
}