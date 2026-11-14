extern "C" {
typedef struct CardSlotRec { char data[0x44]; } CardSlotRec;
typedef struct CardScreen { char pad_00[0x28]; int mode; char pad_2C[0x18]; CardSlotRec slots[5]; int current; } CardScreen;
extern void func_0892B164(CardSlotRec *slot);
extern void func_0892C390(CardScreen *s);
void func_0892BD10(CardScreen *s)
{
    s->mode = 0;
    func_0892C390(s);
    for (int i = 0; i < 5; i++) {
        func_0892B164(&s->slots[i]);
    }
    s->current = 0;
}
}
