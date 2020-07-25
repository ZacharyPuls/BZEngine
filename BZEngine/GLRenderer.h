#pragma once

#include "Headers.h"

#include "IRenderer.h"
#include "Shader.h"
#include "RenderToTexture.h"
#include "picking.glsl"
#include "Log.h"

class GLRenderer :
	protected IRenderer
{
public:
	explicit GLRenderer(std::shared_ptr<Scene> scene)
		: scene_(scene)
	{
		defaultShader_.AddVertexShader("assets/shaders/default/shader.vert", "#version 460", "assets/shaders/default/Preamble.glsl");
		defaultShader_.AddFragmentShader("assets/shaders/default/shader.frag", "#version 460", "assets/shaders/default/Preamble.glsl");
		defaultShader_.CompileAndLink();

		wireframeShader_.AddVertexShader("assets/shaders/wireframe/shader.vert", "#version 460", "assets/shaders/wireframe/Preamble.glsl");
		wireframeShader_.AddFragmentShader("assets/shaders/wireframe/shader.frag", "#version 460", "assets/shaders/wireframe/Preamble.glsl");
		wireframeShader_.CompileAndLink();

		pickingShader_.AddVertexShader("assets/shaders/picking/shader.vert", "#version 460", "assets/shaders/picking/picking.glsl");
		pickingShader_.AddFragmentShader("assets/shaders/picking/shader.frag", "#version 460", "assets/shaders/picking/picking.glsl");
		pickingShader_.CompileAndLink();

		pickingFramebuffer_.Bind();

		pickingRenderTexture_.Bind();
		pickingRenderTexture_.Create(0, GL_R32UI, 1024, 768, GL_RED_INTEGER, GL_UNSIGNED_INT, nullptr);
		pickingRenderTexture_.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		pickingRenderTexture_.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		pickingRenderTexture_.UnBind();
		
		pickingFramebuffer_.AttachTexture(GL_COLOR_ATTACHMENT0, pickingRenderTexture_);
		
		pickingDepthBuffer_.Bind();
		pickingDepthBuffer_.Storage(GL_DEPTH_COMPONENT, 1024, 768);
		pickingDepthBuffer_.Bind();
		
		pickingFramebuffer_.AttachRenderbuffer(GL_DEPTH_ATTACHMENT, pickingDepthBuffer_);
		
		if (!pickingFramebuffer_.IsComplete())
		{
			Log::Debug("glCheckFramebufferStatus(GL_FRAMEBUFFER) didn't return GL_FRAMEBUFFER_COMPLETE.");
		}

		pickingFramebuffer_.UnBind();
	}

	glm::ivec2 Viewport() const
	{
		return { viewportWidth_, viewportHeight_ };
	}
	
	void SetViewport(const int width, const int height)
	{
		viewportWidth_ = width;
		viewportHeight_ = height;
	}

	void RenderFrame()
	{
		glViewport(0, 0, viewportWidth_, viewportHeight_);
		glClearColor(100.0f / 255.0f, 149.0f / 255.0f, 237.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// glEnable(GL_FRAMEBUFFER_SRGB);

		auto& mainCamera = scene_->MainCamera();

		const auto& V = mainCamera.View();
		const auto& P = mainCamera.Projection();

		const glm::mat4 VP = P * V;

		defaultShader_.Bind();

		const auto& instances = scene_->Instances();

		for (uint32_t instanceId : instances)
		{
			const auto instance = scene_->Instance(instanceId);
			auto mesh = scene_->Mesh(instance.MeshId());
			const auto transform = scene_->Transform(instance.TransformId());

			// std::cout << "Rendering instance " << instanceId << " of mesh with " << mesh.NumIndices() << " indices." << std::endl;
			glm::mat4 MW = glm::mat4(1.0f);
			MW = glm::translate(-transform.RotationOrigin()) * MW;
			MW = glm::mat4_cast(transform.Rotation()) * MW;
			MW = glm::translate(transform.RotationOrigin()) * MW;
			MW = glm::scale(transform.Scale()) * MW;
			MW = glm::translate(transform.Translation()) * MW;

			glm::mat3 N_MW = glm::mat4(1.0f);
			N_MW = glm::mat3_cast(transform.Rotation()) * N_MW;
			N_MW = glm::mat3(glm::scale(1.0f / transform.Scale())) * N_MW;

			glm::mat4 MVP = VP * MW;

			glUniformMatrix4fv(SCENE_MW_UNIFORM_LOCATION, 1, GL_FALSE, glm::value_ptr(MW));
			glUniformMatrix3fv(SCENE_N_MW_UNIFORM_LOCATION, 1, GL_FALSE, glm::value_ptr(N_MW));
			glUniformMatrix4fv(SCENE_MVP_UNIFORM_LOCATION, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniform3fv(SCENE_CAMERAPOS_UNIFORM_LOCATION, 1, glm::value_ptr(mainCamera.Eye()));
			glUniform3f(SCENE_LIGHTPOS_UNIFORM_LOCATION, 0.25f, 1.0f, 0.25f);

			glBindVertexArray(*mesh.Vao());
			const auto& drawCommands = mesh.DrawCommands();
			const auto& materialIDs = mesh.MaterialIDs();
			for (size_t drawCommandIndex = 0; drawCommandIndex < drawCommands.size(); ++drawCommandIndex)
			{
				const auto& drawCommand = mesh.DrawCommand(drawCommandIndex);
				const auto& material = scene_->Material(materialIDs[drawCommandIndex]);
				// Log::Debug(
				// 	"Drawing command index [" + std::to_string(drawCommandIndex) + "] with material [" + material.Name()
				// 	+ "], which binds texture [" + scene_->Texture(material.DiffuseTextureId()).Name() + "].");

				glActiveTexture(GL_TEXTURE0 + SCENE_DIFFUSE_MAP_TEXTURE_BINDING);
				if (material.DiffuseTextureId() == UINT32_MAX)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
					glUniform1i(SCENE_HAS_DIFFUSE_MAP_UNIFORM_LOCATION, 0);
				}
				else
				{
					auto& diffuseTexture = scene_->Texture(material.DiffuseTextureId());
					diffuseTexture.Bind();
					glUniform1i(SCENE_HAS_DIFFUSE_MAP_UNIFORM_LOCATION, 1);
				}

				glActiveTexture(GL_TEXTURE0 + SCENE_NORMAL_MAP_TEXTURE_BINDING);
				if (material.NormalTextureId() == UINT32_MAX)
				{
					glBindTexture(GL_TEXTURE_2D, 0);
					glUniform1i(SCENE_HAS_NORMAL_MAP_UNIFORM_LOCATION, 0);
				}
				else
				{
					auto& normalTexture = scene_->Texture(material.NormalTextureId());
					normalTexture.Bind();
					glUniform1i(SCENE_HAS_NORMAL_MAP_UNIFORM_LOCATION, 1);
				}

				glUniform3fv(SCENE_AMBIENT_UNIFORM_LOCATION, 1, &material.Ambient()[0]);
				glUniform3fv(SCENE_DIFFUSE_UNIFORM_LOCATION, 1, &material.Diffuse()[0]);
				glUniform3fv(SCENE_SPECULAR_UNIFORM_LOCATION, 1, &material.Specular()[0]);
				glUniform1f(SCENE_SHININESS_UNIFORM_LOCATION, material.Shininess());

				glDrawElementsInstancedBaseVertexBaseInstance(
					GL_TRIANGLES,
					drawCommand.count,
					GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(sizeof(uint32_t) * drawCommand.firstIndex),
					drawCommand.primCount,
					drawCommand.baseVertex,
					drawCommand.baseInstance);
			}

			if (instance.RenderBoundingBox()) {
				wireframeShader_.Bind();
				glBindVertexArray(*mesh.BoundingBoxVao());

				glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(MVP));
				glUniform4f(1, 1.0f, 0.0f, 0.0f, 1.0f);

				glLineWidth(3.0f);
				glDrawArrays(GL_LINES, 0, 72);

				glBindVertexArray(0);
				defaultShader_.Bind();
			}
		}
	}

	bool PickInstance(float x, float y, uint32_t& instanceId)
	{
		auto& mainCamera = scene_->MainCamera();

		const auto& V = mainCamera.View();
		const auto& P = mainCamera.Projection();

		const glm::mat4 VP = P * V;

		pickingShader_.Bind();
		pickingFramebuffer_.Bind();
		
		glViewport(0, 0, pickingRenderTexture_.Width(), pickingRenderTexture_.Height());
		GLuint clearValue[] = { UINT32_MAX };
		glClearBufferuiv(GL_COLOR, 0, clearValue);
		glClear(GL_DEPTH_BUFFER_BIT);
		
		const auto& instances = scene_->Instances();

		for (uint32_t instanceId : instances)
		{
			const auto instance = scene_->Instance(instanceId);
			auto mesh = scene_->Mesh(instance.MeshId());
			const auto transform = scene_->Transform(instance.TransformId());

			glm::mat4 MW = glm::mat4(1.0f);
			MW = glm::translate(-transform.RotationOrigin()) * MW;
			MW = glm::mat4_cast(transform.Rotation()) * MW;
			MW = glm::translate(transform.RotationOrigin()) * MW;
			MW = glm::scale(transform.Scale()) * MW;
			MW = glm::translate(transform.Translation()) * MW;

			glm::mat4 MVP = VP * MW;

			glUniformMatrix4fv(PICKING_MVP_UNIFORM_LOCATION, 1, GL_FALSE, glm::value_ptr(MVP));
			glUniform1ui(PICKING_INSTANCEID_UNIFORM_LOCATION, instanceId);

			glBindVertexArray(*mesh.Vao());
			const auto& drawCommands = mesh.DrawCommands();
			for (size_t drawCommandIndex = 0; drawCommandIndex < drawCommands.size(); ++drawCommandIndex)
			{
				const auto& drawCommand = mesh.DrawCommand(drawCommandIndex);

				glDrawElementsInstancedBaseVertexBaseInstance(
					GL_TRIANGLES,
					drawCommand.count,
					GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(sizeof(uint32_t) * drawCommand.firstIndex),
					drawCommand.primCount,
					drawCommand.baseVertex,
					drawCommand.baseInstance);
			}
		}

		auto xW = static_cast<int>((x + 1.0f) * (static_cast<float>(1024) / 2.0f));
		auto yW = static_cast<int>((y + 1.0f) * (static_cast<float>(768) / 2.0f));
		instanceId = UINT32_MAX;
		glReadPixels(xW, yW, 1, 1, GL_RED_INTEGER, GL_UNSIGNED_INT, &instanceId);

		pickingFramebuffer_.UnBind();

		pickingShader_.UnBind();

		return instanceId != UINT32_MAX;
	}
	
private:
	Shader defaultShader_;
	Shader wireframeShader_;
	Shader pickingShader_;
	Framebuffer pickingFramebuffer_;
	Texture pickingRenderTexture_;
	Renderbuffer pickingDepthBuffer_;
	// RenderToTexture pickingRenderTarget_;
	std::shared_ptr<Scene> scene_;
};
