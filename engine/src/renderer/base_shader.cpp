// Copyright (c) 2020 udv. All rights reserved.

#include <glm/glm.hpp>
#include <real/renderer/base_shader.hpp>

#include "real/logger.hpp"
#include "real/renderer.hpp"

namespace real {
	shader *shader::make() {
		switch (renderer::api().enumval()) {
			case renderer_api::api::gl: return new gl_shader{};

			default:
			case renderer_api::api::none:
				REAL_CORE_ERROR("Invalid renderer api: {}", renderer_api::api::none);
				return nullptr;
		}
	}

	shader::~shader() = default;

	// Shader library
	void shader_lib::add(const reference<shader> &shader) {
		
	}

	real::reference<shader> shader_lib::load(const std::string &filepath) {
		return real::reference<shader>();
	}

	real::reference<shader> shader_lib::load(const std::string &filepath, const std::string &name) {
		return real::reference<shader>();
	}

	real::reference<shader> shader_lib::get(const std::string &name) {
		return real::reference<shader>();
	}
}
