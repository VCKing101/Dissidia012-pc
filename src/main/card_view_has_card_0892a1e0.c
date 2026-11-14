typedef struct CardView { char pad_000[0x150]; void *card; } CardView;
int func_0892A1E0(CardView *view)
{
    if (view->card == 0) {
        return 0;
    } else {
        return 1;
    }
}
