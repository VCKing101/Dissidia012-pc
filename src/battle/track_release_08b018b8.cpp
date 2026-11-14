typedef struct TrackHandle {
    void *handle;
    float field_04;
} TrackHandle;

typedef struct TrackSet {
    char pad_00[0x50];
    TrackHandle handles[2];
} TrackSet;

extern "C" {

extern void func_088A6AA4(void *handle);

int func_08B018B8(TrackSet *self)
{
    for (int i = 0; i < 2; i++) {
        TrackHandle *entry = &self->handles[i];
        if (entry->handle == 0) {
            continue;
        }
        func_088A6AA4(entry->handle);
        entry->handle = 0;
    }
    return 0;
}

}
