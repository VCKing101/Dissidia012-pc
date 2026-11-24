/* Resets a record: id = -1, byte at 0x20 = 0, short at 0x22 = -1; returns it. */
typedef struct { int id; char pad[0x1C]; char b; char pad2; short s; } Rec08970EE8;
Rec08970EE8 *func_08970EE8(Rec08970EE8 *p) {
    int m = -1;
    p->id = m;
    p->b = 0;
    p->s = m;
    return p;
}
