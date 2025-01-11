module;

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

export module geometry_renderer;

import renderable;
import vertex;
import render_stats_event;
import event;
import shader;

export enum RenderType
{
	polygon = GL_TRIANGLES,
	line = GL_LINES
};


export class GeometryRenderer
{
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	int vertex_buffer_size = 40000;
	int index_buffer_size = 40000;
	int current_offset = 0;
	int index_offset = 0;

	int num_draw_calls = 0;
	int num_total_indices = 0;

	GLuint VBO = -1;
	GLuint VAO = -1;
	GLuint EBO = -1;

	GLuint view_proj_model;

	bool dynamic = false;
	RenderType render_type;

	Shader* shader;
	RenderStatsEventSet render_set;

	int vertices_per_element;

public:
	GeometryRenderer(bool dyn, RenderType render_typ, RenderStatsEventSet rend_set) 
	{ 
		dynamic = dyn; 
		render_type = render_typ;
		render_set = rend_set;
		shader = nullptr;

		if (render_type == RenderType::polygon)
		{
			vertices_per_element = 3;
		}
		else if (render_type == RenderType::line)
		{
			vertices_per_element = 2;
		}
	};
	~GeometryRenderer() {
		if (shader != nullptr)
		{
			delete shader;
		}
	};

	void clear_buffer()
	{
		vertices.clear();
		indices.clear();
		current_offset = 0;
		index_offset = 0;
	}

	void update_renderable_vertices(Renderable* renderable)
	{
        auto buffer_offset = renderable->get_vertex_offset();
        auto num_vertices = renderable->get_num_vertices();
        auto& renderable_vertices = renderable->vertices;

        for (int i = 0; i < num_vertices; i++)
        {
            vertices.at(buffer_offset + i) = renderable_vertices.at(i);
        }
	}

	void add_renderable(Renderable *renderable)
	{
		renderable->set_buffer_offset(current_offset);
		renderable->set_index_offset(index_offset);

		for (int i = 0; i < renderable->get_num_vertices(); i++)
		{
			vertices.push_back(renderable->vertices.at(i));
		}
		for (int i = 0; i < renderable->get_num_indices(); i++)
		{
			indices.push_back(renderable->indices.at(i) + current_offset);
		}
		current_offset += renderable->get_num_vertices();
		index_offset += renderable->get_num_indices();
	}

	int get_num_vertices()
	{
		return vertices.size();
	}

	int get_num_indices()
	{
		return indices.size();
	}

	void setup_vbo()
	{

		GLuint draw_type = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

		if (VAO >= 0)
		{
			glDeleteVertexArrays(1, &VAO);
		}
		if (VBO >= 0)
		{
			glDeleteBuffers(1, &VBO);
		}
		if (EBO >= 0)
		{
			glDeleteBuffers(1, &EBO);
		}

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), draw_type);
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), draw_type);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, red));
		glEnableVertexAttribArray(1);
		glBindVertexArray(0);
	}

	void assign_shader(const std::string& vertex_path, const std::string& fragment_path)
	{
		shader = new Shader(vertex_path, fragment_path);
		view_proj_model = glGetUniformLocation(shader->program_id, "view_proj_model");
		set_transformation(glm::mat4(1.0f));
	}
	
	void update_vbo()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(unsigned int), indices.data());
	}

	void set_transformation(const glm::mat4& transform) const
	{
		glUniformMatrix4fv(view_proj_model, 1, GL_FALSE, glm::value_ptr(transform));
	}

	void new_frame()
	{
		glBindVertexArray(VAO);
		num_draw_calls = 0;
		num_total_indices = 0;
		if (shader != nullptr)
		{
			shader->use();
		}	
	}

	void render(int start_index, int num_indices)
	{
		if (num_indices > 0 and shader != nullptr)
		{
			glDrawElements(render_type, num_indices, GL_UNSIGNED_INT, (void*)(start_index * sizeof(unsigned int)));

			num_draw_calls += 1;
			num_total_indices += num_indices;
		}
	}

	void end_frame() const
	{
		glBindVertexArray(0);
		if (num_draw_calls == 0)
		{
			get_event_dispatcher().dispatch(RenderStatsEvent(render_set, 0, 0.0f, 0.0f));
			return;
		}
		float average_indices = num_total_indices / num_draw_calls;
		float average_triangles = average_indices / vertices_per_element;
		
		get_event_dispatcher().dispatch(RenderStatsEvent(render_set, num_draw_calls, average_indices, average_triangles));
	}
};