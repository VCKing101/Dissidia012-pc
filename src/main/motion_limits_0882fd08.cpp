/* Raises a motion's two lengths to at least the given frame counts (unset
   lengths take them as they are); 0xFFFF asks for the full length. */
struct Motion0882FD08 {
    char pad_000[0x170];
    float min_length;
    float max_length;

    static float raise(float &v, float m) { return v < 0.0f ? m : (v <= m ? m : v); }
};

extern "C" {

float func_0882FC20(Motion0882FD08 *self, int id);

void func_0882FD08(Motion0882FD08 *self, unsigned short min_frames, unsigned short max_frames)
{
    float lo = min_frames;
    float hi = max_frames;
    self->min_length = Motion0882FD08::raise(self->min_length, lo);
    if (hi == 65535.0f) {
        hi = func_0882FC20(self, -1);
    }
    self->max_length = Motion0882FD08::raise(self->max_length, hi);
}

}
