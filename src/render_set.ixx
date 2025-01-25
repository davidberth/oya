module;

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

	Renderable* renderables[1000];

	RenderSet() {
		memset(renderables, 0, sizeof(renderables));
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
	}

	void render(Renderable* renderable)
	{
		renderables[current_renderable] = renderable;
		current_renderable++;
	}

	void end()
	{
		Vertex* transfer_data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(sdl_device, transfer_buffer, false));

		// TODO This will be optimized by only copying new data when geometry changes
		for (int i = 0; i < current_renderable; i++)
		{
			Renderable* renderable = renderables[i];
			for (int j = 0; j < renderable->get_num_vertices(); j++)
			{
				transfer_data[current_vertex] = renderable->vertices[j];
				current_vertex++;
			}
		}
		Uint16* indexData = (Uint16*)&transfer_data[current_vertex];
		for (int i = 0; i < current_renderable; i++)
		{
			Renderable* renderable = renderables[i];
			for (int j = 0; j < renderable->get_num_indices(); j++)
			{
				indexData[current_index] = renderable->indices[j];
				current_index++;
			}
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

	void render_all_geometries(SDL_GPURenderPass* render_pass) const
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

		SDL_DrawGPUIndexedPrimitives(render_pass, current_index, 1, 0, 0, 0);

	}

	void cleanup() const
	{
		SDL_ReleaseGPUTransferBuffer(sdl_device, transfer_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, vertex_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, index_buffer);
		SDL_ReleaseGPUGraphicsPipeline(sdl_device, graphics_pipeline);
	}
};




