typedef struct BodyKind {
    int kind;
} BodyKind;

typedef struct Body {
    char pad_00[0x3A4];
    BodyKind *info;
} Body;

typedef struct BodySlot {
    char pad_00[0x4];
    Body *body;
} BodySlot;

typedef struct KindProbe {
    char pad_00[0x64];
    int id;
} KindProbe;

extern BodySlot *func_08AB3264(int id);
extern Body *func_08A973A8(BodySlot *slot);

int func_08A6C100(KindProbe *self)
{
    Body *own;

    if (func_08AB3264(self->id)->body == 0) {
        return 0;
    }
    own = func_08A973A8(func_08AB3264(self->id));
    if (own == 0) {
        return 0;
    }
    return own->info->kind == 0x15;
}
