/* Deleting destructor: clears the owner, destroys both item lists and both sub-objects. */
struct Sub088BF17C { char data[0x40]; };
struct Item088BF17C { char data[0x44]; };
struct ItemList088BF17C {
    int field_00;
    Item088BF17C items[10];
};
struct Owner088BF17C {
    Sub088BF17C sub0;
    Sub088BF17C sub40;
    ItemList088BF17C list1;
    ItemList088BF17C list2;
};
extern "C" {
void func_088BF41C(Owner088BF17C *self);
void func_0899372C(void *arr, int count, int size, void (*dtor)(void *, int), int a, int b);
void func_088BF0B8(void *item, int flags);
void func_088BD518(Sub088BF17C *sub, int flags);
void func_08992F04(void *p);
}
struct OwnerDtor088BF17C {
    static void destroyItems(ItemList088BF17C *list)
    {
        if (list != 0) {
            func_0899372C(list, 10, 0x44, func_088BF0B8, 0, 0);
        }
    }
    static void destroy(Owner088BF17C *self, int flags)
    {
        if (self != 0) {
            func_088BF41C(self);
            destroyItems(&self->list2);
            destroyItems(&self->list1);
            func_088BD518(&self->sub40, 2);
            func_088BD518(&self->sub0, 2);
            if (flags & 1) {
                func_08992F04(self);
            }
        }
    }
};
extern "C" {
void func_088BF17C(Owner088BF17C *self, int flags)
{
    OwnerDtor088BF17C::destroy(self, flags);
}
}
