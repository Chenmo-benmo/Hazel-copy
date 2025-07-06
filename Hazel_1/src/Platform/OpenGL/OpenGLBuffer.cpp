#include "hzpch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel {

	///////////////////////////////////////////////////////////////////////////////////////
	// VertexBuffer ///////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);								//生成一个缓冲区对象，并将缓冲区对象ID传递给m_RendererID
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);					//将缓冲区对象绑定至上下文，GL_ARRAY_BUFFER表示要绑定的缓冲区类型
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);	//将数据复制到缓冲区对象中，GL_STATIC_DRAW表示数据很少修改和多次使用，GL_DYNAMIC_DRAW表示内容会被反复修改和多次使用
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	///////////////////////////////////////////////////////////////////////////////////////
	// IndexBuffer ////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////

	OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
		: m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}