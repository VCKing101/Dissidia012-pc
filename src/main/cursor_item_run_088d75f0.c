typedef struct Item Item;
typedef struct Obj { char data_00[0x24]; char cursor[0x5C - 0x24]; char items[4]; } Obj;
extern int func_088D723C(Obj *o);
extern int func_088D82F0(void *cursor);
extern int func_088D93E0(void *items);
extern Item *func_088D9498(void *items, int index);
extern int func_088D8E58(Item *item);

int func_088D75F0(Obj *o)
{
    int index;
    Item *item;
    if (func_088D723C(o) == 0) {
        return -1;
    }
    index = func_088D82F0(o->cursor);
    if (index < 0 || index >= func_088D93E0(o->items)) {
        return -1;
    }
    item = func_088D9498(o->items, index);
    if (item == 0) {
        return -1;
    }
    return func_088D8E58(item);
}
