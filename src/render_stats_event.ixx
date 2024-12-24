export module render_stats_event;

import event;

export struct RenderStatsEvent : public Event {
    int num_draw_calls;
    float average_indices;
    float average_triangles;

    RenderStatsEvent(int num_draw_calls, int average_indices, int average_triangles) : 
        num_draw_calls(num_draw_calls) , average_indices(average_indices) , average_triangles(average_triangles) {}
};