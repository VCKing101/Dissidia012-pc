extern "C" {
extern void func_088DAB44(void *handle);
}

struct HandleEntry {
    int field_00;
    void *handle;

    static void release(HandleEntry *entry)
    {
        if (entry->handle != 0) {
            func_088DAB44(entry->handle);
            entry->handle = 0;
        }
    }
};

typedef struct HandleEntryList {
    char pad_00[0x4];
    HandleEntry entries[20];
} HandleEntryList;

extern "C" {

extern void func_08A049B8(HandleEntryList *self);

void func_08A049EC(HandleEntryList *self)
{
    for (int i = 0; i < 20; i++) {
        HandleEntry *entry = &self->entries[i];
        if (entry->handle != 0) {
            HandleEntry::release(entry);
        }
    }
    func_08A049B8(self);
}

}
