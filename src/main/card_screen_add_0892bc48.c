typedef struct CardSlotRec { char data[0x44]; } CardSlotRec;
typedef struct CardScreen { char pad_00[0x2C]; int f2C; int f30; int f34; int f38; int f3C; char pad_40[4]; CardSlotRec slots[5]; int current; } CardScreen;
typedef struct CardSlotDef { unsigned short id; char rest[10]; } CardSlotDef;
extern CardSlotDef D_089CECE0[];
extern void func_0892B084(CardSlotRec *slot, int kind, int a, int b);
extern void func_0892B238(CardSlotRec *slot, int unused, int a, int b, int d, int e);
void func_0892BC48(CardScreen *s, int kind, int a, int b)
{
    func_0892B084(&s->slots[s->current], kind, a, b);
    func_0892B238(&s->slots[s->current], s->f2C, s->f34, s->f30, s->f3C, D_089CECE0[(unsigned int)s->current].id);
    s->current++;
}
