#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

#include "Platform/OpenGL/OpenGLContext.h"

//#include <glad/glad.h> //��֤�� #include <glfw3.h> ��ʹ�� //��Ҫ���� GLFW_INCLUDE_NONE Ԥ����������ʹ�� //�ڲ����� OpenGL ���ж��� //ʹ�ú󲻿�ʹ�� #include <glfw3.h>����ֱ��ʹ�� #include <glad.h> ����

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
		Init(props); //������ WindowsWindow ʱ���ûص�����
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
			//TODO: ��ϵͳ�ر�ʱ����
			int success = glfwInit();
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		m_Context = new OpenGLContext(m_Window); //�����������๹����ֻ��ʹ������������Ļ����Ա����
		m_Context->Init();
		//^^^^^^^^^^^^^^^
		//���������溯��
		//glfwMakeContextCurrent(m_Window); //���� glfw ������
		//
		//gladLoadGLLoader ���ڳ�ʼ�� OpenGL ����ָ��
		//glfwGetProcAddress ���ڻ�ȡ OpenGL �� OpenGL ES ������ַ
		//GLADloadproc ��һ������ָ������
		//status ���� 1 �����ȡ OpenGL �����ɹ�
		//glad ��ȡ�� OpenGL ������������ݣ����� glfw3 ���� glad ���
		//int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		//HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data); //�������ھ��
		SetVSync(true);

		//���� GLFW �ص�����
		//����־����� event.ToString() ��
		//�ص������ʹ��ھ������һ�𣬴�����ÿһ�η����¼�����ִ����Ӧ�Ĵ���
		//���� lambda ���������ûص�����������ֻ�Ǵ������ݣ����Բ���Ҫ��ѭ����ѭ���� glfwPollEvents() �����ᴦ����Щ�ص�����
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{//���ô��ڴ�С
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);//GLFW�����л�ȡ��֮�������û��������ݣ�WindowData���ͣ�������������ݰ󶨵�һ��������
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event); //����������ûص��������� Application::OnEvent(Event& e)

			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{//�رմ���
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{//���̰���
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
					case GLFW_PRESS:
					{//����
						KeyPressedEvent event(key, 0);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{//�ͷ�
						KeyReleasedEvent event(key);
						data.EventCallback(event);
						break;
					}
					case GLFW_REPEAT:
					{//�ظ�
						KeyPressedEvent event(key, 1);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{//����
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{//��갴��
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
					case GLFW_PRESS:
					{//����
						MouseButtonPressedEvent event(button);
						data.EventCallback(event);
						break;
					}
					case GLFW_RELEASE:
					{//�ͷ�
						MouseButtonReleasedEvent event(button);
						data.EventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{//������
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{//���λ��
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
		glfwPollEvents(); //�������й�����¼� //������ Init ������Ļص�����
		m_Context->SwapBuffers();
		//^^^^^^^^^^^^^^^^^^^^^^
		//���������溯��
		//glfwSwapBuffers(m_Window); //������ɫ������
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1); //������ֱͬ�� 60֡/s
		else
			glfwSwapInterval(0); //�����ô�ֱͬ��

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}