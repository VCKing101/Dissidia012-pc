/* Walks 8-byte records until their third halfword is 0xFFFF: releases the object in the second table named by the first halfword (clearing the slot), then clears three words of the object in the first table. */
typedef struct Bind { unsigned short index; short field_02; unsigned short key; short field_06; } Bind;
typedef struct Obj088D442C { char pad[0x30]; int w30; int w34; int w38; } Obj088D442C;
extern void func_0887BA5C(void *task);
void func_088D442C(Obj088D442C **owner, void **tasks, Bind *b)
{
    Bind *cursor = b;
    for (;;) {
        Obj088D442C *o;
        if (cursor->key == 0xFFFF) {
            break;
        }
        void *t = tasks[cursor->index];
        if (t != 0) {
            if (t != 0) {
                func_0887BA5C(t);
                tasks[cursor->index] = 0;
            }
        }
        o = owner[cursor->key];
        o->w34 = 0;
        o->w30 = 0;
        o->w38 = 0;
        cursor++;
    }
}
