/* Opens the status panel: plays the open animations on the header and
 * the layout, places the three icons and the badge from the layout and
 * resets the panel's value and fade. */
typedef struct PanelRgba {
    unsigned char r, g, b, a;
} PanelRgba;

typedef struct PanelMatrix {
    float m[16];
} PanelMatrix;

typedef struct PanelLayout {
    unsigned char pad_00[0xE];
    short frame;
} PanelLayout;

typedef struct PanelParts {
    int value;
    float time;
    float alpha;
    int state;
    int count;
    float fade;
    unsigned char pad_18[0x2C];
    void *icons[3];
    void *badge;
    void *header;
    PanelLayout *layout;
} PanelParts;

typedef struct PanelEffect {
    unsigned char pad_00[0x14];
    PanelParts *parts;
} PanelEffect;

extern void func_088D9F5C(void *sprite, int frame, int flags);
extern void func_088D9CFC(void *sprite);
extern void func_088DA0A8(void *layout, int slot, PanelMatrix *matrix, PanelRgba *color);
extern void func_088D9B84(void *sprite, PanelMatrix *matrix);

void func_08A409B8(PanelEffect *self, int value)
{
    int i;
    PanelMatrix matrix;
    PanelRgba color;
    func_088D9F5C(self->parts->header, 0x16, 1);
    func_088D9CFC(self->parts->header);
    func_088D9F5C(self->parts->layout, 0x17, 1);
    self->parts->layout->frame = 0x18;
    for (i = 0; i < 3; i++) {
        func_088DA0A8(self->parts->layout, i, &matrix, &color);
        func_088D9B84(self->parts->icons[i], &matrix);
    }
    func_088DA0A8(self->parts->layout, 3, &matrix, &color);
    func_088D9B84(self->parts->badge, &matrix);
    self->parts->fade = 0.0f;
    self->parts->count = 0;
    self->parts->value = value;
    self->parts->time = 0.0f;
    self->parts->alpha = 1.0f;
    self->parts->state = 0;
}
