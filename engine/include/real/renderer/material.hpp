// Copyright (c) 2020 udv. All rights reserved.

#ifndef REAL_ENGINE_MATERIAL
#define REAL_ENGINE_MATERIAL

#include "real/core.hpp"


namespace Real {
	class REAL_API Material {
	public:
		Material()
				:Material {
				32.0f,
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
		}
		{};

		Material(float shininess, const glm::vec3& ambient, const glm::vec3& diffuse,
				const glm::vec3& specular)
				:shininess { shininess }, ambient { ambient }, diffuse { diffuse },
				 specular { specular }
		{}

		Material(float shininess, glm::vec3&& ambient, glm::vec3&& diffuse,
		glm::vec3&& specular)
		:shininess { shininess }, ambient { std::move(ambient) },
		 diffuse { std::move(diffuse) },
		 specular { std::move(specular) }
		{}

		float Shininess() const
		{ return shininess; }
		float* ShininessPtr()
		{ return &shininess; }
		void Shininess(float shininess_)
		{ Material::shininess = shininess_; }

		const glm::vec3& Ambient() const
		{ return ambient; }
		float* AmbientPtr()
		{ return glm::value_ptr(ambient); }
		void Ambient(const glm::vec3& ambient_)
		{ Material::ambient = ambient_; }

		const glm::vec3& Diffuse() const
		{ return diffuse; }
		float* DiffusePtr()
		{ return glm::value_ptr(diffuse); }
		void Diffuse(const glm::vec3& diffuse_)
		{ Material::diffuse = diffuse_; }

		const glm::vec3& Specular() const
		{ return specular; }
		float* SpecularPtr()
		{ return glm::value_ptr(specular); }
		void Specular(const glm::vec3& specular_)
		{ Material::specular = specular_; }
	private:
		float shininess;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}

#endif //REAL_ENGINE_MATERIAL
