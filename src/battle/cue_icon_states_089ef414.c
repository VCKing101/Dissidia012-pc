/* Cue icon: three state steps that swap its models for the owner's side
 * colour (state -1 -> 0 -> 1 -> 2). */
typedef struct CueIcon {
    unsigned char open;
    unsigned char pad_01[3];
    int state;
    void *frame;
    unsigned char pad_0c[4];
    int shown;
    float time;
    void *glow;
    int owner;
    void *source;
} CueIcon;

extern void func_088DA060(void *model);
extern unsigned int func_08A22CA8(int owner);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_088D9F00(void *model, float frame);
extern void func_089EF87C(CueIcon *icon, float x, float y);
extern int D_089EFC70[2];
extern int D_089EFC78[2];
extern int D_089EFC88[2];

int func_089EF414(CueIcon *icon)
{
    unsigned int side;

    if (!icon->open) {
        return -1;
    }
    if (icon->state != -1) {
        return -2;
    }
    if (icon->glow != 0) {
        func_088DA060(icon->glow);
        icon->glow = 0;
    }
    if (icon->frame != 0) {
        func_088DA060(icon->frame);
        icon->frame = 0;
    }
    side = func_08A22CA8(icon->owner);
    if (side >= 2) {
        return -3;
    }
    icon->frame = func_088DAA40(icon->source, D_089EFC70[side], 0);
    if (icon->frame == 0) {
        return -4;
    }
    icon->state = 0;
    return 0;
}

int func_089EF4F0(CueIcon *icon)
{
    unsigned int side;
    float zero;

    if (!icon->open) {
        return -1;
    }
    if (icon->state != 0) {
        return -2;
    }
    if (icon->frame == 0) {
        return -3;
    }
    if (icon->glow != 0) {
        func_088DA060(icon->glow);
        icon->glow = 0;
    }
    if (icon->frame != 0) {
        func_088DA060(icon->frame);
        icon->frame = 0;
    }
    side = func_08A22CA8(icon->owner);
    if (side >= 2) {
        return -4;
    }
    icon->frame = func_088DAA40(icon->source, 5, 0);
    if (icon->frame == 0) {
        return -5;
    }
    icon->glow = func_088DAA40(icon->source, D_089EFC78[side], 0);
    if (icon->glow == 0) {
        return -6;
    }
    zero = 0.0f;
    func_088D9F00(icon->frame, zero);
    func_088D9F00(icon->glow, zero);
    icon->state = 1;
    icon->shown = 1;
    icon->time = zero;
    func_089EF87C(icon, zero, zero);
    return 0;
}

int func_089EF644(CueIcon *icon)
{
    unsigned int side;

    if (!icon->open) {
        return -1;
    }
    if (icon->state != 1) {
        return -2;
    }
    if (icon->frame == 0 || icon->glow == 0) {
        return -3;
    }
    if (icon->glow != 0) {
        func_088DA060(icon->glow);
        icon->glow = 0;
    }
    if (icon->frame != 0) {
        func_088DA060(icon->frame);
        icon->frame = 0;
    }
    side = func_08A22CA8(icon->owner);
    if (side >= 2) {
        return -4;
    }
    icon->frame = func_088DAA40(icon->source, D_089EFC88[side], 0);
    if (icon->frame == 0) {
        return -5;
    }
    icon->state = 2;
    return 0;
}
