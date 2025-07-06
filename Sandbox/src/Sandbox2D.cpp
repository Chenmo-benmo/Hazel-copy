#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{
}

void Sandbox2D::OnAttach()
{
	//ÔÚ Renderer2D::Init() ÖÐÊµÏÖ
	//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
	//P51	É¾³ý//m_SquareVA = Hazel::VertexArray::Create();
	//P51	É¾³ý//
	//P51	É¾³ý//float squareVertices[5 * 4] = {
	//P51	É¾³ý//	-0.5f, -0.5f, 0.0f,
	//P51	É¾³ý//	 0.5f, -0.5f, 0.0f,
	//P51	É¾³ý//	 0.5f,  0.5f, 0.0f,
	//P51	É¾³ý//	-0.5f,  0.5f, 0.0f
	//P51	É¾³ý//};
	//P51	É¾³ý//
	//P51	É¾³ý//Hazel::Ref<Hazel::VertexBuffer> squareVB;
	//P51	É¾³ý//squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
	//P51	É¾³ý//squareVB->SetLayout({
	//P51	É¾³ý//	{ Hazel::ShaderDataType::Float3, "a_Position" }
	//P51	É¾³ý//	});
	//P51	É¾³ý//m_SquareVA->AddVertexBuffer(squareVB);
	//P51	É¾³ý//
	//P51	É¾³ý//uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
	//P51	É¾³ý//Hazel::Ref<Hazel::IndexBuffer> squareIB;
	//P51	É¾³ý//squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
	//P51	É¾³ý//m_SquareVA->SetIndexBuffer(squareIB);
	//P51	É¾³ý//
	//P51	É¾³ý//m_FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{
}

void Sandbox2D::OnUpdate(Hazel::Timestep ts)
{
	// Update
	m_CameraController.OnUpdate(ts);

	// Render
	Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Hazel::RenderCommand::Clear();

	Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });
	Hazel::Renderer2D::DrawQuad({ 1.0f, 0.2f }, { 0.5f, 1.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });
	Hazel::Renderer2D::EndScene();

	//P51	É¾³ý//Hazel::Renderer::BeginScene(m_CameraController.GetCamera());
	//P51	É¾³ý//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->Bind();
	//P51	É¾³ý//std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat4("u_Color", m_SquareColor);
	//P51	É¾³ý//Hazel::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
	//P51	É¾³ý//Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));
	ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
	m_CameraController.OnEvent(e);
}