#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"

#define IMGUI_IMPL_API
#include "../vendor/imgui/backends/imgui_impl_glfw.h"
#include "../vendor/imgui/backends/imgui_impl_opengl3.h"

#include "Hazel/Core/Application.h"

// TEMPORARY
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel {

	ImGuiLayer::ImGuiLayer()
		: Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		//P22  ɾ��//ImGui::CreateContext(); //����һ������� ImGui ������
		//P22  ɾ��//ImGui::StyleColorsDark(); //Ӧ�� ImGui ��ɫ����
		//P22  ɾ��//
		//P22  ɾ��//ImGuiIO& io = ImGui::GetIO(); //��ȡ ImGui �� IO �ӿ�
		//P22  ɾ��//
		//P22  ɾ��////�����������������λ�õĹ���
		//P22  ɾ��//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//P22  ɾ��//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		//P22  ɾ��//
		//P22  ɾ��////TEMPORARY: should eventually use Hazel key codes
		//P22  ɾ��////ӳ�� GLFW �ļ��̰����� ImGui �ļ��̰���
		//P22  ɾ��//io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		//P22  ɾ��//io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		//P22  ɾ��//io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		//P22  ɾ��//io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		//P22  ɾ��//io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		//P22  ɾ��//io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		//P22  ɾ��//io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		//P22  ɾ��//io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		//P22  ɾ��//io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		//P22  ɾ��//io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		//P22  ɾ��//io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		//P22  ɾ��//io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		//P22  ɾ��//io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
		//P22  ɾ��//
		//P22  ɾ��//ImGui_ImplOpenGL3_Init("#version 410"); //��ʼ���� ImGui �� OpenGL3 ��Ⱦ�����

		//Setup Dear ImGui context
		IMGUI_CHECKVERSION(); //���汾��ȷ�� ImGui ��İ汾��������
		ImGui::CreateContext(); //����������
		ImGuiIO& io = ImGui::GetIO(); (void)io; //��ȡ ImGui �� IO �ӿ�
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		//���ü��̿���
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		//�����ֱ�����
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			//����ͣ��
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			//���ö��ӽ�/ƽ̨����
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		//Setup Dear ImGui style
		ImGui::StyleColorsDark();		//��ɫ����
		//ImGui::StyleColorsClassic();	//��������

		//when viewports are enables we tweak WindowRounding/WindowBg so platform windows can look identical to
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f; //���ñ�����ɫ͸����Ϊ1.0����͸����
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		//Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true); //��ʼ�� ImGui �� GLFW �� OpenGL ��
		ImGui_ImplOpenGL3_Init("#version 410");		//��ɫ���汾��
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();	//�رղ����� ImGui �� OpenGL3 ��Ⱦ��
		ImGui_ImplGlfw_Shutdown();		//�رղ����� ImGui �� GLFW ֮��İ�
		ImGui::DestroyContext();		//���� ImGui ��������
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();	//֪ͨ ��Ⱦ�� �µ�һ֡������ʼ��OpenGL ��ĳЩ״̬��Ҫ�����ú͸���
		ImGui_ImplGlfw_NewFrame();		//֪ͨ �� �µ�һ֡������ʼ����������ƶ������̰����� GLFW �����¼��������ݸ� ImGui
		ImGui::NewFrame();				//��ʾ�µ�һ֡�Ŀ�ʼ
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight()); //���� ImGui ����ʾ��С

		//Rendering
		ImGui::Render(); //׼������ GUI ��Ⱦ����
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //GetDrawData ��ȡ׼���õ���Ⱦ���ݣ�RenderDrawData ��������Ⱦ�� OpenGL ��������

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) //����Ƿ������˶��ӽǹ���
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); //���浱ǰ OpenGL ��������
			ImGui::UpdatePlatformWindows(); //���� ImGui ���������ƽ̨����
			ImGui::RenderPlatformWindowsDefault(); //��Ⱦ���� ImGui �����ƽ̨����
			glfwMakeContextCurrent(backup_current_context); //�� OpenGL ���������л���֮ǰ������Ǹ�
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	//P22  ɾ��//void ImGuiLayer::OnUpdate()
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO(); //��ȡ ImGui �� IO ʵ�������������ϵͳ���롢�����������صĽӿ�
	//P22  ɾ��//	Application& app = Application::Get(); //��ȡ Application ��ʵ��
	//P22  ɾ��//	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()); //����Ϊ��ǰ���򴰿ڵĳߴ�
	//P22  ɾ��//
	//P22  ɾ��//	float time = (float)glfwGetTime(); //��ȡ��ǰʱ��
	//P22  ɾ��//	io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f); //���㲢����ʱ����
	//P22  ɾ��//	m_Time = time;
	//P22  ɾ��//
	//P22  ɾ��//	ImGui_ImplOpenGL3_NewFrame(); //֪ͨ ImGui �� OpenGL3 ��˿�ʼһ���µ���Ⱦ֡
	//P22  ɾ��//	ImGui::NewFrame(); //��ʼһ���µ� ImGui ֡
	//P22  ɾ��//
	//P22  ɾ��//	static bool show = true;
	//P22  ɾ��//	ImGui::ShowDemoWindow(&show); //��ʾ ImGui ��ʵ������
	//P22  ɾ��//
	//P22  ɾ��//	ImGui::Render(); //��Ⱦ ImGui �Ļ����б�
	//P22  ɾ��//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //�� ImGui �Ļ���������Ⱦ�� OpenGL3 ����������
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//void ImGuiLayer::OnEvent(Event& event)
	//P22  ɾ��//{
	//P22  ɾ��//	EventDispatcher dispatcher(event);
	//P22  ɾ��//	//���ûص�����
	//P22  ɾ��//	//ImGuiIO �ṹ�����Ϊ�û��Ķ����ı䣬ÿ֡���ܻ�ı䣬���Խ�ѭ��
	//P22  ɾ��//	dispatcher.Dispatch<MouseButtonPressedEvent>	(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<MouseButtonReleasedEvent>	(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<MouseMovedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<MouseScrolledEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	//P22  ɾ��//	dispatcher.Dispatch<KeyPressedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<KeyReleasedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<KeyTypedEvent>				(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	//P22  ɾ��//	dispatcher.Dispatch<WindowResizeEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.MouseDown[e.GetMouseButton()] = true; //io.MouseDown ��һ����¼��갴�������� //e.GetMouseButton() ����갴�������� //= true �����˰���״̬Ϊ true
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.MouseDown[e.GetMouseButton()] = false;
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.MousePos = ImVec2(e.GetX(), e.GetY()); //io.MousePos ���ڴ洢���λ��
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.MouseWheelH += e.GetXOffset(); //��¼ˮƽ������Ϊ
	//P22  ɾ��//	io.MouseWheel += e.GetYOffset(); //��¼��ֱ������Ϊ
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.KeysDown[e.GetKeyCode()] = true;
	//P22  ɾ��//
	//P22  ɾ��//	io.KeyCtrl	= io.KeysDown[GLFW_KEY_LEFT_CONTROL]	|| io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//P22  ɾ��//	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]		|| io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//P22  ɾ��//	io.KeyAlt	= io.KeysDown[GLFW_KEY_LEFT_ALT]		|| io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//P22  ɾ��//	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]		|| io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.KeysDown[e.GetKeyCode()] = false;
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	//P22  ɾ��//{
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	int Keycode = e.GetKeyCode();
	//P22  ɾ��//	if (Keycode > 0 && Keycode < 0x10000)
	//P22  ɾ��//		io.AddInputCharacter((unsigned short)Keycode); //io.AddInputCharacter ������ ImGui ���һ���ַ�����
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
	//P22  ɾ��//
	//P22  ɾ��//bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	//P22  ɾ��//{ //�� ImGui �򲻻���ʧ�� Window ����������ٱ��� 1.0f �Ŀ��
	//P22  ɾ��//	ImGuiIO& io = ImGui::GetIO();
	//P22  ɾ��//	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	//P22  ɾ��//	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	//P22  ɾ��//	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	//P22  ɾ��//
	//P22  ɾ��//	return false;
	//P22  ɾ��//}
}