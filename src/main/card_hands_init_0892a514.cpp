extern "C" {
typedef struct CardHand { char data[0x154]; } CardHand;
typedef struct CardHands { CardHand hands[5]; int selected; } CardHands;
extern void func_0892A13C(CardHand *hand);
void func_0892A514(CardHands *h)
{
    for (int i = 0; i < 5; i++) {
        func_0892A13C(&h->hands[i]);
    }
    h->selected = -1;
}
}
