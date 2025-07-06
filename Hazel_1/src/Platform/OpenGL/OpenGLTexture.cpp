#include "hzpch.h"
#include "OpenGLTexture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Hazel {

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path)
		: m_Path(path)
	{
		stbi_set_flip_vertically_on_load(1);	//�������·�ת

		int width, height, channels;
		stbi_uc* data =			//unsigned char* ָ���������ݵ�ָ��
			stbi_load(			//����ͼ�����������
				path.c_str(),	//ͼ���ļ�·��
				&width,			//ͼ����						//���
				&height,		//ͼ��߶�						//���
				&channels,		//ԭʼͨ������3 = RGB, 4 = RGBA��	//���
				0);				//������ͨ������0 = ����ԭʼ, 1 = �Ҷ�, 3 = RGB, 4 = RGBA)

		HZ_CORE_ASSERT(data, "Failed to load image!");
		m_Width = width;
		m_Height = height;

		//������ڲ���ʽ���������������
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

		glCreateTextures(	//�����������
			GL_TEXTURE_2D,	//����Ŀ������
			1,				//Ҫ��������������
			&m_RendererID);	//������飬�洢���ɵ��������ID
		glTextureStorage2D(	//����Ϊ2D������� ���ɱ�洢�ռ�
			m_RendererID,	//�������ID
			1,				//Mipmap �㼶����1��ʾ�� Mipmap��
			internalFormat,	//�����ڲ���ʽ
			m_Width,		//������
			m_Height);		//����߶�

		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(		//��������������
			m_RendererID,			//�������ID
			GL_TEXTURE_MAG_FILTER,	//�������ƣ�MIN������Сʱ�Ĺ��ȷ�ʽ��MAG����Ŵ�ʱ�Ĺ��ȷ�ʽ�� �ȵȣ�
			GL_NEAREST);			//����ֵ��NEAREST������ԡ����ܸߣ�LINEARƽ���������еȣ� �ȵȣ�

		glTextureSubImage2D(	//����2D�����ֻ�ȫ������
			m_RendererID,		//�������ID
			0,					//Mipmap �㼶��0��ʾ�����㼶��
			0,					//X����ƫ���������أ�
			0,					//Y����ƫ���������أ�
			m_Width,			//Ҫ���µ�������
			m_Height,			//Ҫ���µ�����߶�
			dataFormat,				//��������
			GL_UNSIGNED_BYTE,	//���ݸ�ʽ
			data);				//ָ���������ݵ�ָ��

		stbi_image_free(data);	//�ֶ��ͷ��ڴ�
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		glDeleteTextures(	//ɾ����������ͷ������Դ
			1,				//ɾ������������
			&m_RendererID);	//�������ID
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(	//������󶨵�ָ������Ԫ
			slot,			//����Ԫ�������������
			m_RendererID);	//�������ID
	}
}