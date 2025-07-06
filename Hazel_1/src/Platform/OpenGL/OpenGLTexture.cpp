#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		stbi_set_flip_vertically_on_load(1);	//纹理上下翻转

		int width, height, channels;
		stbi_uc* data =			//unsigned char* 指向像素数据的指针
			stbi_load(			//加载图像的像素数据
				path.c_str(),	//图像文件路径
				&width,			//图像宽度						//输出
				&height,		//图像高度						//输出
				&channels,		//原始通道数（3 = RGB, 4 = RGBA）	//输出
				0);				//期望的通道数（0 = 保留原始, 1 = 灰度, 3 = RGB, 4 = RGBA)

		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		//输入的内部格式，输出的数据类型
		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

		glCreateTextures(	//创建纹理对象
			GL_TEXTURE_2D,	//纹理目标类型
			1,				//要创建的纹理数量
			&m_RendererID);	//输出数组，存储生成的纹理对象ID
		glTextureStorage2D(	//用于为2D纹理分配 不可变存储空间
			m_RendererID,	//纹理对象ID
			1,				//Mipmap 层级数（1表示无 Mipmap）
			internalFormat,	//纹理内部格式
			m_Width,		//纹理宽度
			m_Height);		//纹理高度

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(		//设置纹理对象参数
			m_RendererID,			//纹理对象ID
			GL_TEXTURE_MAG_FILTER,	//参数名称（MIN纹理缩小时的过度方式，MAG纹理放大时的过度方式， 等等）
			GL_NEAREST);			//参数值（NEAREST锯齿明显、性能高，LINEAR平滑、性能中等， 等等）

		glTextureSubImage2D(	//更新2D纹理部分或全部数据
			m_RendererID,		//纹理对象ID
			0,					//Mipmap 层级（0表示基础层级）
			0,					//X方向偏移量（像素）
			0,					//Y方向偏移量（像素）
			m_Width,			//要更新的区域宽度
			m_Height,			//要更新的区域高度
			dataFormat,				//数据类型
			GL_UNSIGNED_BYTE,	//数据格式
			data);				//指向像素数据的指针

		stbi_image_free(data);	//手动释放内存
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(	//删除纹理对象并释放相关资源
			1,				//删除的纹理数量
			&m_RendererID);	//纹理对象ID
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(	//将纹理绑定到指定纹理单元
			slot,			//纹理单元索引，即纹理槽
			m_RendererID);	//纹理对象ID
	}
}