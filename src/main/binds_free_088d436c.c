/* Walks 8-byte records until their third halfword is 0xFFFF: stops and frees the object in the second table named by the first halfword, then clears three words of the object in the first table. */
typedef struct Bind { unsigned short index; short field_02; unsigned short key; short field_06; } Bind;
typedef struct Obj088D436C { char pad[0x30]; int w30; int w34; int w38; } Obj088D436C;
extern void func_0887B8A8(void *task, int v);
extern void func_0887B944(void *task);
void func_088D436C(Obj088D436C **owner, void **tasks, Bind *b)
{
    Bind *cursor = b;
    for (;;) {
        Obj088D436C *o;
        if (cursor->key == 0xFFFF) {
            break;
        }
        if (tasks[cursor->index] != 0) {
            func_0887B8A8(tasks[cursor->index], -1);
            func_0887B944(tasks[cursor->index]);
        }
        o = owner[cursor->key];
        o->w34 = 0;
        o->w30 = 0;
        o->w38 = 0;
        cursor++;
    }
}
