struct Rec254D0 {
    unsigned short value;
    char pad_02[14];
};

struct Work254D0 {
    char pad_0000[0x1EA0];
    Rec254D0 recs[19];
    char pad_1FD0[4];
    short order[22];
    int cursor;
    char pad_2004[0x2440 - 0x2004];
    unsigned short current;
    char pad_2442[2];
    int result_2444;
    int result_2448;
};

struct Screen254D0 {
    char pad_0000[0x28F0];
    Work254D0 *work;
};

extern "C" {
extern int func_08A25574(Screen254D0 *self, unsigned short id);
extern int func_08A255AC(Screen254D0 *self, unsigned short id, int flag);
}

extern "C" int func_08A254D0(Screen254D0 *self)
{
    self->work->current = self->work->recs[self->work->order[self->work->cursor]].value;
    self->work->result_2448 = func_08A25574(self, self->work->current);
    self->work->result_2444 = func_08A255AC(self, self->work->current, 0);
    return self->work->result_2444;
}
