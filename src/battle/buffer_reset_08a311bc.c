typedef struct ResetBuffer311BC {
    char pad_000[0x8C];
    int field_8C;
    char pad_090[0xB0];
    int field_140;
    float field_144;
} ResetBuffer311BC;

typedef struct BufferOwner311BC {
    char pad_000[0x8];
    int state;
    char pad_00C[0x8];
    ResetBuffer311BC *buffer;
} BufferOwner311BC;

extern void func_08935828(void *dst, int value, int size);

void func_08A311BC(BufferOwner311BC *self, ResetBuffer311BC *buffer)
{
    self->buffer = buffer;
    func_08935828(buffer, 0, 0x148);
    if (self->state == 4) {
        self->buffer->field_8C = 4;
        self->buffer->field_140 = 0;
        self->buffer->field_144 = 0.0f;
    }
}
