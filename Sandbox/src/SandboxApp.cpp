#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Sandbox2D.h"

//glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
//{
//	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
//	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//	return Projection * View * Model;
//}

class ExampleLayer : public Hazel::Layer
{ //创建派生类可直接当成基类使用，如可在接收 Layer* 的函数时传递 ExampleLayer* 
public:
	ExampleLayer()
		: Layer("Example") /*创建 Layer*/, 
		//P47	删除//m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
		m_CameraController(1280.0f / 720.0f, true)
	{	
		//auto cam = camera(5.0f, { 0.5f, 0.5f });
		

	//P36---以下所有渲染代码全部由 Application 移动过来--------------------------------
	//------------------------------------------------------------------------------


		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		//std::unique_ptr::reset 接管实例，这里 m_VertexBuffer 接管了 VertexBuffer 静态方法创建的实例，() 为空则释放管理对象

		Hazel::BufferLayout layout = {
			//initializer_list 可以传递给 BufferElement 所需的构造元素而不需要像 vector 单独构建本身
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float4, "a_Color" }
		};

		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Hazel::Ref<Hazel::IndexBuffer> indexBuffer;
		indexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA = Hazel::VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> squareVB;
		squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Hazel::ShaderDataType::Float3, "a_Position" },
			{ Hazel::ShaderDataType::Float2, "a_TexCoord" }
		});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[2 * 3] = {
			0, 1, 2,
			2, 3, 0
		};
		Hazel::Ref<Hazel::IndexBuffer> squareIB;
		squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;					//location = 0 和 glEnableVertexAttribArray(0) 对应 //输入一个 vec3 变量，这里是点的位置
			layout(location = 1) in vec4 a_Color;
		
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;										//输出点的位置 vec3
			out vec4 v_Color;
		
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				//告诉 gl_Position 这个点的位置，第四个数据表示窗口半边长为 1.0	//VP矩阵*模型矩阵得到相机中的图像
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);				
			}
		)";


		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;		//输出一个 vec4 变量，这里是像素的rgba
		
			in vec3 v_Position;						//和顶点着色器对应，变量名和类型必须一模一样
			in vec4 v_Color;
		
			void main()
			{
				color = vec4((v_Position + 1.0) / 2, 1.0);			//赋予 color 颜色
				color = v_Color;
			}
		)";

		m_Shader = Hazel::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;	
		
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
		
			out vec3 v_Position;						
		
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;		
		
			in vec3 v_Position;							
		
			uniform vec3 u_Color;			

			void main()
			{
				color = vec4(u_Color, 1.0f);
			}
		)";

		m_FlatColorShader = Hazel::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);

		//迁移到 assets/shaders/Texture.glsl 文件中
		//P44	删除//std::string textureShaderVertexSrc = R"(
		//P44	删除//	#version 330 core
		//P44	删除//	
		//P44	删除//	layout(location = 0) in vec3 a_Position;	
		//P44	删除//	layout(location = 1) in vec2 a_TexCoord;
		//P44	删除//
		//P44	删除//	uniform mat4 u_ViewProjection;
		//P44	删除//	uniform mat4 u_Transform;
		//P44	删除//
		//P44	删除//	out vec2 v_TexCoord;						
		//P44	删除//
		//P44	删除//	void main()
		//P44	删除//	{
		//P44	删除//		v_TexCoord = a_TexCoord;
		//P44	删除//		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		//P44	删除//	}
		//P44	删除//)";
		//P44	删除//std::string textureShaderFragmentSrc = R"(
		//P44	删除//	#version 330 core
		//P44	删除//	
		//P44	删除//	layout(location = 0) out vec4 color;		
		//P44	删除//
		//P44	删除//	in vec2 v_TexCoord;					
		//P44	删除//
		//P44	删除//	uniform sampler2D u_Texture;	
		//P44	删除
		//P44	删除//	void main()
		//P44	删除//	{
		//P44	删除//		color = texture(u_Texture, v_TexCoord);
		//P44	删除//	}
		//P44	删除//)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/chengshan.gif");
		m_ChernoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		//HZ_INFO("ExampleLayer::Update");

		//P36	删除//if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
		//P36	删除//	HZ_ERROR("Tab key is pressed (poll)!");

		m_CameraController.OnUpdate(ts);
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//抽象至 OrthographicCameraController
		//用方向键和AD键控制相机，用 每帧的时间 * 速度 即可获得固定的速度
		//P47	删除//if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		//P47	删除//	m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		//P47	删除//else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		//P47	删除//	m_CameraPosition.x += m_CameraMoveSpeed * ts;
		//P47	删除//if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		//P47	删除//	m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		//P47	删除//else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		//P47	删除//	m_CameraPosition.y += m_CameraMoveSpeed * ts;
		//P47	删除//
		//P47	删除//if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		//P47	删除//	m_CameraRotation += m_CameraRotationSpeed * ts;
		//P47	删除//if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		//P47	删除//	m_CameraRotation -= m_CameraRotationSpeed * ts;



		Hazel::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Hazel::RenderCommand::Clear();

		//P47	删除//m_Camera.SetPosition(m_CameraPosition);
		//P47	删除//m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));	//scale 物体缩放0.1倍

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				//创建单位矩阵，并将物体移动到 pos 坐标上，并缩放 scale 倍
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;

				Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}

		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoTexture->Bind();
		Hazel::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Triangle
		//Hazel::Renderer::Submit(m_Shader, m_VertexArray);

		Hazel::Renderer::EndScene();
	}
private://opengl
	Hazel::ShaderLibrary m_ShaderLibrary;

	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;

	Hazel::Ref<Hazel::Shader> m_FlatColorShader;
	Hazel::Ref<Hazel::VertexArray> m_SquareVA;

	Hazel::Ref<Hazel::Texture2D> m_Texture, m_ChernoTexture;



	//P36---以上所有渲染代码全部由 Application 移动过来--------------------------------
	//------------------------------------------------------------------------------



public:
	virtual void OnImGuiRender() override
	{
		//P36	删除//ImGui::Begin("Text");
		//P36	删除//ImGui::Text("Hello World!");
		//P36	删除//ImGui::End();

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//P36	删除//if (event.GetEventType() == Hazel::EventType::KeyPressed)
		//P36	删除//{//展示键盘按下的按键
		//P36	删除//	Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
		//P36	删除//	if (e.GetKeyCode() == HZ_KEY_TAB)
		//P36	删除//		HZ_TRACE("Tab key is pressed! (event)");
		//P36	删除//	HZ_ERROR("{0}",(char)e.GetKeyCode());
		//P36	删除//}

		m_CameraController.OnEvent(event);

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressecEvent));
	}

	bool OnKeyPressecEvent(Hazel::KeyPressedEvent& event)
	{

		return false;
	}
private://camera
	Hazel::OrthographicCameraController m_CameraController;		//创建一个相机
	//P47	删除//glm::vec3 m_CameraPosition;			//相机变换矩阵
	//P47	删除//float m_CameraMoveSpeed = 5.0f;		//相机变换速度
	//P47	删除//float m_CameraRotation = 0.0f;			//相机变换角度
	//P47	删除//float m_CameraRotationSpeed = 180.0f;	//相机角度速度

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

class Sandbox : public Hazel::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~Sandbox()
	{

	}

};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}