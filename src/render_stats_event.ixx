export module render_stats_event;

import event;

export struct RenderStatsEvent : public Event {
    int num_draw_calls;

    RenderStatsEvent(int num_draw_calls) : num_draw_calls(num_draw_calls) {}
};