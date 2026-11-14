#include "main/agent2_types.h"

struct Agent2Vec4 {
    float x;
    float y;
    float z;
    float w;
};

extern "C" Agent2GlobalState *D_089D5EC0;
extern "C" void func_089A4AF8(Agent2Vec4 *, void *, ...);
extern "C" void func_089A3868(Agent2Vec4 *, void *);

/* Maps a projected point in [-1, 1] to the 480x272 screen. */
struct Agent2Screen {
    static void place(Agent2Vec4 *out, const Agent2Vec4 *projected) {
        out->x = (projected->x * 0.5f + 0.5f) * 480.0f;
        out->y = 272.0f - (projected->y * 0.5f + 0.5f) * 272.0f;
        out->w = 0.0f;
        out->z = 0.0f;
    }
};

extern "C" void func_08866DA8(Agent2Vec4 *out, Agent2GlobalState *state) {
    Agent2Vec4 value;
    func_089A4AF8(&value, state->field_07E0 + 0x50);
    *out = value;
}

extern "C" int func_08866E04(Agent2Vec4 *out, void *source) {
    Agent2Vec4 point;
    Agent2Vec4 projected;
    func_089A3868(&point, source);
    point.w = 1.0f;
    func_089A4AF8(&projected, D_089D5EC0->field_07E0 + 0x50, &point);
    if (projected.w < 0.0f) return 0;
    Agent2Screen::place(out, &projected);
    return 1;
}
