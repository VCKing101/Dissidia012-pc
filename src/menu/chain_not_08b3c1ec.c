/* Returns 1 when self->0xCC is null or its byte at 0x10 is zero or func_0889F35C(D_089D6C30) is zero, else 0. */
struct Rec { char pad[0x10]; unsigned char on; };
struct Self { char pad[0xCC]; struct Rec *rec; };
extern void *D_089D6C30;
extern int func_0889F35C(void *p);
int func_08B3C1EC(struct Self *self)
{
    if (self->rec == 0) { return 1; }
    else if (self->rec->on == 0) { return 1; }
    else if (func_0889F35C(D_089D6C30) == 0) { return 1; }
    else { return 0; }
}
