typedef struct HandleEntry30 {
    char pad_00[0x20];
    void *handle;
    char pad_24[0xC];
} HandleEntry30;

typedef struct HandleTable30 {
    char pad_00[0x10];
    HandleEntry30 entries[15];
} HandleTable30;

typedef struct HandleTableOwner {
    char pad_00[0x14];
    HandleTable30 *table;
} HandleTableOwner;

extern "C" {

extern void func_088A6C70(void *handle);

void func_08A45B5C(HandleTableOwner *self)
{
    for (int i = 0; i < 15; i++) {
        HandleEntry30 *entry = &self->table->entries[i];
        if (entry->handle == 0) {
            continue;
        }
        func_088A6C70(entry->handle);
        entry->handle = 0;
    }
}

}
