#include "Scene.h"
#include <map>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iterator>
#include "Log.h"

Scene::Scene(std::string name) : id_(0), name_(std::move(name)), textures_(256), materials_(256), meshes_(256),
                                 transforms_(256), instances_(256), cameras_(256)
{
}

uint32_t Scene::AddTexture(const ::Texture& texture)
{
	return textures_.insert(texture);
}

Texture& Scene::Texture(const uint32_t id)
{
	return textures_[id];
}

packed_freelist<Material> Scene::Materials() const
{
	return materials_;
}

::Material& Scene::Material(const uint32_t id)
{
	return materials_[id];
}

packed_freelist<Mesh> Scene::Meshes() const
{
	return meshes_;
}

Mesh& Scene::Mesh(const uint32_t id) const
{
	return meshes_[id];
}

packed_freelist<Transform> Scene::Transforms() const
{
	return transforms_;
}

Transform& Scene::Transform(const uint32_t id) const
{
	return transforms_[id];
}

packed_freelist<::Instance> Scene::Instances() const
{
	return instances_;
}

::Instance& Scene::Instance(const uint32_t id) const
{
	return instances_[id];
}

packed_freelist<Camera> Scene::Cameras() const
{
	return cameras_;
}

::Camera& Scene::Camera(const uint32_t id) const
{
	return cameras_[id];
}

uint32_t Scene::MainCameraId() const
{
	return mainCameraId_;
}

void Scene::SetMainCameraId(const uint32_t mainCameraId)
{
	mainCameraId_ = mainCameraId;
}

::Camera& Scene::MainCamera() const
{
	return Camera(MainCameraId());
}

uint32_t Scene::AddMesh(const std::string& filename, const std::string& materialDir, const std::string& textureDir,
                        const std::string& meshName)
{
	::Mesh newMesh;
	newMesh.SetName(meshName);

	Assimp::Importer importer;
	const auto assimpScene = importer.ReadFile(filename.c_str(),
	                                           aiProcess_CalcTangentSpace);

	if (!assimpScene)
	{
		std::cerr << "Failed to load model file [" << filename << "]." << std::endl;
		return UINT32_MAX;
	}

	std::vector<float> attributes = {};
	std::vector<uint32_t> indices = {};
	std::map<uint32_t, uint32_t> materials = {};

	for (uint32_t i = 0; i < assimpScene->mNumMaterials; ++i)
	{
		const auto material = assimpScene->mMaterials[i];
		::Material newMaterial;
		newMaterial.SetName(material->GetName().C_Str());

		aiColor3D color;
		material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		newMaterial.SetDiffuse(glm::make_vec3(&color[0]));
		material->Get(AI_MATKEY_COLOR_AMBIENT, color);
		newMaterial.SetAmbient(glm::make_vec3(&color[0]));
		material->Get(AI_MATKEY_COLOR_SPECULAR, color);
		newMaterial.SetSpecular(glm::make_vec3(&color[0]));
		float shininess;
		material->Get(AI_MATKEY_SHININESS, shininess);
		newMaterial.SetShininess(shininess);
		
		aiString path;
		aiTextureMapping textureMapping;
		unsigned uvIndex;
		ai_real blend;
		aiTextureOp textureOp;
		aiTextureMapMode textureMapMode;

		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 1)
		{
			std::cerr << "Model file [" << filename << "] material {index: " << i << ", name: " << newMaterial.Name() <<
				"} has more than 1 diffuse map. This is unsupported at this time." << std::endl;
		}

		if (material->GetTextureCount(aiTextureType_NORMALS) > 1)
		{
			std::cerr << "Model file [" << filename << "] material {index: " << i << ", name: " << newMaterial.Name() <<
				"} has more than 1 normal map. This is unsupported at this time." << std::endl;
		}

		if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path, &textureMapping, &uvIndex, &blend, &textureOp,
		                         &textureMapMode) == aiReturn_SUCCESS)
		{
			::Texture diffuseTexture(newMaterial.Name() + ".Diffuse");
			diffuseTexture.Load(materialDir + path.C_Str(), GL_RGB, GL_RGB);
			newMaterial.SetDiffuseTextureId(AddTexture(diffuseTexture));
		}
		else
		{
			std::cerr << "Model file [" << filename << "] material {index: " << i << ", name: " << newMaterial.Name() <<
				"} does not have a diffuse map." << std::endl;
			newMaterial.SetDiffuseTextureId(UINT32_MAX);
		}

		if (material->GetTexture(aiTextureType_NORMALS, 0, &path, &textureMapping, &uvIndex, &blend, &textureOp,
		                         &textureMapMode) == aiReturn_SUCCESS)
		{
			::Texture normalTexture(newMaterial.Name() + ".Normal");
			normalTexture.Load(materialDir + path.C_Str(), GL_RGB, GL_RGB);
			newMaterial.SetNormalTextureId(AddTexture(normalTexture));
		}
		else
		{
			std::cerr << "Model file [" << filename << "] material {index: " << i << ", name: " << newMaterial.Name() <<
				"} does not have a normal map." << std::endl;
			newMaterial.SetNormalTextureId(UINT32_MAX);
		}

		materials.insert({i, materials_.insert(newMaterial)});
	}

	glm::vec3 minVertex{};
	glm::vec3 maxVertex{};

	auto lastIndexCount = 0;

	for (uint32_t i = 0; i < assimpScene->mNumMeshes; ++i)
	{
		const auto mesh = assimpScene->mMeshes[i];

		for (uint32_t j = 0; j < mesh->mNumVertices; ++j)
		{
			minVertex = glm::min(minVertex, glm::make_vec3(&mesh->mVertices[j][0]));

			maxVertex = glm::max(maxVertex, glm::make_vec3(&mesh->mVertices[j][0]));

			if (mesh->GetNumUVChannels() > 1)
			{
				std::cerr << "Model file [" << filename <<
					"] has more than 1 UV channel. This is unsupported at this time." << std::endl;
			}
			attributes.insert(attributes.end(), {mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z});
			attributes.insert(attributes.end(), {mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y});
			attributes.insert(attributes.end(), {mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z});
			attributes.insert(attributes.end(), {mesh->mTangents[j].x, mesh->mTangents[j].y, mesh->mTangents[j].z});
			indices.push_back(indices.size());
		}

		DrawElementsIndirectCommand currDrawCommand;
		currDrawCommand.count = indices.size() - lastIndexCount;
		currDrawCommand.primCount = 1;
		currDrawCommand.firstIndex = lastIndexCount;
		currDrawCommand.baseVertex = 0;
		currDrawCommand.baseInstance = 0;

		Log::Debug("Material index [" + std::to_string(mesh->mMaterialIndex) + "] being used by mesh [" + mesh->mName.C_Str() + "] maps to material cache value [" + std::to_string(materials[mesh->mMaterialIndex]) + "].");
		
		newMesh.MaterialIDs().push_back(materials.at(mesh->mMaterialIndex));
		newMesh.DrawCommands().push_back(currDrawCommand);

		lastIndexCount = indices.size();
	}
	
	assert(!attributes.empty());
	assert(!indices.empty());

	std::cout << "Attributes: " << attributes.size() << std::endl;;
	std::cout << "Indices: " << indices.size() << std::endl;

	newMesh.SetAABB({ minVertex, maxVertex });
	
	glBindVertexArray(*newMesh.Vao());

	glBindBuffer(GL_ARRAY_BUFFER, *newMesh.AttributeVbo());
	glBufferData(GL_ARRAY_BUFFER,
	             attributes.size() * sizeof(float),
	             attributes.data(), GL_STATIC_DRAW);
	newMesh.SetNumVertices(indices.size());

	glVertexAttribPointer(SCENE_POSITION_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(float) * 11, nullptr);
	glEnableVertexAttribArray(SCENE_POSITION_ATTRIB_LOCATION);

	glVertexAttribPointer(SCENE_TEXCOORD_ATTRIB_LOCATION, 2, GL_FLOAT, GL_FALSE,
	                      sizeof(float) * 11, reinterpret_cast<void*>(sizeof(float) * 3));
	glEnableVertexAttribArray(SCENE_TEXCOORD_ATTRIB_LOCATION);

	glVertexAttribPointer(SCENE_NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE,
	                      sizeof(float) * 11, reinterpret_cast<void*>(sizeof(float) * 5));
	glEnableVertexAttribArray(SCENE_NORMAL_ATTRIB_LOCATION);

	glVertexAttribPointer(SCENE_TANGENT_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 11,
	                      reinterpret_cast<void*>(sizeof(float) * 8));
	glEnableVertexAttribArray(SCENE_TANGENT_ATTRIB_LOCATION);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *newMesh.IndexVbo());
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
	             indices.size() * sizeof(uint32_t),
	             indices.data(), GL_STATIC_DRAW);
	newMesh.SetNumIndices(indices.size());

	glBindVertexArray(0);


	float boundingBoxAttributes[] = {
		minVertex.x, minVertex.y, minVertex.z, maxVertex.x, minVertex.y, minVertex.z,
		maxVertex.x, minVertex.y, minVertex.z, maxVertex.x, maxVertex.y, minVertex.z,
		maxVertex.x, maxVertex.y, minVertex.z, minVertex.x, maxVertex.y, minVertex.z,
		minVertex.x, maxVertex.y, minVertex.z, minVertex.x, minVertex.y, minVertex.z,
		minVertex.x, minVertex.y, minVertex.z, minVertex.x, minVertex.y, maxVertex.z,
		minVertex.x, maxVertex.y, minVertex.z, minVertex.x, maxVertex.y, maxVertex.z,
		maxVertex.x, maxVertex.y, minVertex.z, maxVertex.x, maxVertex.y, maxVertex.z,
		maxVertex.x, minVertex.y, minVertex.z, maxVertex.x, minVertex.y, maxVertex.z,
		minVertex.x, minVertex.y, maxVertex.z, maxVertex.x, minVertex.y, maxVertex.z,
		maxVertex.x, minVertex.y, maxVertex.z, maxVertex.x, maxVertex.y, maxVertex.z,
		maxVertex.x, maxVertex.y, maxVertex.z, minVertex.x, maxVertex.y, maxVertex.z,
		minVertex.x, maxVertex.y, maxVertex.z, minVertex.x, minVertex.y, maxVertex.z
	};

	glBindVertexArray(*newMesh.BoundingBoxVao());

	glBindBuffer(GL_ARRAY_BUFFER, *newMesh.BoundingBoxAttributeVbo());
	glBufferData(GL_ARRAY_BUFFER,
	             sizeof(boundingBoxAttributes),
	             &boundingBoxAttributes[0], GL_STATIC_DRAW);

	glVertexAttribPointer(SCENE_POSITION_ATTRIB_LOCATION, 3, GL_FLOAT, GL_FALSE,
	                      0, nullptr);
	glEnableVertexAttribArray(SCENE_POSITION_ATTRIB_LOCATION);

	glBindVertexArray(0);

	::Texture iconTexture(newMesh.Name() + ".Icon");
	iconTexture.Load(textureDir + "icon.png", GL_SRGB8, GL_RGB, false);
	newMesh.SetIconTextureId(AddTexture(iconTexture));
	
	return meshes_.insert(newMesh);
}

uint32_t Scene::AddTransform(const ::Transform transform)
{
	return transforms_.insert(transform);
}

uint32_t Scene::AddInstance(const ::Instance instance)
{
	return instances_.insert(instance);
}

uint32_t Scene::AddCamera(const ::Camera camera, bool setAsMainCamera)
{
	auto cameraId = cameras_.insert(camera);

	if (setAsMainCamera)
	{
		SetMainCameraId(cameraId);
	}

	return cameraId;
}

bool Scene::PickInstance(glm::vec3 origin, glm::vec3 direction, uint32_t resultInstanceId)
{
	for (const auto& instanceId : instances_)
	{
		const auto& instance = instances_[instanceId];
		auto& mesh = meshes_[instance.MeshId()];
		const auto& transform = transforms_[instance.TransformId()];

		if (mesh.AABB().RayIntersect(origin, direction, transform.ModelMatrix()))
		{
			resultInstanceId = instanceId;
			return true;
		}
	}

	return false;
}
