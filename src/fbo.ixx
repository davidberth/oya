module;

#include <GL/glew.h>
#include "loguru.hpp"

export module fbo;


export class FBO 
{
private:
	unsigned int fbo;
	unsigned int texture;
	unsigned int rbo;

public:
	FBO(){

		create_frame_buffer();
		attach_to_viewport();
	}
	~FBO() {
		delete_frame_buffer();
	}

	void create_frame_buffer()
	{
		glGenFramebuffers(1, &fbo);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, get_width(), get_height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		DLOG_F(INFO, "Creating FBO height %d width %d", get_width(), get_height());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, get_width(), get_height());
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			LOG_F(ERROR, "Framebuffer is not complete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		set_texture_index(texture);
	}

	void delete_frame_buffer()
	{
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &texture);
		glDeleteRenderbuffers(1, &rbo);
	}

	void bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glViewport(0, 0, get_width(), get_height());
	}
	void unbind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	void bind_texture() {
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void resize()
	{
		LOG_F(INFO, "Resizing FBO %d %d", get_width(), get_height());
		delete_frame_buffer();
		create_frame_buffer();
	}

	void attach_to_viewport() {
		//add_listener(&viewport_data, &FBO::resize);
	}

	void set_texture_index(int id) { 
		//viewport_data.texture_index = id; 
	}

	
	int get_width() { return 1280; }
	int get_height() { return 720; }
	//int get_width() { return viewport_data.width; }
	//int get_height() { return viewport_data.height; }

};