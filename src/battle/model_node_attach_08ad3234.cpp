typedef struct ModelData {
    char pad_00[0xE];
    short bone;
    char pad_10[0x28];
    unsigned int flags;
} ModelData;

typedef struct ModelRef {
    char pad_00[0x24];
    ModelData *data;
} ModelRef;

typedef struct ModelNode {
    char pad_000[0x64];
    ModelRef *model;
    char pad_068[0x1C];
    unsigned int flags;

    static bool attached(ModelData *data) { return (data->flags & 0x20) != 0; }
} ModelNode;

extern "C" {

extern void func_08AD2B84(ModelNode *self, int bone, int a, int b);

void func_08AD3234(ModelNode *self)
{
    ModelData *data = self->model->data;
    if (data->bone >= 0 && ModelNode::attached(data) == 1 && (self->flags & 0x10) == 0) {
        func_08AD2B84(self, data->bone, 1, 0);
    }
    self->flags |= 1;
}

}
