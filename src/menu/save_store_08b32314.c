/* Copies the save block at D_09B6DD10 + 0x134 into the message block and
   clears its first word. */
struct Block08B32314 {
    int words[0x30];
};

struct Global08B32314 {
    char pad_000[0x134];
    struct Block08B32314 field_134;
};

extern void *D_08B01CF8;
extern struct Global08B32314 D_09B6DD10;
extern struct Block08B32314 *func_08AF34A0(void *queue);

void func_08B32314(void)
{
    struct Block08B32314 *block = func_08AF34A0(D_08B01CF8);
    *block = D_09B6DD10.field_134;
    block->words[0] = 0;
}
