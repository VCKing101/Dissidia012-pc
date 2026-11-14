/* Constructor: sets the vtable and copies a 0x7C8-byte table into the object.
   (Not a stack-spill function: SNC copies the struct 8 bytes per iteration.) */
typedef struct Table08AB24B8 {
    int words[0x1F2];
} Table08AB24B8;

typedef struct Object08AB24B8 {
    void *vtable;
    Table08AB24B8 table;
} Object08AB24B8;

extern char D_08AB2820[];

Object08AB24B8 *func_08AB24B8(Object08AB24B8 *self, Table08AB24B8 *table)
{
    self->vtable = D_08AB2820;
    self->table = *table;
    return self;
}
