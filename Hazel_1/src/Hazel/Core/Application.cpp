#include "hzpch.h"
#include "Application.h"

#include "Log.h"

//P33	Application ���Ѿ�������� OpenGL ������//#include <glad/glad.h> //#include <GLFW/glfw3.h> //glad �����Ѿ��˽��� OpenGL �������������
#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Hazel {

//std::bind ������Ա����ָ�� &x �Ͷ���ʵ�� this ����һ�𣬴���һ�����Զ������õĿɵ��ö���
//��Ա����ָ�����ͨ�ĺ���ָ�벻ͬ����Ա����ָ����Ҫһ������ʵ��������
//��Ա����ָ��� std::function �����ݣ�std::function ����һ���ɵ��ö������Գ�Ա����ָ����Ҫʹ�� std::bind ����һ���µĿɵ��ö���
//std::placeholders::_1 �� this ��Ϊ�󶨺����ĵ�һ���������
//���� lambda ���ʽ��� std::bind������˵ lambda �� std::bind ���и��õ����ܣ���Ϊ����Ҫ��������ĺ�������
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		//P36	ɾ��//: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{ //�����ظ�ִ��
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create()); //���ûص�����
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); //�󶨻ص������������Ǵ�ӡ��־
		//m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Renderer::Init();

		m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer.get()); //get() ���س���ָ��



	//P36---����������Ⱦ����ȫ���ƶ��� Sandbox ��--------------------------------------
	//------------------------------------------------------------------------------

		/////////////////////////////////
		//Vertex Array ��������		//P32
		/////////////////////////////////
		//P36	ɾ��//m_VertexArray.reset(VertexArray::Create());
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//������ VertexArray
		//P32	ɾ��//glGenVertexArrays(1, &m_VertexArray);
		//P32	ɾ��//glBindVertexArray(m_VertexArray);


		/////////////////////////////////////
		//Vretex Buffer	���㻺����		//P28
		/////////////////////////////////////
		//P30	ɾ��//glGenBuffers(1, &m_VertexBuffer);
		//P30	ɾ��//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		//P36	ɾ��//float vertices[3 * 7] = {
		//P36	ɾ��//	-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		//P36	ɾ��//	 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		//P36	ɾ��//	 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		//P36	ɾ��//};
		//P36	ɾ��//std::shared_ptr<VertexBuffer> vertexBuffer;
		//P36	ɾ��//vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//std::unique_ptr::reset �ӹ�ʵ�������� m_VertexBuffer �ӹ��� VertexBuffer ��̬����������ʵ����() Ϊ�����ͷŹ������
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//������ VertexBuffer
		//P30	ɾ��//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//BufferLayout layout({ {ShaderDataType::Float3, "a_Position"} }); //���ֹ��췽����Ϊ initializer_list ��Ҫһ�� {} ����ȷ����һ��Ԫ�ص��б����Ժ� vector һ����Ҫ���� {}
		//P36	ɾ��//BufferLayout layout = { 
		//P36	ɾ��//	//initializer_list ���Դ��ݸ� BufferElement ����Ĺ���Ԫ�ض�����Ҫ�� vector ������������
		//P36	ɾ��//	{ ShaderDataType::Float3, "a_Position" },
		//P36	ɾ��//	{ ShaderDataType::Float4, "a_Color" }
		//P36	ɾ��//};
		
		//P36	ɾ��//vertexBuffer->SetLayout(layout);
		//P36	ɾ��//m_VertexArray->AddVertexBuffer(vertexBuffer);
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//������ VertexArray
		//P32	ɾ��//uint32_t index = 0;
		//P32	ɾ��//const auto& layout = m_VertexBuffer->GetLayout();
		//P32	ɾ��//for (const auto& element : layout)
		//P32	ɾ��//{
		//P32	ɾ��//	glEnableVertexAttribArray(index);
		//P32	ɾ��//	glVertexAttribPointer(index,						//����
		//P32	ɾ��//		element.GetComponentCount(),					//���ݸ���
		//P32	ɾ��//		ShaderDataTypeToOpenGLBaseType(element.Type),	//���ݿ�����
		//P32	ɾ��//		element.Normalized ? GL_TRUE : GL_FALSE,		//�Ƿ��ʼ��
		//P32	ɾ��//		layout.GetStride(),								//����
		//P32	ɾ��//		(const void*)element.Offset);					//ƫ����
		//P32	ɾ��//	index++;
		//P32	ɾ��//}

		//P31	ɾ��//glEnableVertexAttribArray(0); //����ڶ�����ɫ����λ��Ϊ0�Ķ����������ݣ���glVertexAttribPointer�ĵ�һ��������location = 0 ����ʾʹ��λ��Ϊ0�Ķ�����������
		//P31	ɾ��//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); //���� OpenGL ��ν���ָ�򻺳�����������ݲ���Ⱦ����

		/////////////////////////////////////
		//Index Buffer ����������		//P28
		/////////////////////////////////////
		//P30	ɾ��//glGenBuffers(1, &m_IndexBuffer);
		//P30	ɾ��//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//P36	ɾ��//uint32_t indices[3] = { 0, 1, 2 };
		//P36	ɾ��//std::shared_ptr<IndexBuffer> indexBuffer;
		//P36	ɾ��//indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//������ IndexBuffer
		//P30	ɾ��//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//P36	ɾ��//m_VertexArray->SetIndexBuffer(indexBuffer);


		//P32	square test///////////////////////////////////
		//P36	ɾ��//m_SquareVA.reset(VertexArray::Create());
		//P36	ɾ��//float squareVertices[3 * 4] = {
		//P36	ɾ��//	-0.75f, -0.75f, 0.0f, 
		//P36	ɾ��//	 0.75f, -0.75f, 0.0f, 
		//P36	ɾ��//	 0.75f,  0.75f, 0.0f,
		//P36	ɾ��//	-0.75f,  0.75f, 0.0f
		//P36	ɾ��//};
		//P36	ɾ��//std::shared_ptr<VertexBuffer> squareVB;
		//P36	ɾ��//squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		//P36	ɾ��//squareVB->SetLayout({
		//P36	ɾ��//	{ ShaderDataType::Float3, "a_Position" }
		//P36	ɾ��//	});
		//P36	ɾ��//m_SquareVA->AddVertexBuffer(squareVB);
		//P36	ɾ��//
		//P36	ɾ��//uint32_t squareIndices[2 * 3] = {
		//P36	ɾ��//	0, 1, 2,
		//P36	ɾ��//	2, 3, 0
		//P36	ɾ��//};
		//P36	ɾ��//std::shared_ptr<IndexBuffer> squareIB;
		//P36	ɾ��//squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		//P36	ɾ��//m_SquareVA->SetIndexBuffer(squareIB);


		/////////////////////////////////
		//Shader ��ɫ��		//P29
		/////////////////////////////////
		
		//λ��
		//P36	ɾ��//std::string vertexSrc = R"(
		//P36	ɾ��//	#version 330 core
		//P36	ɾ��//	
		//P36	ɾ��//	layout(location = 0) in vec3 a_Position;	//location = 0 �� glEnableVertexAttribArray(0) ��Ӧ //����һ�� vec3 �����������ǵ��λ��
		//P36	ɾ��//	layout(location = 1) in vec4 a_Color;
 		//P36	ɾ��//
		//P36	ɾ��//	uniform mat4 u_ViewProjection;
		//P36	ɾ��//	
		//P36	ɾ��//	out vec3 v_Position;						//������λ�� vec3
		//P36	ɾ��//	out vec4 v_Color;
		//P36	ɾ��//
		//P36	ɾ��//	void main()
		//P36	ɾ��//	{
		//P36	ɾ��//		v_Position = a_Position;
		//P36	ɾ��//		v_Color = a_Color;
		//P36	ɾ��//		gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	//���� gl_Position ������λ�ã����ĸ����ݱ�ʾ���ڰ�߳�Ϊ 1.0	//VP����*ģ�;���õ�����е�ͼ��
		//P36	ɾ��//	}
		//P36	ɾ��//)";

		//��ɫ
		//P36	ɾ��//std::string fragmentSrc = R"(
		//P36	ɾ��//	#version 330 core
		//P36	ɾ��//	
		//P36	ɾ��//	layout(location = 0) out vec4 color;		//���һ�� vec4 ���������������ص�rgba
		//P36	ɾ��//
		//P36	ɾ��//	in vec3 v_Position;							//�Ͷ�����ɫ����Ӧ�������������ͱ���һģһ��
		//P36	ɾ��//	in vec4 v_Color;
		//P36	ɾ��//
		//P36	ɾ��//	void main()
		//P36	ɾ��//	{
		//P36	ɾ��//		color = vec4((v_Position + 1.0) / 2, 1.0);			//���� color ��ɫ
		//P36	ɾ��//		color = v_Color;
		//P36	ɾ��//	}
		//P36	ɾ��//)";

		//P36	ɾ��//m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		//m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);


		//P32	square test///////////////////////////////////
		//P36	ɾ��//std::string blueShaderVertexSrc = R"(
		//P36	ɾ��//	#version 330 core
		//P36	ɾ��//	
		//P36	ɾ��//	layout(location = 0) in vec3 a_Position;	
		//P36	ɾ��//
		//P36	ɾ��//	uniform mat4 u_ViewProjection;
 		//P36	ɾ��//
		//P36	ɾ��//	out vec3 v_Position;						
		//P36	ɾ��//
		//P36	ɾ��//	void main()
		//P36	ɾ��//	{
		//P36	ɾ��//		v_Position = a_Position;
		//P36	ɾ��//		gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		//P36	ɾ��//	}
		//P36	ɾ��//)";
		//P36	ɾ��//std::string blueShaderFragmentSrc = R"(
		//P36	ɾ��//	#version 330 core
		//P36	ɾ��//	
		//P36	ɾ��//	layout(location = 0) out vec4 color;		
		//P36	ɾ��//
		//P36	ɾ��//	in vec3 v_Position;							
		//P36	ɾ��//
		//P36	ɾ��//	void main()
		//P36	ɾ��//	{
		//P36	ɾ��//		color = vec4(0.2, 0.3, 0.8, 1.0);
		//P36	ɾ��//	}
		//P36	ɾ��//)";

		//P36	ɾ��//m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{ //���ظ�ִ��
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//���� �رմ��� �Ļص�����
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));	//���� 

		//HZ_CORE_TRACE("{0}", e); //��ӡ��־

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			//ͨ����һ��Ļص������ı� m_Handled ��������һ����ж�
			(*--it)->OnEvent(e);
			if (e.m_Handled)
				break;
		}
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			HZ_TRACE("{0}", e);
			
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			HZ_TRACE(e);
		}

		while (m_Running)
		{
			//P36	ɾ��//RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 }); 
			//P36	ɾ��//RenderCommand::Clear();
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			//������ RenderCommand
			//P32	ɾ��//glClearColor(0.1, 0.1, 0.1, 1);
			//P32	ɾ��//glClear(GL_COLOR_BUFFER_BIT);
			//P36	ɾ��//m_Camera.SetPosition({ Input::GetMouseX()/-1000, Input::GetMouseY()/1000, 0.0f});
			//P36	ɾ��//m_Camera.SetRotation(45.0f);

			//P36	ɾ��//Renderer::BeginScene(m_Camera);
			
			//P35	ɾ��//m_BlueShader->Bind();
			//P35	ɾ��//m_BlueShader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			//P36	ɾ��//Renderer::Submit(m_BlueShader, m_SquareVA);
			//P35	ɾ��//m_Shader->Bind();
			//P35	ɾ��//m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			//P36	ɾ��//Renderer::Submit(m_Shader, m_VertexArray);

			//P36	ɾ��//Renderer::EndScene();
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			//������ Renderer
			//P32	square test///////////////////////////////////
			//P33	ɾ��//m_BlueShader->Bind();
			//P33	ɾ��//m_SquareVA->Bind();
			//P33	ɾ��//glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			//P33	ɾ��//
			//P33	ɾ��//
			//P33	ɾ��//m_Shader->Bind();
			//P33	ɾ��//m_VertexArray->Bind();//<<<<������P32// glBindVertexArray(m_VertexArray);	//P28
			//P33	ɾ��//glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);	//P28

		//P36---����������Ⱦ����ȫ���ƶ��� Sandbox ��--------------------------------------
		//------------------------------------------------------------------------------


			//����ÿ��ѭ����ʱ�䣬��ÿ֡��ʱ��
			float time = (float)glfwGetTime();	//Platfomr::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack) //LayerStack ���ṩ�� Layer* �Ŀ�ʼ�ͽ�������������ֱ�ӱ���
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			//HZ_INFO("MousePosition: ({0}, {1})", Input::GetMouseX(), Input::GetMouseY());

			m_Window->OnUpdate(); //����жϹ��𣬼��رմ���
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{ //�رմ�����ֹ��Ⱦ
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{ //��ȡ���ڴ�С�������ӿڴ�С�봰�ڴ�С��ͬ
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

		return false;
	}

}