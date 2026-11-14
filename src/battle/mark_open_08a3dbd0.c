/* Opens the banner mark: loads its sprite (frame 11) and starts its model,
 * either the banner's own (cue -1) or the fighter's model for the cue. */
typedef struct MarkSprite {
    unsigned char pad_00[0xE];
    short frame;
} MarkSprite;

typedef struct MarkModelSet {
    unsigned char pad_00[0xCC];
    void *models;
} MarkModelSet;

typedef struct MarkFighter {
    unsigned char pad_000[0x390];
    MarkModelSet *model;
} MarkFighter;

typedef struct MarkOptions {
    unsigned char pad_000[0x220];
    void *sheet;
} MarkOptions;

typedef struct MarkBanner {
    unsigned char pad_000[0x228];
    MarkSprite *mark;
    unsigned char pad_22c[0x20];
    void *model;
    unsigned char pad_250[0x1C];
    MarkFighter *fighter;
    unsigned char pad_270[0x18];
    unsigned char own_models[4];
} MarkBanner;

extern MarkOptions *D_08A405E0;
extern MarkSprite *func_088DAA40(void *sheet, int frame, int flags);
extern void *func_0887AD78(void *models, int a, int id, int b, int c, int d);
extern void func_0887B98C(void *model);
extern void func_0887B9C4(void *model);

void func_08A3DBD0(MarkBanner *self, int cue)
{
    void *model;
    if (self->mark != 0) {
        return;
    }
    self->mark = func_088DAA40(D_08A405E0->sheet, 0xA, 0);
    self->mark->frame = 0xB;
    if (cue == -1) {
        model = self->model = func_0887AD78(self->own_models, 0, 0, 0, 0, 0);
    } else {
        model = self->model = func_0887AD78(self->fighter->model->models, 0, cue, 0, 0, 0);
    }
    if (model != 0) {
        func_0887B98C(model);
        func_0887B9C4(self->model);
    }
}
