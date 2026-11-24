/* Sets a menu's colour: at once on the menu and its parts when `frames` is
   0, otherwise as a fade over that many frames. */
typedef struct Color {
    float r;
    float g;
    float b;
    float a;
} Color;

typedef struct Part {
    char pad_00[0xC];
    int sprite;
} Part;

typedef struct PartColor {
    char pad_00[0x44];
    int color;
} PartColor;

typedef struct PartSlot {
    char data[0x130];
} PartSlot;

typedef struct FrameInfo {
    char pad_00[0x18];
    float step;
} FrameInfo;

typedef struct Fader {
    char pad_000[0x54];
    int color;
    char pad_058[0x74 - 0x58];
    Part *base;
    char pad_078[0x140 - 0x78];
    PartSlot parts[3];
    char pad_4D0[0x10];
    PartSlot extra;
    unsigned char has_extra;
    char pad_611[0x6D0 - 0x611];
    unsigned char active;
    char pad_6D1[0x6E0 - 0x6D1];
    Color to;
    Color from;
    float duration;
    float time;
    int target;
} Fader;

extern Part *func_08895420(PartSlot *slot);
extern PartColor *func_08895428(PartSlot *slot);
extern void func_089AD2D0(int sprite, int color);
extern void func_089A4970(Color *out, int color);

void func_08893A20(Fader *self, int color, unsigned int frames)
{
    int i;

    self->target = color;
    if (frames == 0) {
        self->color = color;
        self->active = 0;
        func_089AD2D0(self->base->sprite, color);
        if (self->has_extra) {
            Part *part = func_08895420(&self->extra);
            PartColor *pc = func_08895428(&self->extra);
            if (part != 0) {
                pc->color = color;
                func_089AD2D0(part->sprite, color);
            }
        }
        for (i = 0; i < 3; i++) {
            Part *part = func_08895420(&self->parts[i]);
            PartColor *pc = func_08895428(&self->parts[i]);
            if (part != 0) {
                pc->color = color;
                func_089AD2D0(part->sprite, color);
            }
        }
    } else {
        func_089A4970(&self->to, color);
        func_089A4970(&self->from, self->color);
        self->time = 0.0f;
        self->duration = frames;
        self->active = 1;
    }
}
