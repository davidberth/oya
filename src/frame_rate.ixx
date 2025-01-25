module;
#include <chrono>
#include <SDL3/SDL.h>

export module frame_rate;

export void log_frame_rate()
{
	static auto last_time = std::chrono::high_resolution_clock::now();
	static int frame_count = 0;

	frame_count++;

	auto current_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = current_time - last_time;

	if (elapsed.count() >= 3.0)
	{
		double framerate = frame_count / elapsed.count();
		SDL_Log("Framerate: %.2f FPS", framerate);
		frame_count = 0;
		last_time = current_time;
	}
}