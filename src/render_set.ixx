module;

#include <glm/glm.hpp>
#include <SDL3/SDL.h>
#include <vector>

export module render_set;

import sdl_data;
import vertex; 
import renderable;

export enum RenderGeometryType
{
	TRIANGLE_LIST
};

export class RenderSet {
public:
	SDL_GPUGraphicsPipeline* graphics_pipeline;
	SDL_GPUBuffer* vertex_buffer;
	SDL_GPUBuffer* index_buffer;
	SDL_GPUTransferBuffer* transfer_buffer;

	int vertex_buffer_size = 0;
	int index_buffer_size = 0;

	int current_renderable = 0;
	int current_index = 0;
	int current_vertex = 0;

	int total_indices = 0;
	int total_vertices = 0;
	static const int max_renderables = 5000;
	Renderable* renderables[max_renderables];
	glm::mat4x4* transform[max_renderables];

	int batch_offset[max_renderables];
	int batch_size[max_renderables];
	glm::mat4x4* batch_transform[max_renderables];
	int num_batches;

	glm::mat4x4 identity;

	RenderSet() {
		memset(renderables, 0, sizeof(renderables));
		memset(transform, 0, sizeof(transform));
		num_batches = 0;

		identity = glm::mat4(1.0f);
	}

	void init(RenderGeometryType geometry_type, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader,
		int vert_buffer_size, int ind_buffer_size) {

		vertex_buffer_size = vert_buffer_size;
		index_buffer_size = ind_buffer_size;

		SDL_GPUColorTargetDescription color_target_description{};
		color_target_description.format = SDL_GetGPUSwapchainTextureFormat(sdl_device, sdl_window);
		std::vector color_target_descriptions{ color_target_description };

		SDL_GPUGraphicsPipelineTargetInfo target_info{};
		target_info.color_target_descriptions = color_target_descriptions.data();
		target_info.num_color_targets = color_target_descriptions.size();

		SDL_GPUGraphicsPipelineCreateInfo pipeline_create_info{};
		pipeline_create_info.vertex_shader = vertex_shader;
		pipeline_create_info.fragment_shader = fragment_shader;
		pipeline_create_info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
		pipeline_create_info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
		pipeline_create_info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
		pipeline_create_info.target_info = target_info;

		SDL_GPUVertexBufferDescription vertex_input_description{};
		vertex_input_description.slot = 0;
		vertex_input_description.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
		vertex_input_description.instance_step_rate = 0;
		vertex_input_description.pitch = sizeof(Vertex);

		std::vector vertex_input_descriptions{ vertex_input_description };
		pipeline_create_info.vertex_input_state.vertex_buffer_descriptions = vertex_input_descriptions.data();
		pipeline_create_info.vertex_input_state.num_vertex_buffers = vertex_input_descriptions.size();

		SDL_GPUVertexAttribute vertex_attribute{};
		vertex_attribute.buffer_slot = 0;
		vertex_attribute.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		vertex_attribute.location = 0;
		vertex_attribute.offset = 0;

		SDL_GPUVertexAttribute vertex_attribute2{};
		vertex_attribute2.buffer_slot = 0;
		vertex_attribute2.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		vertex_attribute2.location = 1;
		vertex_attribute2.offset = sizeof(float) * 2;

		std::vector vertex_attributes{ vertex_attribute, vertex_attribute2 };
		pipeline_create_info.vertex_input_state.vertex_attributes = vertex_attributes.data();
		pipeline_create_info.vertex_input_state.num_vertex_attributes = vertex_attributes.size();

		graphics_pipeline = SDL_CreateGPUGraphicsPipeline(sdl_device, &pipeline_create_info);

		SDL_ReleaseGPUShader(sdl_device, vertex_shader);
		SDL_ReleaseGPUShader(sdl_device, fragment_shader);

		SDL_GPUBufferCreateInfo vertex_buffer_create_info{};
		vertex_buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		vertex_buffer_create_info.size = sizeof(Vertex) * vertex_buffer_size;
		vertex_buffer = SDL_CreateGPUBuffer(sdl_device, &vertex_buffer_create_info);

		SDL_GPUBufferCreateInfo index_buffer_create_info{};
		index_buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
		index_buffer_create_info.size = sizeof(uint16_t) * index_buffer_size;
		index_buffer = SDL_CreateGPUBuffer(sdl_device, &index_buffer_create_info);

		SDL_GPUTransferBufferCreateInfo transfer_buffer_create_info{};
		transfer_buffer_create_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		transfer_buffer_create_info.size = (sizeof(Vertex) * vertex_buffer_size) + (sizeof(Uint16) * index_buffer_size);

		transfer_buffer = SDL_CreateGPUTransferBuffer(sdl_device, &transfer_buffer_create_info);
	}

	void begin()
	{
		current_renderable = 0;
		current_index = 0;
		current_vertex = 0;

		total_indices = 0;
		total_vertices = 0;
	}

	void add(Renderable* renderable, glm::mat4x4* transf)
	{
		if ((current_renderable < max_renderables) &&
			(total_vertices + renderable->get_num_vertices() < vertex_buffer_size) &&
			(total_indices + renderable->get_num_indices() < index_buffer_size))
		{
			renderables[current_renderable] = renderable;
			transform[current_renderable] = transf;
			current_renderable++;
			total_vertices += renderable->vertices.size();
			total_indices += renderable->indices.size();
		}
		else
		{
			SDL_Log("RenderSet::add: Renderable not added, max renderables or buffer size exceeded");
		}
	}

	void end()
	{
		Vertex* transfer_data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(sdl_device, transfer_buffer, false));

		for (int i = 0; i < current_renderable; i++)
		{
			if (current_vertex + renderables[i]->vertices.size() < vertex_buffer_size)
			{
				Renderable* renderable = renderables[i];
				memcpy(&transfer_data[current_vertex], renderable->vertices.data(), sizeof(Vertex) * renderable->vertices.size());
				current_vertex += renderable->vertices.size();
			}

		}
		Uint16* indexData = (Uint16*)&transfer_data[current_vertex];
		int vertex_offset = 0;
		num_batches = 0;
		bool first_time = true;
		for (int i = 0; i < current_renderable; i++)
		{
			Renderable* renderable = renderables[i];
			if (first_time || (transform[i] != nullptr) || (i == current_renderable - 1))
			{
				if (!first_time)
				{
					batch_size[num_batches - 1] = current_index - batch_offset[num_batches - 1];
					batch_offset[num_batches] = current_index;
					batch_transform[num_batches] = transform[i];
					++num_batches;
				}
				else
				{
					first_time = false;
					batch_offset[num_batches] = 0;
					if (transform[i] != nullptr)
					{
						batch_transform[num_batches] = transform[i];
					}
					else
					{
						batch_transform[num_batches] = &identity;
					}
					++num_batches;
				}
			}

			for (int j = 0; j < renderable->get_num_indices(); j++)
			{
				indexData[current_index] = renderable->indices[j] + vertex_offset;
				current_index++;
			}
			vertex_offset += renderable->get_num_vertices();
		}

		SDL_UnmapGPUTransferBuffer(sdl_device, transfer_buffer);
		SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(sdl_cmdbuf);

		// upload the vertex data
		SDL_GPUTransferBufferLocation transfer_buffer_location_vertex{};
		transfer_buffer_location_vertex.transfer_buffer = transfer_buffer;
		transfer_buffer_location_vertex.offset = 0;

		SDL_GPUBufferRegion buffer_region{};
		buffer_region.buffer = vertex_buffer;
		buffer_region.offset = 0;
		buffer_region.size = sizeof(Vertex) * current_vertex;

		SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location_vertex,
			&buffer_region, false);

		// upload the index data
		SDL_GPUTransferBufferLocation transfer_buffer_location_index{};
		transfer_buffer_location_index.transfer_buffer = transfer_buffer;
		transfer_buffer_location_index.offset = sizeof(Vertex) * current_vertex;

		SDL_GPUBufferRegion buffer_region_index{};
		buffer_region_index.buffer = index_buffer;
		buffer_region_index.offset = 0;
		buffer_region_index.size = sizeof(Uint16) * current_index;

		SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location_index,
			&buffer_region_index, false);

		SDL_EndGPUCopyPass(copy_pass);
	}

	void render_all_geometries(SDL_GPURenderPass* render_pass, const glm::mat4x4& view_proj) const
	{
		SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
		SDL_GPUBufferBinding vertex_buffer_binding{};
		vertex_buffer_binding.buffer = vertex_buffer;
		vertex_buffer_binding.offset = 0;
		SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_buffer_binding, 1);

		SDL_GPUBufferBinding index_buffer_binding{};
		index_buffer_binding.buffer = index_buffer;
		index_buffer_binding.offset = 0;
		SDL_BindGPUIndexBuffer(render_pass, &index_buffer_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

		glm::mat4x4 view_proj_model;

		for (int i = 0; i < num_batches; i++)
		{
			if (batch_size[i] > 0)
			{
				view_proj_model = view_proj * (*batch_transform[i]);
				SDL_PushGPUVertexUniformData(sdl_cmdbuf, 0, &view_proj_model, sizeof(glm::mat4x4));
				SDL_DrawGPUIndexedPrimitives(render_pass, batch_size[i], 1, batch_offset[i], 0, 0);
			}
		}
		//SDL_PushGPUVertexUniformData(sdl_cmdbuf, 0, &view_proj, sizeof(view_proj));
		//SDL_DrawGPUIndexedPrimitives(render_pass, current_index, 1, 0, 0, 0);
	}

	void cleanup() const
	{
		SDL_ReleaseGPUTransferBuffer(sdl_device, transfer_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, vertex_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, index_buffer);
		SDL_ReleaseGPUGraphicsPipeline(sdl_device, graphics_pipeline);
	}
};




