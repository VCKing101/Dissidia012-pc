struct ModelNode {
    char pad_000[0x64];
    void *model;
    void *motion;
    void *owner;
    int field_70;
    int field_74;
    char pad_078[0x8];
    int field_80;
    unsigned int flags;
    int index;
    char pad_08C[0x6C];
    const void *vtable;

    static ModelNode *construct(ModelNode *self, const void *vtable, void *model, void *motion, void *owner)
    {
        self->vtable = vtable;
        self->model = model;
        self->motion = motion;
        self->owner = owner;
        self->field_70 = 0;
        self->field_74 = 0;
        self->field_80 = 0;
        self->flags = 0;
        self->index = -1;
        return self;
    }
};

extern "C" {

extern const unsigned char D_08AD75B8[];

ModelNode *func_08AD29D8(ModelNode *self, void *model, void *motion, void *owner)
{
    return ModelNode::construct(self, D_08AD75B8, model, motion, owner);
}

}
