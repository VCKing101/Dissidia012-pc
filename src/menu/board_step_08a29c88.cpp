/* Runs the current step of a 5-step member-function table copied from .data. */
class BoardState {
public:
    char pad_0000[0x14FC];
    int field_14FC;
};

class BoardScreen {
public:
    char pad_000[0x138];
    BoardState *field_138;
    virtual void v0();
};

typedef void (BoardScreen::*BoardScreenStep)();
struct BoardScreenTable {
    BoardScreenStep steps[5];
};

extern "C" void func_08935524(void *dst, const void *src, int size);
extern "C" const BoardScreenTable D_08A2E930;

extern "C" {
void func_08A29C88(BoardScreen *self)
{
    BoardScreenTable table;
    func_08935524(&table, &D_08A2E930, sizeof(table));
    (self->*table.steps[self->field_138->field_14FC])();
}
}
