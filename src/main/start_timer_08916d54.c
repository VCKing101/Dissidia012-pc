// Starts the owner's timer if it is not running, then sets the stored duration and the active flag.
struct Own { char d[0xBA0]; char timer[0xC]; unsigned char running; };
struct S { char d0[0x6D0]; struct Own *own; char d1[0xFC]; float dur; char d2[0xC]; unsigned char active; };
extern float D_089CE2A8;
extern void func_08AC5BF8(void *t);
void func_08916D54(struct S *s)
{
    if (s->own->running == 0)
        func_08AC5BF8(s->own->timer);
    s->dur = D_089CE2A8;
    s->active = 1;
}
