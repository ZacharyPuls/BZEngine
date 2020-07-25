#pragma once

#include "Headers.h"

#include <vector>
#include <memory>
#include "Camera.h"
#include "Material.h"
#include "Mesh.h"
#include "Transform.h"
#include "Texture.h"
#include <atomic>
#include "packed_freelist.h"
#include "Instance.h"
#include <array>
#include "Preamble.glsl"

class Scene
{
public:
	Scene(std::string name = "");

	uint32_t AddTexture(const Texture& texture);
	Texture& Texture(uint32_t id);

	packed_freelist<Material> Materials() const;
	::Material& Material(uint32_t id);

	packed_freelist<Mesh> Meshes() const;
	Mesh& Mesh(uint32_t id) const;

	packed_freelist<Transform> Transforms() const;
	Transform& Transform(uint32_t id) const;

	packed_freelist<::Instance> Instances() const;
	::Instance& Instance(uint32_t id) const;

	packed_freelist<Camera> Cameras() const;
	::Camera& Camera(uint32_t id) const;

	uint32_t MainCameraId() const;
	void SetMainCameraId(uint32_t mainCameraId);
	::Camera& MainCamera() const;

	uint32_t AddMesh(const std::string& filename, const std::string& materialDir, const std::string& textureDir, const std::string& meshName = "");

	uint32_t AddTransform(::Transform transform);

	uint32_t AddInstance(::Instance instance);

	uint32_t AddCamera(::Camera camera, bool setAsMainCamera = false);

	bool PickInstance(glm::vec3 origin, glm::vec3 direction, uint32_t resultInstanceId);

private:
	uint32_t id_;
	std::string name_;
	
	packed_freelist<::Texture> textures_;
	packed_freelist<::Material> materials_;
	packed_freelist<::Mesh> meshes_;
	packed_freelist<::Transform> transforms_;
	packed_freelist<::Instance> instances_;
	packed_freelist<::Camera> cameras_;

	uint32_t mainCameraId_;

	// bool intersectMesh_(glm::vec3 origin, glm::vec3 direction, ::Camera& camera, ::Instance& mesh)
	
	// static std::atomic<uint32_t> nextId_;
};

