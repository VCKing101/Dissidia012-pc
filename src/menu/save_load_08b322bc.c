/* Copies the message block into the save block at D_09B6DD10 + 0x134. */
struct Block08B322BC {
    int words[0x30];
};

struct Global08B322BC {
    char pad_000[0x134];
    struct Block08B322BC field_134;
};

extern void *D_08B01CF8;
extern struct Global08B322BC D_09B6DD10;
extern struct Block08B322BC *func_08AF34A0(void *queue);

void func_08B322BC(void)
{
    struct Block08B322BC *block = func_08AF34A0(D_08B01CF8);
    D_09B6DD10.field_134 = *block;
}
