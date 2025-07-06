#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

//#include <glad/glad.h> //保证在 #include <glfw3.h> 后使用 //需要定义 GLFW_INCLUDE_NONE 预处理器才能使用 //内部包含 OpenGL 所有定义 //使用后不可使用 #include <glfw3.h>，可直接使用 #include <glad.h> 代替

namespace Hazel {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error_code, const char* description)
	{
		HZ_CORE_ERROR("GLFW Error ({0}): {1}", error_code, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props); //在生成 WindowsWindow 时设置回调函数
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			//TODO: 在系统关闭时结束
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window); //基类用派生类构建后，只能使用派生类包含的基类成员函数
		m_Context->Init();
		//^^^^^^^^^^^^^^^
		//抽象至上面函数
		//glfwMakeContextCurrent(m_Window); //创建 glfw 上下文
		//
		//gladLoadGLLoader 用于初始化 OpenGL 函数指针
		//glfwGetProcAddress 用于获取 OpenGL 或 OpenGL ES 函数地址
		//GLADloadproc 是一个函数指针类型
		//status 返回 1 代表获取 OpenGL 函数成功
		//glad 获取了 OpenGL 定义的所有内容，后面 glfw3 需用 glad 替代
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data); //创建窗口句柄
		SetVSync(true);

		//设置 GLFW 回调函数
		//将日志存放在 event.ToString() 内
		//回调函数和窗口句柄绑定在一起，窗口中每一次发生事件都会执行相应的代码
		//传递 lambda 函数来设置回调函数，这里只是传递数据，所以不需要进循环，循环里 glfwPollEvents() 函数会处理这些回调函数
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{//设置窗口大小
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);//GLFW窗口中获取与之关联的用户定义数据（WindowData类型），并将这个数据绑定到一个引用上
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event); //会在这里调用回调函数，即 Application::OnEvent(Event& e)

			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{//关闭窗口
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{//键盘按键
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{//按下
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{//释放
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{//重复
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{//打字
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{//鼠标按键
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{//按下
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{//释放
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{//鼠标滚轮
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{//鼠标位置
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents(); //处理所有挂起的事件 //处理在 Init 函数里的回调函数
		m_Context->SwapBuffers();
		//^^^^^^^^^^^^^^^^^^^^^^
		//抽象至上面函数
		//glfwSwapBuffers(m_Window); //交换颜色缓冲区
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1); //开启垂直同步 60帧/s
		else
			glfwSwapInterval(0); //不启用垂直同步

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}