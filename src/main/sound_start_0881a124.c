/* Starts the sound named by the handle's id: -1 when the handle is unused,
   -2 without a sound bank. */
typedef union Handle0881A124Flags {
    unsigned int word;
    struct {
        unsigned int used : 1;
        signed int id : 16;
        unsigned int playing : 1;
        unsigned int started : 1;
    } bits;
} Handle0881A124Flags;

typedef struct Handle0881A124 {
    Handle0881A124Flags flags;
    float time;
} Handle0881A124;

typedef struct Bank0881A124 {
    int field_00;
} Bank0881A124;

typedef struct Sound0881A124 {
    char pad_000[0x8B0];
    Bank0881A124 bank;
} Sound0881A124;

extern Sound0881A124 *D_089D5EC0;

int func_08899300(void);
void func_08898464(int player, int id, Bank0881A124 *bank);

int func_0881A124(Handle0881A124 *handle)
{
    Bank0881A124 *bank;
    if (!handle->flags.bits.used) {
        return -1;
    }
    bank = &D_089D5EC0->bank;
    if (bank == 0) {
        return -2;
    }
    func_08898464(func_08899300(), handle->flags.bits.id, bank);
    handle->flags.word |= 0x60000;
    handle->time = 0.0f;
    return 0;
}
