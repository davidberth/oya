export module render_stats_event;

import event;

export enum RenderStatsEventSet {
    world_geometry,
    debug_render
};

export struct RenderStatsEvent : public Event {
	RenderStatsEventSet render_set;
    int num_draw_calls;
    float average_indices;
    float average_elements;

    RenderStatsEvent(RenderStatsEventSet render_set, int num_draw_calls, int average_indices, int average_elements) : 
        render_set(render_set) , num_draw_calls(num_draw_calls) , average_indices(average_indices) , average_elements(average_elements) {}
};