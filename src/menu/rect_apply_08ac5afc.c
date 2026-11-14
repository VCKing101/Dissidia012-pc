/* Applies a new view rectangle: updates the parts that react to it and stores it. */
typedef struct ApplyRect {
    int field_00;
    int field_04;
    int field_08;
    int field_0C;
} ApplyRect;

typedef struct ApplyView {
    char pad_000[0x9F4];
    ApplyRect field_9F4;
} ApplyView;

typedef struct ApplyPanel3 {
    char pad_000[0x138];
    ApplyView *field_138;
} ApplyPanel3;

extern int func_08AC49EC(ApplyPanel3 *self, ApplyRect *rect);
extern int func_08AC4A48(ApplyPanel3 *self, ApplyRect *rect);
extern void func_08AC4FE8(ApplyPanel3 *self, ApplyRect *rect);
extern void func_08AC523C(ApplyPanel3 *self, ApplyRect *rect);

void func_08AC5AFC(ApplyPanel3 *self, ApplyRect *rect)
{
    int moved = func_08AC49EC(self, rect);
    int resized = func_08AC4A48(self, rect);
    if (moved != 0) {
        func_08AC4FE8(self, rect);
    }
    if (resized != 0) {
        func_08AC523C(self, rect);
    }
    self->field_138->field_9F4 = *rect;
}
