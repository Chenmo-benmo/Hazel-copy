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
{ //�����������ֱ�ӵ��ɻ���ʹ�ã�����ڽ��� Layer* �ĺ���ʱ���� ExampleLayer* 
public:
	ExampleLayer()
		: Layer("Example") /*���� Layer*/, 
		//P47	ɾ��//m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
		m_CameraController(1280.0f / 720.0f, true)
	{	
		//auto cam = camera(5.0f, { 0.5f, 0.5f });
		

	//P36---����������Ⱦ����ȫ���� Application �ƶ�����--------------------------------
	//------------------------------------------------------------------------------


		m_VertexArray = Hazel::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};
		Hazel::Ref<Hazel::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
		//std::unique_ptr::reset �ӹ�ʵ�������� m_VertexBuffer �ӹ��� VertexBuffer ��̬����������ʵ����() Ϊ�����ͷŹ������

		Hazel::BufferLayout layout = {
			//initializer_list ���Դ��ݸ� BufferElement ����Ĺ���Ԫ�ض�����Ҫ�� vector ������������
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
			
			layout(location = 0) in vec3 a_Position;					//location = 0 �� glEnableVertexAttribArray(0) ��Ӧ //����һ�� vec3 �����������ǵ��λ��
			layout(location = 1) in vec4 a_Color;
		
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;										//������λ�� vec3
			out vec4 v_Color;
		
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;

				//���� gl_Position ������λ�ã����ĸ����ݱ�ʾ���ڰ�߳�Ϊ 1.0	//VP����*ģ�;���õ�����е�ͼ��
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);				
			}
		)";


		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;		//���һ�� vec4 ���������������ص�rgba
		
			in vec3 v_Position;						//�Ͷ�����ɫ����Ӧ�������������ͱ���һģһ��
			in vec4 v_Color;
		
			void main()
			{
				color = vec4((v_Position + 1.0) / 2, 1.0);			//���� color ��ɫ
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

		//Ǩ�Ƶ� assets/shaders/Texture.glsl �ļ���
		//P44	ɾ��//std::string textureShaderVertexSrc = R"(
		//P44	ɾ��//	#version 330 core
		//P44	ɾ��//	
		//P44	ɾ��//	layout(location = 0) in vec3 a_Position;	
		//P44	ɾ��//	layout(location = 1) in vec2 a_TexCoord;
		//P44	ɾ��//
		//P44	ɾ��//	uniform mat4 u_ViewProjection;
		//P44	ɾ��//	uniform mat4 u_Transform;
		//P44	ɾ��//
		//P44	ɾ��//	out vec2 v_TexCoord;						
		//P44	ɾ��//
		//P44	ɾ��//	void main()
		//P44	ɾ��//	{
		//P44	ɾ��//		v_TexCoord = a_TexCoord;
		//P44	ɾ��//		gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
		//P44	ɾ��//	}
		//P44	ɾ��//)";
		//P44	ɾ��//std::string textureShaderFragmentSrc = R"(
		//P44	ɾ��//	#version 330 core
		//P44	ɾ��//	
		//P44	ɾ��//	layout(location = 0) out vec4 color;		
		//P44	ɾ��//
		//P44	ɾ��//	in vec2 v_TexCoord;					
		//P44	ɾ��//
		//P44	ɾ��//	uniform sampler2D u_Texture;	
		//P44	ɾ��
		//P44	ɾ��//	void main()
		//P44	ɾ��//	{
		//P44	ɾ��//		color = texture(u_Texture, v_TexCoord);
		//P44	ɾ��//	}
		//P44	ɾ��//)";

		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = Hazel::Texture2D::Create("assets/textures/chengshan.gif");
		m_ChernoTexture = Hazel::Texture2D::Create("assets/textures/ChernoLogo.png");

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Hazel::Timestep ts) override
	{
		//HZ_INFO("ExampleLayer::Update");

		//P36	ɾ��//if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
		//P36	ɾ��//	HZ_ERROR("Tab key is pressed (poll)!");

		m_CameraController.OnUpdate(ts);
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//������ OrthographicCameraController
		//�÷������AD������������� ÿ֡��ʱ�� * �ٶ� ���ɻ�ù̶����ٶ�
		//P47	ɾ��//if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
		//P47	ɾ��//	m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		//P47	ɾ��//else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
		//P47	ɾ��//	m_CameraPosition.x += m_CameraMoveSpeed * ts;
		//P47	ɾ��//if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
		//P47	ɾ��//	m_CameraPosition.y -= m_CameraMoveSpeed * ts;
		//P47	ɾ��//else if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
		//P47	ɾ��//	m_CameraPosition.y += m_CameraMoveSpeed * ts;
		//P47	ɾ��//
		//P47	ɾ��//if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
		//P47	ɾ��//	m_CameraRotation += m_CameraRotationSpeed * ts;
		//P47	ɾ��//if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
		//P47	ɾ��//	m_CameraRotation -= m_CameraRotationSpeed * ts;



		Hazel::RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 });
		Hazel::RenderCommand::Clear();

		//P47	ɾ��//m_Camera.SetPosition(m_CameraPosition);
		//P47	ɾ��//m_Camera.SetRotation(m_CameraRotation);

		Hazel::Renderer::BeginScene(m_CameraController.GetCamera());

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));	//scale ��������0.1��

		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				//������λ���󣬲��������ƶ��� pos �����ϣ������� scale ��
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



	//P36---����������Ⱦ����ȫ���� Application �ƶ�����--------------------------------
	//------------------------------------------------------------------------------



public:
	virtual void OnImGuiRender() override
	{
		//P36	ɾ��//ImGui::Begin("Text");
		//P36	ɾ��//ImGui::Text("Hello World!");
		//P36	ɾ��//ImGui::End();

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Hazel::Event& event) override
	{
		//P36	ɾ��//if (event.GetEventType() == Hazel::EventType::KeyPressed)
		//P36	ɾ��//{//չʾ���̰��µİ���
		//P36	ɾ��//	Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
		//P36	ɾ��//	if (e.GetKeyCode() == HZ_KEY_TAB)
		//P36	ɾ��//		HZ_TRACE("Tab key is pressed! (event)");
		//P36	ɾ��//	HZ_ERROR("{0}",(char)e.GetKeyCode());
		//P36	ɾ��//}

		m_CameraController.OnEvent(event);

		Hazel::EventDispatcher dispatcher(event);
		dispatcher.Dispatch<Hazel::KeyPressedEvent>(HZ_BIND_EVENT_FN(ExampleLayer::OnKeyPressecEvent));
	}

	bool OnKeyPressecEvent(Hazel::KeyPressedEvent& event)
	{

		return false;
	}
private://camera
	Hazel::OrthographicCameraController m_CameraController;		//����һ�����
	//P47	ɾ��//glm::vec3 m_CameraPosition;			//����任����
	//P47	ɾ��//float m_CameraMoveSpeed = 5.0f;		//����任�ٶ�
	//P47	ɾ��//float m_CameraRotation = 0.0f;			//����任�Ƕ�
	//P47	ɾ��//float m_CameraRotationSpeed = 180.0f;	//����Ƕ��ٶ�

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