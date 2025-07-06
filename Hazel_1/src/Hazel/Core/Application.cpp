#include "hzpch.h"
#include "Application.h"

#include "Log.h"

//P33	Application 中已经不会出现 OpenGL 代码了//#include <glad/glad.h> //#include <GLFW/glfw3.h> //glad 现在已经了解了 OpenGL 定义的所有内容
#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include <GLFW/glfw3.h>

namespace Hazel {

//std::bind 允许将成员函数指针 &x 和对象实例 this 绑定在一起，创建一个可以独立调用的可调用对象
//成员函数指针和普通的函数指针不同，成员函数指针需要一个对象实例来调用
//成员函数指针和 std::function 不兼容，std::function 期望一个可调用对象，所以成员函数指针需要使用 std::bind 创建一个新的可调用对象
//std::placeholders::_1 将 this 作为绑定函数的第一个输入变量
//可用 lambda 表达式替代 std::bind，按理说 lambda 比 std::bind 具有更好的性能，因为不需要创建额外的函数对象
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

	Application* Application::s_Instance = nullptr;

	Application::Application()
		//P36	删除//: m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{ //不会重复执行
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create()); //设置回调函数
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent)); //绑定回调函数，这里是打印日志
		//m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));

		Renderer::Init();

		m_ImGuiLayer = std::make_unique<ImGuiLayer>();
		PushOverlay(m_ImGuiLayer.get()); //get() 返回常规指针



	//P36---以下所有渲染代码全部移动到 Sandbox 中--------------------------------------
	//------------------------------------------------------------------------------

		/////////////////////////////////
		//Vertex Array 顶点数组		//P32
		/////////////////////////////////
		//P36	删除//m_VertexArray.reset(VertexArray::Create());
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//抽象至 VertexArray
		//P32	删除//glGenVertexArrays(1, &m_VertexArray);
		//P32	删除//glBindVertexArray(m_VertexArray);


		/////////////////////////////////////
		//Vretex Buffer	顶点缓冲区		//P28
		/////////////////////////////////////
		//P30	删除//glGenBuffers(1, &m_VertexBuffer);
		//P30	删除//glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);

		//P36	删除//float vertices[3 * 7] = {
		//P36	删除//	-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
		//P36	删除//	 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
		//P36	删除//	 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		//P36	删除//};
		//P36	删除//std::shared_ptr<VertexBuffer> vertexBuffer;
		//P36	删除//vertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
		//std::unique_ptr::reset 接管实例，这里 m_VertexBuffer 接管了 VertexBuffer 静态方法创建的实例，() 为空则释放管理对象
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//抽象至 VertexBuffer
		//P30	删除//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//BufferLayout layout({ {ShaderDataType::Float3, "a_Position"} }); //这种构造方法因为 initializer_list 需要一层 {} 来明确这是一个元素的列表，所以和 vector 一样需要两层 {}
		//P36	删除//BufferLayout layout = { 
		//P36	删除//	//initializer_list 可以传递给 BufferElement 所需的构造元素而不需要像 vector 单独构建本身
		//P36	删除//	{ ShaderDataType::Float3, "a_Position" },
		//P36	删除//	{ ShaderDataType::Float4, "a_Color" }
		//P36	删除//};
		
		//P36	删除//vertexBuffer->SetLayout(layout);
		//P36	删除//m_VertexArray->AddVertexBuffer(vertexBuffer);
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//抽象至 VertexArray
		//P32	删除//uint32_t index = 0;
		//P32	删除//const auto& layout = m_VertexBuffer->GetLayout();
		//P32	删除//for (const auto& element : layout)
		//P32	删除//{
		//P32	删除//	glEnableVertexAttribArray(index);
		//P32	删除//	glVertexAttribPointer(index,						//索引
		//P32	删除//		element.GetComponentCount(),					//数据个数
		//P32	删除//		ShaderDataTypeToOpenGLBaseType(element.Type),	//数据库类型
		//P32	删除//		element.Normalized ? GL_TRUE : GL_FALSE,		//是否初始化
		//P32	删除//		layout.GetStride(),								//步长
		//P32	删除//		(const void*)element.Offset);					//偏移量
		//P32	删除//	index++;
		//P32	删除//}

		//P31	删除//glEnableVertexAttribArray(0); //激活处于顶点着色器中位置为0的顶点属性数据，即glVertexAttribPointer的第一个参数，location = 0 即表示使用位置为0的顶点属性数据
		//P31	删除//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr); //告诉 OpenGL 如何解释指向缓冲区对象的数据并渲染它们

		/////////////////////////////////////
		//Index Buffer 索引缓冲区		//P28
		/////////////////////////////////////
		//P30	删除//glGenBuffers(1, &m_IndexBuffer);
		//P30	删除//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);

		//P36	删除//uint32_t indices[3] = { 0, 1, 2 };
		//P36	删除//std::shared_ptr<IndexBuffer> indexBuffer;
		//P36	删除//indexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
		//抽象至 IndexBuffer
		//P30	删除//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//P36	删除//m_VertexArray->SetIndexBuffer(indexBuffer);


		//P32	square test///////////////////////////////////
		//P36	删除//m_SquareVA.reset(VertexArray::Create());
		//P36	删除//float squareVertices[3 * 4] = {
		//P36	删除//	-0.75f, -0.75f, 0.0f, 
		//P36	删除//	 0.75f, -0.75f, 0.0f, 
		//P36	删除//	 0.75f,  0.75f, 0.0f,
		//P36	删除//	-0.75f,  0.75f, 0.0f
		//P36	删除//};
		//P36	删除//std::shared_ptr<VertexBuffer> squareVB;
		//P36	删除//squareVB.reset(VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		//P36	删除//squareVB->SetLayout({
		//P36	删除//	{ ShaderDataType::Float3, "a_Position" }
		//P36	删除//	});
		//P36	删除//m_SquareVA->AddVertexBuffer(squareVB);
		//P36	删除//
		//P36	删除//uint32_t squareIndices[2 * 3] = {
		//P36	删除//	0, 1, 2,
		//P36	删除//	2, 3, 0
		//P36	删除//};
		//P36	删除//std::shared_ptr<IndexBuffer> squareIB;
		//P36	删除//squareIB.reset(IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		//P36	删除//m_SquareVA->SetIndexBuffer(squareIB);


		/////////////////////////////////
		//Shader 着色器		//P29
		/////////////////////////////////
		
		//位置
		//P36	删除//std::string vertexSrc = R"(
		//P36	删除//	#version 330 core
		//P36	删除//	
		//P36	删除//	layout(location = 0) in vec3 a_Position;	//location = 0 和 glEnableVertexAttribArray(0) 对应 //输入一个 vec3 变量，这里是点的位置
		//P36	删除//	layout(location = 1) in vec4 a_Color;
 		//P36	删除//
		//P36	删除//	uniform mat4 u_ViewProjection;
		//P36	删除//	
		//P36	删除//	out vec3 v_Position;						//输出点的位置 vec3
		//P36	删除//	out vec4 v_Color;
		//P36	删除//
		//P36	删除//	void main()
		//P36	删除//	{
		//P36	删除//		v_Position = a_Position;
		//P36	删除//		v_Color = a_Color;
		//P36	删除//		gl_Position = u_ViewProjection * vec4(a_Position, 1.0);	//告诉 gl_Position 这个点的位置，第四个数据表示窗口半边长为 1.0	//VP矩阵*模型矩阵得到相机中的图像
		//P36	删除//	}
		//P36	删除//)";

		//颜色
		//P36	删除//std::string fragmentSrc = R"(
		//P36	删除//	#version 330 core
		//P36	删除//	
		//P36	删除//	layout(location = 0) out vec4 color;		//输出一个 vec4 变量，这里是像素的rgba
		//P36	删除//
		//P36	删除//	in vec3 v_Position;							//和顶点着色器对应，变量名和类型必须一模一样
		//P36	删除//	in vec4 v_Color;
		//P36	删除//
		//P36	删除//	void main()
		//P36	删除//	{
		//P36	删除//		color = vec4((v_Position + 1.0) / 2, 1.0);			//赋予 color 颜色
		//P36	删除//		color = v_Color;
		//P36	删除//	}
		//P36	删除//)";

		//P36	删除//m_Shader.reset(new Shader(vertexSrc, fragmentSrc));
		//m_Shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);


		//P32	square test///////////////////////////////////
		//P36	删除//std::string blueShaderVertexSrc = R"(
		//P36	删除//	#version 330 core
		//P36	删除//	
		//P36	删除//	layout(location = 0) in vec3 a_Position;	
		//P36	删除//
		//P36	删除//	uniform mat4 u_ViewProjection;
 		//P36	删除//
		//P36	删除//	out vec3 v_Position;						
		//P36	删除//
		//P36	删除//	void main()
		//P36	删除//	{
		//P36	删除//		v_Position = a_Position;
		//P36	删除//		gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
		//P36	删除//	}
		//P36	删除//)";
		//P36	删除//std::string blueShaderFragmentSrc = R"(
		//P36	删除//	#version 330 core
		//P36	删除//	
		//P36	删除//	layout(location = 0) out vec4 color;		
		//P36	删除//
		//P36	删除//	in vec3 v_Position;							
		//P36	删除//
		//P36	删除//	void main()
		//P36	删除//	{
		//P36	删除//		color = vec4(0.2, 0.3, 0.8, 1.0);
		//P36	删除//	}
		//P36	删除//)";

		//P36	删除//m_BlueShader.reset(new Shader(blueShaderVertexSrc, blueShaderFragmentSrc));
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
	{ //会重复执行
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//设置 关闭窗口 的回调函数
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));	//设置 

		//HZ_CORE_TRACE("{0}", e); //打印日志

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); )
		{
			//通过上一层的回调函数改变 m_Handled 来决定下一层的行动
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
			//P36	删除//RenderCommand::SetClearColor({ 0.1, 0.1, 0.1, 1 }); 
			//P36	删除//RenderCommand::Clear();
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			//抽象至 RenderCommand
			//P32	删除//glClearColor(0.1, 0.1, 0.1, 1);
			//P32	删除//glClear(GL_COLOR_BUFFER_BIT);
			//P36	删除//m_Camera.SetPosition({ Input::GetMouseX()/-1000, Input::GetMouseY()/1000, 0.0f});
			//P36	删除//m_Camera.SetRotation(45.0f);

			//P36	删除//Renderer::BeginScene(m_Camera);
			
			//P35	删除//m_BlueShader->Bind();
			//P35	删除//m_BlueShader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			//P36	删除//Renderer::Submit(m_BlueShader, m_SquareVA);
			//P35	删除//m_Shader->Bind();
			//P35	删除//m_Shader->UploadUniformMat4("u_ViewProjection", m_Camera.GetViewProjectionMatrix());
			//P36	删除//Renderer::Submit(m_Shader, m_VertexArray);

			//P36	删除//Renderer::EndScene();
			//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
			//抽象至 Renderer
			//P32	square test///////////////////////////////////
			//P33	删除//m_BlueShader->Bind();
			//P33	删除//m_SquareVA->Bind();
			//P33	删除//glDrawElements(GL_TRIANGLES, m_SquareVA->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
			//P33	删除//
			//P33	删除//
			//P33	删除//m_Shader->Bind();
			//P33	删除//m_VertexArray->Bind();//<<<<抽象至P32// glBindVertexArray(m_VertexArray);	//P28
			//P33	删除//glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);	//P28

		//P36---以上所有渲染代码全部移动到 Sandbox 中--------------------------------------
		//------------------------------------------------------------------------------


			//计算每个循环的时间，即每帧的时间
			float time = (float)glfwGetTime();	//Platfomr::GetTime
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if (!m_Minimized)
			{
				for (Layer* layer : m_LayerStack) //LayerStack 内提供了 Layer* 的开始和结束迭代器，可直接遍历
					layer->OnUpdate(timestep);
			}

			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender();
			m_ImGuiLayer->End();

			//HZ_INFO("MousePosition: ({0}, {1})", Input::GetMouseX(), Input::GetMouseY());

			m_Window->OnUpdate(); //如果中断挂起，即关闭窗口
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{ //关闭窗口中止渲染
		m_Running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{ //获取窗口大小并设置视口大小与窗口大小相同
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