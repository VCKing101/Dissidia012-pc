/* Clears the count and initialises the five 0x118-byte items. */
struct Item088C0228 {
    char data[0x118];
};

struct Group088C0228 {
    Item088C0228 items[5];
    int count;
};

extern "C" {
void func_088C01C0(Item088C0228 *item);
void func_088C0228(Group088C0228 *self)
{
    self->count = 0;
    for (int i = 0; i < 5; i++) {
        func_088C01C0(&self->items[i]);
    }
}
}
