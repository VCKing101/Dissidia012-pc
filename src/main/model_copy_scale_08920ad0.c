/* Copies the scale of a looked-up record into the owner's model. */
typedef struct ScaleRecord {
    int field_00;
    int field_04;
    float scale;                /* 0x08 */
} ScaleRecord;

typedef struct ModelRef {
    int id;                     /* 0x00 */
    char pad_04[0x44];
    float scale;                /* 0x48 */
} ModelRef;

typedef struct ModelOwner {
    char pad_00[0xC];
    ModelRef *model;            /* 0x0C */
} ModelOwner;

extern ScaleRecord *func_08B43424(int key, int id);

void func_08920AD0(ModelOwner *owner, int key)
{
    ScaleRecord *rec = func_08B43424(key, owner->model->id);
    if (rec != 0) {
        owner->model->scale = rec->scale;
    }
}
