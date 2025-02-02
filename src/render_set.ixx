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



export template <typename VertexType>
class RenderSet {
public:
	SDL_GPUGraphicsPipeline* graphics_pipeline = nullptr;
	SDL_GPUBuffer* vertex_buffer = nullptr;
	SDL_GPUBuffer* index_buffer = nullptr;
	SDL_GPUTransferBuffer* transfer_buffer = nullptr;
	SDL_GPUSampler* texture_sampler = nullptr;
	SDL_GPUTexture* texture = nullptr;

	bool uses_texture = false;

	int vertex_buffer_size = 0;
	int index_buffer_size = 0;

	int current_renderable = 0;
	int current_index = 0;
	int current_vertex = 0;

	int total_indices = 0;
	int total_vertices = 0;

	int change_start = -1;

	unsigned int max_renderables = 0;
	Renderable<VertexType>** renderables;
	glm::mat4x4** transform;

	int* batch_offset;
	int* batch_size;
	glm::mat4x4** batch_transform;

	int num_batches;

	glm::mat4x4 identity;

	RenderSet() {
		num_batches = 0;
		identity = glm::mat4(1.0f);
	}

	void init(RenderGeometryType geometry_type, int max_renderable,
		SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader,
		int vert_buffer_size, int ind_buffer_size, SDL_Surface* texture_surface) {

		if (texture_surface != nullptr)
		{
			uses_texture = true;
		}

		max_renderables = max_renderable;
		renderables = new Renderable<VertexType> *[max_renderables];
		for (unsigned int i = 0; i < max_renderables; ++i) {
			renderables[i] = nullptr;
		}
		transform = new glm::mat4x4 * [max_renderables];
		for (unsigned int i = 0; i < max_renderables; ++i) {
			transform[i] = nullptr;
		}
		batch_transform = new glm::mat4x4 * [max_renderables];
		for (unsigned int i = 0; i < max_renderables; ++i) {
			batch_transform[i] = nullptr;
		}
		batch_offset = new int[max_renderables];
		batch_size = new int[max_renderables];

		vertex_buffer_size = vert_buffer_size;
		index_buffer_size = ind_buffer_size;

		init_shaders(vertex_shader, fragment_shader);
		init_buffers();
		if (uses_texture) {
			init_texture(texture_surface);
		}
	}


	void begin()
	{
		current_renderable = 0;
		current_index = 0;
		current_vertex = 0;

		total_indices = 0;
		total_vertices = 0;

		change_start = -1;
	}

	void add(Renderable<VertexType>* renderable, glm::mat4x4* transf)
	{
		if (renderable->get_num_indices() < 1) return;
		// no change detected
		if (renderables[current_renderable] != nullptr && renderables[current_renderable]->id == renderable->id)
		{
			current_renderable++;

			total_vertices += renderable->vertices.size();
			total_indices += renderable->indices.size();
		}
		// change detected
		else if ((current_renderable < max_renderables) &&
			(total_vertices + renderable->get_num_vertices() < vertex_buffer_size) &&
			(total_indices + renderable->get_num_indices() < index_buffer_size))
		{
			if (change_start == -1)
				change_start = current_renderable;

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
		if (current_renderable == 0) return;
		if (change_start < 0) change_start = current_renderable;
		auto* transfer_data = static_cast<VertexType*>(SDL_MapGPUTransferBuffer(sdl_device, transfer_buffer, false));

		for (int i = change_start; i < current_renderable; i++)
		{
			if (current_vertex + renderables[i]->vertices.size() < vertex_buffer_size)
			{
				Renderable<VertexType>* renderable = renderables[i];
				memcpy(&transfer_data[current_vertex], renderable->vertices.data(), sizeof(VertexType) * renderable->vertices.size());
				current_vertex += renderable->vertices.size();
			}

		}

		// a change was detected
		// rebuild the index buffer and draw buffers

		// currently this check is all or nothing
		// one optimization in the future is to allow partial updates
		if (change_start < current_renderable)
		{
			Uint16* indexData = (Uint16*)&transfer_data[current_vertex];
			int vertex_offset = 0;
			num_batches = 0;
			bool first_time = true;
			for (int i = 0; i < current_renderable; i++)
			{
				Renderable<VertexType>* renderable = renderables[i];
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

			if (num_batches == 1)
				batch_size[0] = current_index;

			SDL_UnmapGPUTransferBuffer(sdl_device, transfer_buffer);
			SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(sdl_cmdbuf);

			// upload the vertex data
			SDL_GPUTransferBufferLocation transfer_buffer_location_vertex{};
			transfer_buffer_location_vertex.transfer_buffer = transfer_buffer;
			transfer_buffer_location_vertex.offset = 0;

			SDL_GPUBufferRegion buffer_region{};
			buffer_region.buffer = vertex_buffer;
			buffer_region.offset = 0;
			buffer_region.size = sizeof(VertexType) * current_vertex;

			SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location_vertex,
				&buffer_region, false);

			// upload the index data
			SDL_GPUTransferBufferLocation transfer_buffer_location_index{};
			transfer_buffer_location_index.transfer_buffer = transfer_buffer;
			transfer_buffer_location_index.offset = sizeof(VertexType) * current_vertex;

			SDL_GPUBufferRegion buffer_region_index{};
			buffer_region_index.buffer = index_buffer;
			buffer_region_index.offset = 0;
			buffer_region_index.size = sizeof(Uint16) * current_index;

			SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location_index,
				&buffer_region_index, false);

			SDL_EndGPUCopyPass(copy_pass);
		}
	}

	void render_all_geometries(SDL_GPURenderPass* render_pass, const glm::mat4x4& view_proj) const
	{
		if (current_renderable == 0) return;
		SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
		SDL_GPUBufferBinding vertex_buffer_binding{};
		vertex_buffer_binding.buffer = vertex_buffer;
		vertex_buffer_binding.offset = 0;
		SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_buffer_binding, 1);

		SDL_GPUBufferBinding index_buffer_binding{};
		index_buffer_binding.buffer = index_buffer;
		index_buffer_binding.offset = 0;
		SDL_BindGPUIndexBuffer(render_pass, &index_buffer_binding, SDL_GPU_INDEXELEMENTSIZE_16BIT);

		if (uses_texture)
		{
			SDL_GPUTextureSamplerBinding texture_sampler_binding{};
			texture_sampler_binding.texture = texture;
			texture_sampler_binding.sampler = texture_sampler;

			SDL_BindGPUFragmentSamplers(render_pass, 0, &texture_sampler_binding, 1);
		}

		glm::mat4x4 view_proj_model;

		for (int i = 0; i < num_batches; i++)
		{
			if (batch_size[i] > 0)
			{

				// TODO explicit create a uses_transform flag or more broadly support uniforms
				// or add transforms to fonts
				if (!uses_texture)
				{
					view_proj_model = view_proj * (*batch_transform[i]);
					SDL_PushGPUVertexUniformData(sdl_cmdbuf, 0, &view_proj_model, sizeof(glm::mat4x4));
				}
				SDL_DrawGPUIndexedPrimitives(render_pass, batch_size[i], 1, batch_offset[i], 0, 0);
			}
		}

	}

	void cleanup() const
	{
		delete[] renderables;
		delete[] transform;
		delete[] batch_offset;
		delete[] batch_size;
		delete[] batch_transform;

		SDL_ReleaseGPUTransferBuffer(sdl_device, transfer_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, vertex_buffer);
		SDL_ReleaseGPUBuffer(sdl_device, index_buffer);
		SDL_ReleaseGPUGraphicsPipeline(sdl_device, graphics_pipeline);
	}

private:
	void init_shaders(SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader) {
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
		vertex_input_description.pitch = sizeof(VertexType);

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
		if (uses_texture)
		{
			vertex_attribute2.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
		}
		else
		{
			vertex_attribute2.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
		}
		vertex_attribute2.location = 1;
		vertex_attribute2.offset = sizeof(float) * 2;

		std::vector vertex_attributes{ vertex_attribute, vertex_attribute2 };
		pipeline_create_info.vertex_input_state.vertex_attributes = vertex_attributes.data();
		pipeline_create_info.vertex_input_state.num_vertex_attributes = vertex_attributes.size();

		graphics_pipeline = SDL_CreateGPUGraphicsPipeline(sdl_device, &pipeline_create_info);

		SDL_ReleaseGPUShader(sdl_device, vertex_shader);
		SDL_ReleaseGPUShader(sdl_device, fragment_shader);
	}

	void init_buffers() {
		int vertex_size = sizeof(VertexType);

		SDL_GPUBufferCreateInfo vertex_buffer_create_info{};
		vertex_buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		vertex_buffer_create_info.size = vertex_size * vertex_buffer_size;
		vertex_buffer = SDL_CreateGPUBuffer(sdl_device, &vertex_buffer_create_info);

		SDL_GPUBufferCreateInfo index_buffer_create_info{};
		index_buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_INDEX;
		index_buffer_create_info.size = sizeof(uint16_t) * index_buffer_size;
		index_buffer = SDL_CreateGPUBuffer(sdl_device, &index_buffer_create_info);

		SDL_GPUTransferBufferCreateInfo transfer_buffer_create_info{};
		transfer_buffer_create_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		transfer_buffer_create_info.size = (vertex_size * vertex_buffer_size) + (sizeof(Uint16) * index_buffer_size);

		transfer_buffer = SDL_CreateGPUTransferBuffer(sdl_device, &transfer_buffer_create_info);
	}

	void init_texture(SDL_Surface* texture_surface) {
		SDL_GPUSamplerCreateInfo sample_info{};
		sample_info.min_filter = SDL_GPU_FILTER_LINEAR;
		sample_info.mag_filter = SDL_GPU_FILTER_LINEAR;
		sample_info.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR;
		sample_info.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		sample_info.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
		sample_info.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;

		texture_sampler = SDL_CreateGPUSampler(sdl_device, &sample_info);

		SDL_GPUTextureCreateInfo texture_create_info{};
		texture_create_info.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
		texture_create_info.type = SDL_GPU_TEXTURETYPE_2D;
		texture_create_info.width = texture_surface->w;
		texture_create_info.height = texture_surface->h;
		texture_create_info.layer_count_or_depth = 1;
		texture_create_info.num_levels = 1;
		texture_create_info.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;

		texture = SDL_CreateGPUTexture(sdl_device, &texture_create_info);

		SDL_GPUTransferBufferCreateInfo texture_transfer_create_info{};
		texture_transfer_create_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		texture_transfer_create_info.size = texture_surface->w * texture_surface->h * 4;

		SDL_GPUTransferBuffer* texture_transfer_buffer = SDL_CreateGPUTransferBuffer(sdl_device, &texture_transfer_create_info);

		Uint8* texture_transfer_ptr = (Uint8*)SDL_MapGPUTransferBuffer(
			sdl_device,
			texture_transfer_buffer,
			false
		);

		for (int i = 0; i < texture_surface->h; i++)
		{
			for (int j = 0; j < texture_surface->w; j++)
			{
				Uint8* pixel = (Uint8*)texture_surface->pixels + (i * texture_surface->pitch) + (j * 3);
				texture_transfer_ptr[(i * texture_surface->w + j) * 4] = pixel[0];
				texture_transfer_ptr[(i * texture_surface->w + j) * 4 + 1] = pixel[1];
				texture_transfer_ptr[(i * texture_surface->w + j) * 4 + 2] = pixel[2];
				texture_transfer_ptr[(i * texture_surface->w + j) * 4 + 3] = 255;
			}
		}

		SDL_UnmapGPUTransferBuffer(sdl_device, texture_transfer_buffer);

		SDL_GPUCommandBuffer* upload_cmd_buffer = SDL_AcquireGPUCommandBuffer(sdl_device);
		SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buffer);

		SDL_GPUTextureTransferInfo texture_transfer_info{};
		texture_transfer_info.transfer_buffer = texture_transfer_buffer;
		texture_transfer_info.offset = 0;

		SDL_GPUTextureRegion texture_region{};
		texture_region.texture = texture;
		texture_region.w = texture_surface->w;
		texture_region.h = texture_surface->h;
		texture_region.d = 1;

		SDL_UploadToGPUTexture(
			copy_pass,
			&texture_transfer_info,
			&texture_region,
			false
		);

		SDL_EndGPUCopyPass(copy_pass);
		SDL_SubmitGPUCommandBuffer(upload_cmd_buffer);

		SDL_ReleaseGPUTransferBuffer(sdl_device, texture_transfer_buffer);
	}
};