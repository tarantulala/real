// Copyright (c) 2020 udv. All rights reserved.

#include <real/real.hpp>
#include <imgui.h> // TODO: abstract imgui
#include <glm/gtc/type_ptr.hpp>

class SandboxLayer : public Real::Layer
{
private:
	// @formatter:off
	Real::Light light;
	Real::Material material;

	// Global
	float cameraYOffset  = 2.0f;
	float lightYOffset   = 2.0f;
	float lightAngle     = 0.0f;
	float lightDistance  = 5.0f;
	float cameraDistance = 5.0f;
	float rotationSpeed  = 0.5f;
	// @formatter:on

	glm::mat4 transformMat = glm::identity<glm::mat4>();

	// Rendering
	Real::Reference<Real::VertexArray> vao;
	Real::Camera* camera;
	Real::ShaderLibrary shaderLib;
public:
	SandboxLayer()
			:Real::Layer()
	{
		// Perspective
		camera = new Real::PerspectiveCamera { 45.0f, 16.0f, 9.0f };
		light = Real::Light {
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
		};
		material = Real::Material {
				32.0f,
				{ 1.0f, 0.5f, 0.31f },
				{ 1.0f, 0.5f, 0.31f },
				{ 0.5f, 0.5f, 0.5f },
		};
	}

	virtual void OnImGUIRender() override
	{
		ImGui::Begin("Settings");

		ImGui::Text("Global");
		ImGui::SliderFloat("Rotation speed", &rotationSpeed, 0.1f, 1.0f, "%.1f");
		ImGui::SliderFloat("Camera Distance", &cameraDistance, 2.0f, 7.0f, "%.1f");
		ImGui::SliderFloat("Camera Y", &cameraYOffset, 2.0f, 7.0f, "%.1f");

		ImGui::Text("Light");
		ImGui::SliderFloat("Light Angle", &lightAngle, 0.0f, 360.0f, "%.1f");
		ImGui::SliderFloat("Light Y", &lightYOffset, 2.0f, 5.0f, "%.1f");
		ImGui::ColorEdit3("Light Ambient", light.AmbientPtr());
		ImGui::ColorEdit3("Light Diffuse", light.DiffusePtr());
		ImGui::ColorEdit3("Light Specular", light.SpecularPtr());

		ImGui::Text("Material");
		ImGui::SliderFloat("Material Shininess", material.ShininessPtr(), 32.0f, 256.0f,
				"%.0f");
		ImGui::ColorEdit3("Material Ambient", material.AmbientPtr());
		ImGui::ColorEdit3("Material Diffuse", material.DiffusePtr());
		ImGui::ColorEdit3("Material Specular", material.SpecularPtr());

		ImGui::End();
	}

	virtual void Attach() override
	{
		auto shader = shaderLib.Load("shaders/material.glsl");

		// region Setup rendering
		// Vertices
		// @formatter:off
		float vertices[] = {
				// TOP
				// Positions           // Normals             // Colors
				 1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,   // 1.0f, 0.0f, 0.0f, 1.0f, // FTR
				-1.0f,  1.0f,  1.0f,    0.0f,  1.0f,  0.0f,   // 1.0f, 0.0f, 0.0f, 1.0f, // FTL
				 1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,   // 1.0f, 0.0f, 0.0f, 1.0f, // RTR
				-1.0f,  1.0f, -1.0f,    0.0f,  1.0f,  0.0f,   // 1.0f, 0.0f, 0.0f, 1.0f, // RTL

				// BOTTOM
				// Positions           // Normals             // Colors
				 1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,   // 0.0f, 1.0f, 0.0f, 1.0f, // FBR
				-1.0f, -1.0f,  1.0f,    0.0f, -1.0f,  0.0f,   // 0.0f, 1.0f, 0.0f, 1.0f, // FBL
				 1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,   // 0.0f, 1.0f, 0.0f, 1.0f, // RBR
				-1.0f, -1.0f, -1.0f,    0.0f, -1.0f,  0.0f,   // 0.0f, 1.0f, 0.0f, 1.0f, // RBL

				// FRONT
				// Positions           // Normals             // Colors
				 1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,   // 0.0f, 0.0f, 1.0f, 1.0f, // FTR
				-1.0f,  1.0f,  1.0f,    0.0f,  0.0f,  1.0f,   // 0.0f, 0.0f, 1.0f, 1.0f, // FTL
				 1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,   // 0.0f, 0.0f, 1.0f, 1.0f, // FBR
				-1.0f, -1.0f,  1.0f,    0.0f,  0.0f,  1.0f,   // 0.0f, 0.0f, 1.0f, 1.0f, // FBL

				// BACK
				// Positions           // Normals             // Colors
				 1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,   // 0.5f, 0.5f, 0.0f, 1.0f, // RTR
				-1.0f,  1.0f, -1.0f,    0.0f,  0.0f, -1.0f,   // 0.5f, 0.5f, 0.0f, 1.0f, // RTL
				 1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,   // 0.5f, 0.5f, 0.0f, 1.0f, // RBR
				-1.0f, -1.0f, -1.0f,    0.0f,  0.0f, -1.0f,   // 0.5f, 0.5f, 0.0f, 1.0f, // RBL

				// LEFT
				// Positions           // Normals             // Colors
				-1.0f,  1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,   // 0.0f, 0.5f, 0.5f, 1.0f, // FTL
				-1.0f, -1.0f,  1.0f,   -1.0f,  0.0f,  0.0f,   // 0.0f, 0.5f, 0.5f, 1.0f, // FBL
				-1.0f,  1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,   // 0.0f, 0.5f, 0.5f, 1.0f, // RTL
				-1.0f, -1.0f, -1.0f,   -1.0f,  0.0f,  0.0f,   // 0.0f, 0.5f, 0.5f, 1.0f, // RBL

				// RIGHT
				// Positions           // Normals             // Colors
			 	 1.0f,  1.0f,  1.0f,    1.0f,  0.0f,  0.0f,   // 0.5f, 0.0f, 0.5f, 1.0f, // FTR
			 	 1.0f, -1.0f,  1.0f,    1.0f,  0.0f,  0.0f,   // 0.5f, 0.0f, 0.5f, 1.0f, // FBR
			 	 1.0f,  1.0f, -1.0f,    1.0f,  0.0f,  0.0f,   // 0.5f, 0.0f, 0.5f, 1.0f, // RTR
			 	 1.0f, -1.0f, -1.0f,    1.0f,  0.0f,  0.0f,   // 0.5f, 0.0f, 0.5f, 1.0f, // RBR
		};

		unsigned int positions[] {
				0,  1,  2,    1,  2,  3,   // TOP
				4,  5,  6,    5,  6,  7,   // BOTTOM
				8,  9, 10,    9, 10, 11,   // FRONT
				12, 13, 14,   13, 14, 15,   // BACK
				16, 17, 18,   17, 18, 19,   // LEFT
				20, 21, 22,   21, 22, 23,   // RIGHT
		};
		// @formatter:on


		// Vertex Array
		vao = Real::VertexArray::Make();

		// Vertex Buffer
		Real::Reference<Real::VertexBuffer> vbo = Real::VertexBuffer::Make(vertices,
				sizeof(vertices));
		vbo->Layout({
				{ Real::shader_data_t::vec3, "_pos", },
				{ Real::shader_data_t::vec3, "_normal", },
		});

		// Index Buffer
		Real::Reference<Real::IndexBuffer> ibo = Real::IndexBuffer::Make(positions,
				sizeof(positions) / sizeof(unsigned int));

		// Link buffers to vertex array
		vao->AddVertexBuffer(vbo);
		vao->AddIndexBuffer(ibo);
		// endregion
	}

	virtual void Update(Real::Timestep ts) override
	{
		// Light
		float lightAngleRads = glm::radians(lightAngle);
		float lx = glm::sin(lightAngleRads) * lightDistance;
		float lz = glm::cos(lightAngleRads) * lightDistance;
		light.Pos({ lx, lightYOffset, lz, });

		// Camera
		camera->Position({ 0.0f, cameraYOffset, cameraDistance });
		camera->LookAt({ 0.0, 0.0, 0.0 });

		// Cube
		float deltaAngle =
				ts.milliseconds() * 0.1f * rotationSpeed; // TODO: float operators
		float deltaAngleRads = glm::radians(deltaAngle);
		transformMat = glm::rotate(transformMat, deltaAngleRads,
				glm::vec3(0.0f, 1.0f, 0.0f));
		Real::Transform transform = transformMat;

		const auto& shader = shaderLib.Get("material");
		shader->Bind();
		// Material
		shader->UniformFloat("u_Material.ambient", material.Ambient());
		shader->UniformFloat("u_Material.diffuse", material.Diffuse());
		shader->UniformFloat("u_Material.specular", material.Specular());
		shader->UniformFloat("u_Material.shininess", material.Shininess());
		// Light
		shader->UniformFloat("u_Light.position", light.Pos());
		shader->UniformFloat("u_Light.ambient", light.Ambient());
		shader->UniformFloat("u_Light.diffuse", light.Diffuse());
		shader->UniformFloat("u_Light.specular", light.Specular());
		// Lighting

		// Scene
		Real::Renderer::StartScene(*camera);

		Real::Renderer::Submit(vao, shader, transform);
		Real::Renderer::EndScene();

		Real::RenderCommand::DrawIndexed(vao);
	}
};

class Application : public Real::Application
{
public:
	Application()
			:Real::Application()
	{
		PushLayer(new SandboxLayer());
	}
};

Real::Scope<Real::Application> Real::Make()
{
	return Real::MakeScope<::Application>();
}