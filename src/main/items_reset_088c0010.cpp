/* Clears the count and initialises the four 12-byte items. */
struct Item088C0010 {
    int a;
    int b;
    int c;
};

struct Owner088C0010 {
    Item088C0010 items[4];
    int count;
};

extern "C" {
void func_088BFF94(Item088C0010 *item);
void func_088C0010(Owner088C0010 *self)
{
    self->count = 0;
    for (int i = 0; i < 4; i++) {
        func_088BFF94(&self->items[i]);
    }
}
}
