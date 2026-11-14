/* Moves the slider knob to step index of the list's steps (never past the track end) and shows it. */
typedef struct SliderKnob {
    char pad_00[0x20];
    unsigned short field_20;
} SliderKnob;

typedef struct SliderParts {
    char pad_00[0x44];
    SliderKnob *field_44;
} SliderParts;

typedef struct SliderState {
    char pad_000[0x4];
    SliderParts *field_04;
    char pad_008[0x410 - 0x8];
    void *field_410;
} SliderState;

typedef struct SliderOwner {
    char pad_000[0x418];
    SliderState *field_418;
} SliderOwner;

extern int func_088DA024(SliderKnob *knob);
extern int func_08A484D0(void *list);
extern void func_088D9AEC(SliderKnob *knob, float position);

void func_08A46BC0(SliderOwner *self, int index)
{
    int length[1];
    int position[1];
    int steps;

    length[0] = func_088DA024(self->field_418->field_04->field_44);
    steps = func_08A484D0(self->field_418->field_410);
    position[0] = length[0] * index / (steps - 1);
    if (length[0] < position[0]) {
        position[0] = length[0];
    }
    func_088D9AEC(self->field_418->field_04->field_44, position[0]);
    self->field_418->field_04->field_44->field_20 &= ~2;
}
