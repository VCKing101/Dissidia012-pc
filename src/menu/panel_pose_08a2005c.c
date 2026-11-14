/* Copies the pose of the current kind's model (table row present) onto the panel's marker model. */
typedef struct PoseKind {
    int field_00;
    int field_04;
    char pad_08[0xC];
} PoseKind;

typedef struct PoseState {
    char pad_00000[0x25C50];
    int field_25C50;
    char pad_25C54[0x8];
    int field_25C5C;
} PoseState;

typedef struct PoseOwner {
    char pad_000[0x154];
    PoseState *field_154;
} PoseOwner;

typedef struct PosePanel {
    char pad_00[0x90];
    void *field_90;
    char pad_94[0x8];
    void *field_9C;
} PosePanel;

extern void func_088DA0A8(void *model, int part, float *matrix, int *color);
extern void func_088D9B84(void *model, float *matrix);
extern void func_088D9AF4(void *model, int *color);
extern PoseKind D_08A23FB0[];
extern unsigned short D_08A23FEC[];

void func_08A2005C(PoseOwner *self, PosePanel *panel)
{
    float matrix[16];
    int color;
    int kind = self->field_154->field_25C50;

    if (kind == -1) {
        return;
    }
    if (D_08A23FB0[kind].field_04 == -1) {
        return;
    }
    func_088DA0A8(panel->field_9C, D_08A23FEC[self->field_154->field_25C5C], matrix, &color);
    func_088D9B84(panel->field_90, matrix);
    func_088D9AF4(panel->field_90, &color);
}
