typedef struct ModelData {
    char pad_00[0x18];
    short bone;
} ModelData;

typedef struct ModelRef {
    char pad_00[0x24];
    ModelData *data;
} ModelRef;

typedef struct ModelNode {
    char pad_000[0x64];
    ModelRef *model;
} ModelNode;

extern int func_08AD2B84(ModelNode *self, int bone, unsigned char a, unsigned char b);

int func_08AD37A8(ModelNode *self, unsigned char a, unsigned char b)
{
    short bone = self->model->data->bone;
    int result = 0;
    if (bone >= 0) {
        result = func_08AD2B84(self, bone, a, b);
    }
    return result;
}
