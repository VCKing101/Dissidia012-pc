extern "C" {
typedef struct CardSlotRec { char data[0x44]; } CardSlotRec;
typedef struct CardScreen { char head[0x44]; CardSlotRec slots[5]; } CardScreen;
extern void func_0892ACAC(CardScreen *s);
extern void func_0892B164(CardSlotRec *slot);
extern void func_0892C390(CardScreen *s);
extern void func_0892C33C(CardScreen *s);
void func_0892BA64(CardScreen *s)
{
    func_0892ACAC(s);
    for (int i = 0; i < 5; i++) {
        func_0892B164(&s->slots[i]);
    }
    func_0892C390(s);
    func_0892C33C(s);
}
}
