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
		//P22  删除//ImGui::CreateContext(); //创建一个虚拟的 ImGui 上下文
		//P22  删除//ImGui::StyleColorsDark(); //应用 ImGui 颜色主题
		//P22  删除//
		//P22  删除//ImGuiIO& io = ImGui::GetIO(); //获取 ImGui 的 IO 接口
		//P22  删除//
		//P22  删除////启用鼠标光标和设置鼠标位置的功能
		//P22  删除//io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		//P22  删除//io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		//P22  删除//
		//P22  删除////TEMPORARY: should eventually use Hazel key codes
		//P22  删除////映射 GLFW 的键盘按键到 ImGui 的键盘按键
		//P22  删除//io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		//P22  删除//io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		//P22  删除//io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		//P22  删除//io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		//P22  删除//io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		//P22  删除//io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		//P22  删除//io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		//P22  删除//io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		//P22  删除//io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		//P22  删除//io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		//P22  删除//io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		//P22  删除//io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		//P22  删除//io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		//P22  删除//io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		//P22  删除//io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		//P22  删除//io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		//P22  删除//io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		//P22  删除//io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		//P22  删除//io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		//P22  删除//io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		//P22  删除//io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
		//P22  删除//
		//P22  删除//ImGui_ImplOpenGL3_Init("#version 410"); //初始化了 ImGui 的 OpenGL3 渲染器后端

		//Setup Dear ImGui context
		IMGUI_CHECKVERSION(); //检查版本，确保 ImGui 库的版本与代码兼容
		ImGui::CreateContext(); //创建上下文
		ImGuiIO& io = ImGui::GetIO(); (void)io; //获取 ImGui 的 IO 接口
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		//启用键盘控制
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;		//启用手柄控制
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			//启用停靠
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			//启用多视角/平台窗口
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		//Setup Dear ImGui style
		ImGui::StyleColorsDark();		//深色主题
		//ImGui::StyleColorsClassic();	//经典主题

		//when viewports are enables we tweak WindowRounding/WindowBg so platform windows can look identical to
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f; //设置背景颜色透明度为1.0（不透明）
		}

		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		//Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true); //初始化 ImGui 的 GLFW 和 OpenGL 绑定
		ImGui_ImplOpenGL3_Init("#version 410");		//着色器版本号
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();	//关闭并清理 ImGui 的 OpenGL3 渲染器
		ImGui_ImplGlfw_Shutdown();		//关闭并清理 ImGui 与 GLFW 之间的绑定
		ImGui::DestroyContext();		//销毁 ImGui 的上下文
	}

	void ImGuiLayer::Begin()
	{
		ImGui_ImplOpenGL3_NewFrame();	//通知 渲染器 新的一帧即将开始，OpenGL 的某些状态需要被重置和更新
		ImGui_ImplGlfw_NewFrame();		//通知 绑定 新的一帧即将开始，处理鼠标移动、键盘按键等 GLFW 输入事件，并传递给 ImGui
		ImGui::NewFrame();				//表示新的一帧的开始
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight()); //设置 ImGui 的显示大小

		//Rendering
		ImGui::Render(); //准备所有 GUI 渲染数据
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //GetDrawData 获取准备好的渲染数据，RenderDrawData 将数据渲染到 OpenGL 上下文中

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) //检查是否启用了多视角功能
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); //保存当前 OpenGL 的上下文
			ImGui::UpdatePlatformWindows(); //更新 ImGui 管理的所有平台窗口
			ImGui::RenderPlatformWindowsDefault(); //渲染所有 ImGui 管理的平台窗口
			glfwMakeContextCurrent(backup_current_context); //将 OpenGL 的上下文切换回之前保存的那个
		}
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	//P22  删除//void ImGuiLayer::OnUpdate()
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO(); //获取 ImGui 的 IO 实例，这是与操作系统输入、输出和配置相关的接口
	//P22  删除//	Application& app = Application::Get(); //获取 Application 的实例
	//P22  删除//	io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight()); //设置为当前程序窗口的尺寸
	//P22  删除//
	//P22  删除//	float time = (float)glfwGetTime(); //获取当前时间
	//P22  删除//	io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f); //计算并设置时间间隔
	//P22  删除//	m_Time = time;
	//P22  删除//
	//P22  删除//	ImGui_ImplOpenGL3_NewFrame(); //通知 ImGui 的 OpenGL3 后端开始一个新的渲染帧
	//P22  删除//	ImGui::NewFrame(); //开始一个新的 ImGui 帧
	//P22  删除//
	//P22  删除//	static bool show = true;
	//P22  删除//	ImGui::ShowDemoWindow(&show); //显示 ImGui 的实例窗口
	//P22  删除//
	//P22  删除//	ImGui::Render(); //渲染 ImGui 的绘制列表
	//P22  删除//	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); //将 ImGui 的绘制数据渲染到 OpenGL3 的上下文中
	//P22  删除//}
	//P22  删除//
	//P22  删除//void ImGuiLayer::OnEvent(Event& event)
	//P22  删除//{
	//P22  删除//	EventDispatcher dispatcher(event);
	//P22  删除//	//设置回调函数
	//P22  删除//	//ImGuiIO 结构体会因为用户的动作改变，每帧可能会改变，所以进循环
	//P22  删除//	dispatcher.Dispatch<MouseButtonPressedEvent>	(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
	//P22  删除//	dispatcher.Dispatch<MouseButtonReleasedEvent>	(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
	//P22  删除//	dispatcher.Dispatch<MouseMovedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
	//P22  删除//	dispatcher.Dispatch<MouseScrolledEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
	//P22  删除//	dispatcher.Dispatch<KeyPressedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
	//P22  删除//	dispatcher.Dispatch<KeyReleasedEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
	//P22  删除//	dispatcher.Dispatch<KeyTypedEvent>				(HZ_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
	//P22  删除//	dispatcher.Dispatch<WindowResizeEvent>			(HZ_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.MouseDown[e.GetMouseButton()] = true; //io.MouseDown 是一个记录鼠标按键的数组 //e.GetMouseButton() 是鼠标按键的索引 //= true 设置了按下状态为 true
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.MouseDown[e.GetMouseButton()] = false;
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.MousePos = ImVec2(e.GetX(), e.GetY()); //io.MousePos 用于存储鼠标位置
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.MouseWheelH += e.GetXOffset(); //记录水平滚轮行为
	//P22  删除//	io.MouseWheel += e.GetYOffset(); //记录垂直滚轮行为
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.KeysDown[e.GetKeyCode()] = true;
	//P22  删除//
	//P22  删除//	io.KeyCtrl	= io.KeysDown[GLFW_KEY_LEFT_CONTROL]	|| io.KeysDown[GLFW_KEY_RIGHT_CONTROL];
	//P22  删除//	io.KeyShift = io.KeysDown[GLFW_KEY_LEFT_SHIFT]		|| io.KeysDown[GLFW_KEY_RIGHT_SHIFT];
	//P22  删除//	io.KeyAlt	= io.KeysDown[GLFW_KEY_LEFT_ALT]		|| io.KeysDown[GLFW_KEY_RIGHT_ALT];
	//P22  删除//	io.KeySuper = io.KeysDown[GLFW_KEY_LEFT_SUPER]		|| io.KeysDown[GLFW_KEY_RIGHT_SUPER];
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.KeysDown[e.GetKeyCode()] = false;
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	//P22  删除//{
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	int Keycode = e.GetKeyCode();
	//P22  删除//	if (Keycode > 0 && Keycode < 0x10000)
	//P22  删除//		io.AddInputCharacter((unsigned short)Keycode); //io.AddInputCharacter 用于向 ImGui 添加一个字符输入
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
	//P22  删除//
	//P22  删除//bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	//P22  删除//{ //让 ImGui 框不会消失在 Window 框里，并且至少保有 1.0f 的宽度
	//P22  删除//	ImGuiIO& io = ImGui::GetIO();
	//P22  删除//	io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
	//P22  删除//	io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	//P22  删除//	glViewport(0, 0, e.GetWidth(), e.GetHeight());
	//P22  删除//
	//P22  删除//	return false;
	//P22  删除//}
}