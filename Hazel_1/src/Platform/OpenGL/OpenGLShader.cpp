#include "hzpch.h"
#include "OpenGLShader.h"

#include <fstream>
#include <glad/glad.h>

#include <glm/gtc/type_ptr.hpp>

namespace Hazel {

	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")	
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		HZ_CORE_ASSERT(false, "Unknown shader type!");
		return 0;
	}

	OpenGLShader::OpenGLShader(const std::string& filepath)
	{
		std::string source = ReadFile(filepath);
		auto shaderSources = PreProcess(source);
		Compile(shaderSources);

		//�ҵ� filepath ���ļ���
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');	//�����ض����ַ������ַ�
		auto count = lastDot == std::string::npos ? filepath.size() - lastSlash : lastDot - lastSlash;
		m_Name = filepath.substr(lastSlash, count);
	}

	OpenGLShader::OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc)
		: m_Name(name)
	{
		std::unordered_map<GLenum, std::string> sources;
		sources[GL_VERTEX_SHADER] = vertexSrc;
		sources[GL_FRAGMENT_SHADER] = fragmentSrc;
		Compile(sources);
	}

	OpenGLShader::~OpenGLShader()
	{
		glDeleteProgram(m_RendererID);
	}

	std::string OpenGLShader::ReadFile(const std::string& filepath)
	{ //��ȡ�ļ�
		std::string result;												//���ڴ洢�ļ�����
		std::ifstream in(filepath, std::ios::in, std::ios::binary);		//�Զ�����ģʽ std::ios::binary ���ļ��������ı�ת��
		if (in)															//����ļ��Ƿ�ɹ���
		{
			in.seekg(0, std::ios::end);			//���ļ�ָ���ƶ����ļ�ĩβ std::ios::end��ƫ����Ϊ 0��std::ifstream::seekg() �ƶ��ļ�ָ��

			result.resize(in.tellg());			//std::ifstream::tellg() ��ȡ��ǰ�ļ�ָ���λ�ã����涨 result �Ĵ�С
												//resize() ���������ַ������߼���С��reserve() ���������ַ������ڴ��С
												//�߼���С���ú���Ը����ַ�����С���仯���ڴ��С���ú󲻿ɱ仯
												//resize() �����������ַ�����reserve() ������Ԥ�����ڴ���������ݣ����߽��ʹ�ÿ�ʵ�ָ�Ч���ڴ����

			in.seekg(0, std::ios::beg);			//���ļ�ָ���ƶ����ļ���ͷ std::ios::beg��ƫ����Ϊ 0

			in.read(&result[0], result.size());	//&result[0] ��ȡ�ײ��ַ�������ָ�룬���涨��ȡ���ֽ��������������ݶ�ȡ�� result ��
			in.close();							//�ر��ļ�����Ȼ���Զ��رգ����ֶ��ر��Ǹ���ϰ��
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{ //���ඥ����ɫ����Ƭ����ɫ��
		std::unordered_map<GLenum, std::string> shaderSources;	//�洢��ֵ�� GLenum-std::string��ͨ�� GLenum ���ٲ��� std::string

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);	//size_t ȡ����ϵͳ���ܣ�32λϵͳ��unsigned int��64λϵͳ��unsigned long �� unsigned long long
													//strlen() �����ַ�������
		size_t pos = source.find(typeToken, 0);		//std::string::find() ��λ�� 0 ��ʼ���� source �в��� typeToken ���״γ��ֵ�λ��
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);	//std::string::find_first_of() ��λ�� pos ��ʼ���� source �в����״γ������� "\r\n" ���ϵ� �ַ���λ��
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);	//std::string::substr() ��λ�� begin ��ʼ����ȡ����Ϊ eol - begin ���ַ���
			HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);	//std::string::find_first_not_of() ��λ�� eol ��ʼ���� source �в����״γ��ֲ����� "\r\n" ���ϵ� �ַ���λ��
			pos = source.find(typeToken, nextLinePos);

			//����ܲ��ҵ���һ�� "#type" ���������� shaderSources[type]�����û���ҵ���һ������������������ shaderSources[type]
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			//pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)�õ�����source.size
			//Ӧ�õõ�source.size - nextLinePos�Ų��ᳬ����Χ
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{ //��ɫ��������
		GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			//����������ɫ������
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{ //�������Ƿ�ɹ�
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data()); //std::vector::data ����һ��ָ��ײ������ָ��
				HZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			//��������ɫ�������Ƭ����ɫ���������ӵ���ɫ��������
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{ //�������Ƿ�ɹ�
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			// We don't need the program anymore.
			glDeleteProgram(program);
			
			for (auto id : glShaderIDs)
				glDeleteShader(id);

			HZ_CORE_ERROR("{0}", infoLog.data());
			HZ_CORE_ASSERT(false, "Shader link failure!");
			return;
		}

		//����ɫ���������ɫ�������з������������Ӧ����Ҫ glDeleteShader ɾ��ʹ�ú����ɫ������ģ�������û�У�Ӧ���ǿ��Ǻ����п��ܻ��ٴ�ʹ����ɫ������
		for (auto id : glShaderIDs)
			glDetachShader(program, id);

		m_RendererID = program;
	}

	void OpenGLShader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void OpenGLShader::Unbind() const
	{
		glUseProgram(0);
	}

	void OpenGLShader::SetFloat3(const std::string& name, const glm::vec3& value)
	{
		UploadUniformFloat3(name, value);
	}

	void OpenGLShader::SetFloat4(const std::string& name, const glm::vec4& value)
	{
		UploadUniformFloat4(name, value);
	}

	void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& value)
	{
		UploadUniformMat4(name, value);
	}

	void OpenGLShader::UploadUniformInt(const std::string& name, int values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, values);
	}

	void OpenGLShader::UploadUniformFloat(const std::string& name, float values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, values);
	}

	void OpenGLShader::UploadUniformFloat2(const std::string& name, const glm::vec2& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, values.x, values.y);
	}

	void OpenGLShader::UploadUniformFloat3(const std::string& name, const glm::vec3& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, values.x, values.y, values.z);
	}

	void OpenGLShader::UploadUniformFloat4(const std::string& name, const glm::vec4& values)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform4f(location, values.x, values.y, values.z, values.w);
	}

	void OpenGLShader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void OpenGLShader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		//ʹ�� glGetUniformLocation �ɻ�ȡ��ɫ���� name ����λ�ã�����-1����û�ҵ�
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix4fv(				//��CPU�ڴ��е�4 * 4������󴫵ݵ�GPU����ɫ����
			location,					//��ɫ���е� uniform ����λ��
			1,							//����ľ�������
			GL_FALSE,					//�Ƿ���Ҫת�ã��� OrthographicCamera �ļ����Ѿ���������ת������ת�ó���ͼ����	���˴��� GL_FALSE
			glm::value_ptr(matrix));	//ָ����ͼ�������ݵ�ָ�� glm::value_ptr ��glm����ת��Ϊԭʼָ��
	}
}