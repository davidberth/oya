module;

#include <SDL3/SDL.h>
#include <vector>
export module render_system;

import sdl_data;
import vertex; 

export enum RenderGeometryType
{
	TRIANGLE_LIST
};

export class RenderSet {
public:
	SDL_GPUGraphicsPipeline* graphics_pipeline;
	SDL_GPUBuffer* vertex_buffer;

	RenderSet(RenderGeometryType geometry_type, SDL_GPUShader* vertex_shader, SDL_GPUShader* fragment_shader) {

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

		SDL_GPUBufferCreateInfo buffer_create_info{};
		buffer_create_info.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
		buffer_create_info.size = sizeof(Vertex) * 3;

		vertex_buffer = SDL_CreateGPUBuffer(sdl_device, &buffer_create_info);

		SDL_GPUTransferBufferCreateInfo transfer_buffer_create_info{};
		transfer_buffer_create_info.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
		transfer_buffer_create_info.size = sizeof(Vertex) * 3;

		SDL_GPUTransferBuffer* transfer_buffer = SDL_CreateGPUTransferBuffer(sdl_device, &transfer_buffer_create_info);
		Vertex* transfer_data = static_cast<Vertex*>(SDL_MapGPUTransferBuffer(sdl_device, transfer_buffer, false));

		transfer_data[0] = Vertex{ -1, -1, 1.0f, 0.0f, 0.0f };
		transfer_data[1] = Vertex{ 1, -1, 0.0f, 1.0f, 0.0f };
		transfer_data[2] = Vertex{ 0, 1, 0.0f, 0.0f, 1.0f };

		SDL_UnmapGPUTransferBuffer(sdl_device, transfer_buffer);

		// Upload the transfer data to the vertex buffer
		SDL_GPUCommandBuffer* upload_cmd_buffer = SDL_AcquireGPUCommandBuffer(sdl_device);
		SDL_GPUCopyPass* copy_pass = SDL_BeginGPUCopyPass(upload_cmd_buffer);

		SDL_GPUTransferBufferLocation transfer_buffer_location{};
		transfer_buffer_location.transfer_buffer = transfer_buffer;
		transfer_buffer_location.offset = 0;

		SDL_GPUBufferRegion buffer_region{};
		buffer_region.buffer = vertex_buffer;
		buffer_region.offset = 0;
		buffer_region.size = sizeof(Vertex) * 3;

		SDL_UploadToGPUBuffer(copy_pass, &transfer_buffer_location, &buffer_region, false);

		SDL_EndGPUCopyPass(copy_pass);
		SDL_SubmitGPUCommandBuffer(upload_cmd_buffer);
		SDL_ReleaseGPUTransferBuffer(sdl_device, transfer_buffer);
	}

	void render(SDL_GPURenderPass* render_pass)
	{
		SDL_BindGPUGraphicsPipeline(render_pass, graphics_pipeline);
		SDL_GPUBufferBinding vertex_buffer_binding{};
		vertex_buffer_binding.buffer = vertex_buffer;
		vertex_buffer_binding.offset = 0;
		SDL_BindGPUVertexBuffers(render_pass, 0, &vertex_buffer_binding, 1);
		SDL_DrawGPUPrimitives(render_pass, 3, 1, 0, 0);
	}

	void cleanup()
	{
		SDL_ReleaseGPUBuffer(sdl_device, vertex_buffer);
		SDL_ReleaseGPUGraphicsPipeline(sdl_device, graphics_pipeline);
	}
};




