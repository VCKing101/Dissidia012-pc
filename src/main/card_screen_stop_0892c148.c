typedef struct CardSlotRec { char data[0x44]; } CardSlotRec;
typedef struct CardScreen { char head[0x44]; CardSlotRec slots[5]; } CardScreen;
extern void func_0892B294(CardSlotRec *slot, float time);
void func_0892C148(CardScreen *s)
{
    int i;
    for (i = 0; i < 5; i++) {
        func_0892B294(&s->slots[i], 0.0f);
    }
}
