struct StreamBuffer {
    char pad_00[0x20];
    unsigned short flags;
};

struct StreamTask {
    char pad_00[0x4];
    int id;
    char pad_08[0xC];
    StreamBuffer *buffer;

    static unsigned char finished(StreamBuffer *buffer) { return (buffer->flags & 0x20) != 0; }
};

extern "C" {

extern int func_08A06A18(StreamTask *self);
extern void func_08A06808(StreamTask *self);
extern void func_08A069D4(StreamTask *self);
extern void func_088DA060(StreamBuffer *buffer);

/* Advances the stream, then frees its buffer once the buffer reports it is finished. */
int func_08A0676C(StreamTask *self)
{
    if (func_08A06A18(self) == 1) {
        if (self->id == -1) {
            func_08A06808(self);
        }
    } else {
        func_08A069D4(self);
    }
    if (self->buffer != 0 && StreamTask::finished(self->buffer) == 1) {
        if (self->buffer != 0) {
            func_088DA060(self->buffer);
            self->buffer = 0;
        }
    }
    return 0;
}

}
