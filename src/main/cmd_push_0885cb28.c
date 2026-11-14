typedef struct CmdEntry { short id; short pad_02; int a; int b; int c; } CmdEntry;
typedef struct CmdList { int field_00; int count; int field_08; int field_0C; CmdEntry entries[1]; } CmdList;
void func_0885CB28(CmdList *self, int id, int b, int c)
{
    int n = self->count;
    self->entries[n].id = id;
    self->entries[n].a = 0;
    self->entries[n].b = b;
    self->entries[n].c = c;
    self->count = n + 1;
}
void func_0885CB50(CmdList *self, int id, int a, int b, int c)
{
    int n = self->count;
    self->entries[n].id = id;
    self->entries[n].a = a;
    self->entries[n].b = b;
    self->entries[n].c = c;
    self->count = n + 1;
}
