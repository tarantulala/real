// Copyright (c) 2020 udv. All rights reserved.

#ifndef REAL_ENGINE_LIGHT
#define REAL_ENGINE_LIGHT

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "real/core.hpp"

namespace Real
{
	class REAL_API Light
	{
	public:
		Light()
				:Light {
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
				{ 1.0f, 1.0f, 1.0f, },
		}
		{};

		Light(const glm::vec3& pos, const glm::vec3& ambient, const glm::vec3& diffuse,
				const glm::vec3& specular)
				:pos { pos }, ambient { ambient }, diffuse { diffuse },
				 specular { specular }
		{}

		Light(glm::vec3&& pos, glm::vec3&& ambient, glm::vec3&& diffuse,
				glm::vec3&& specular)
				:pos { std::move(pos) }, ambient { std::move(ambient) },
				 diffuse { std::move(diffuse) },
				 specular { std::move(specular) }
		{}

		const glm::vec3& Pos() const
		{ return pos; }
		float* PosPtr()
		{ return glm::value_ptr(pos); }
		void Pos(const glm::vec3& pos_)
		{ Light::pos = pos_; }

		const glm::vec3& Ambient() const
		{ return ambient; }
		float* AmbientPtr()
		{ return glm::value_ptr(ambient); }
		void Ambient(const glm::vec3& ambient_)
		{ Light::ambient = ambient_; }

		const glm::vec3& Diffuse() const
		{ return diffuse; }
		float* DiffusePtr()
		{ return glm::value_ptr(diffuse); }
		void Diffuse(const glm::vec3& diffuse_)
		{ Light::diffuse = diffuse_; }

		const glm::vec3& Specular() const
		{ return specular; }
		float* SpecularPtr()
		{ return glm::value_ptr(specular); }
		void Specular(const glm::vec3& specular_)
		{ Light::specular = specular_; }
	private:
		glm::vec3 pos;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}

#endif //REAL_ENGINE_LIGHT
