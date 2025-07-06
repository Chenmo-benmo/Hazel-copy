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

		//找到 filepath 的文件名
		auto lastSlash = filepath.find_last_of("/\\");
		lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
		auto lastDot = filepath.rfind('.');	//查找特定子字符串或字符
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
	{ //读取文件
		std::string result;												//用于存储文件内容
		std::ifstream in(filepath, std::ios::in, std::ios::binary);		//以二进制模式 std::ios::binary 打开文件，避免文本转换
		if (in)															//检查文件是否成功打开
		{
			in.seekg(0, std::ios::end);			//将文件指针移动到文件末尾 std::ios::end，偏移量为 0；std::ifstream::seekg() 移动文件指针

			result.resize(in.tellg());			//std::ifstream::tellg() 获取当前文件指针的位置，即规定 result 的大小
												//resize() 用于设置字符串的逻辑大小，reserve() 用于设置字符串的内存大小
												//逻辑大小设置后可以根据字符串大小而变化，内存大小设置后不可变化
												//resize() 常用于设置字符串，reserve() 常用于预分配内存避免多次扩容，两者结合使用可实现高效的内存管理

			in.seekg(0, std::ios::beg);			//将文件指针移动到文件开头 std::ios::beg，偏移量为 0

			in.read(&result[0], result.size());	//&result[0] 获取底层字符串数组指针，并规定读取的字节数，将整个内容读取到 result 中
			in.close();							//关闭文件，虽然会自动关闭，但手动关闭是个好习惯
		}
		else
		{
			HZ_CORE_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGLShader::PreProcess(const std::string& source)
	{ //分类顶点着色器、片段着色器
		std::unordered_map<GLenum, std::string> shaderSources;	//存储键值对 GLenum-std::string，通过 GLenum 快速查找 std::string

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);	//size_t 取决与系统构架，32位系统：unsigned int，64位系统：unsigned long 或 unsigned long long
													//strlen() 计算字符串长度
		size_t pos = source.find(typeToken, 0);		//std::string::find() 从位置 0 开始，在 source 中查找 typeToken 的首次出现的位置
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);	//std::string::find_first_of() 从位置 pos 开始，在 source 中查找首次出现属于 "\r\n" 集合的 字符的位置
			HZ_CORE_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);	//std::string::substr() 从位置 begin 开始，提取长度为 eol - begin 的字符串
			HZ_CORE_ASSERT(type == "vertex" || type == "fragment" || type == "pixel", "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);	//std::string::find_first_not_of() 从位置 eol 开始，在 source 中查找首次出现不属于 "\r\n" 集合的 字符的位置
			pos = source.find(typeToken, nextLinePos);

			//如果能查找到下一个 "#type" 将该行载入 shaderSources[type]；如果没能找到下一个，将后面所有载入 shaderSources[type]
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
			//pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos)得到的是source.size
			//应该得到source.size - nextLinePos才不会超出范围
		}

		return shaderSources;
	}

	void OpenGLShader::Compile(const std::unordered_map<GLenum, std::string>& shaderSources)
	{ //着色器上下文
		GLuint program = glCreateProgram();
		HZ_CORE_ASSERT(shaderSources.size() <= 2, "We only support 2 shaders for now");
		std::array<GLenum, 2> glShaderIDs;
		int glShaderIDIndex = 0;
		for (auto& kv : shaderSources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;

			GLuint shader = glCreateShader(type);

			//创建顶点着色器对象
			const GLchar* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{ //检查编译是否成功
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				glDeleteShader(shader);

				HZ_CORE_ERROR("{0}", infoLog.data()); //std::vector::data 返回一个指向底层数组的指针
				HZ_CORE_ASSERT(false, "Shader compilation failure!");
				break;
			}

			//将顶点着色器对象和片段着色器对象链接到着色器程序上
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}


		// Link our program
		glLinkProgram(program);

		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{ //检查编译是否成功
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

		//将着色器对象从着色器程序中分离出来，这里应该需要 glDeleteShader 删除使用后的着色器对象的，但这里没有，应该是考虑后面有可能会再次使用着色器对象
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
		//使用 glGetUniformLocation 可获取着色器中 name 所在位置，返回-1代表没找到
		GLint location = glGetUniformLocation(m_RendererID, name.c_str());

		glUniformMatrix4fv(				//将CPU内存中的4 * 4浮点矩阵传递到GPU的着色器中
			location,					//着色器中的 uniform 变量位置
			1,							//传输的矩阵数量
			GL_FALSE,					//是否需要转置，在 OrthographicCamera 文件中已经添加了相机转换矩阵转置成视图矩阵	，此处是 GL_FALSE
			glm::value_ptr(matrix));	//指向视图矩阵数据的指针 glm::value_ptr 将glm矩阵转换为原始指针
	}
}