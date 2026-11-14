typedef struct StreamSub {
    char data[0x100];
} StreamSub;

typedef struct StreamOwner {
    char pad_000[0x100];
    StreamSub field_100;
} StreamOwner;

extern unsigned char D_089D5F1F;
extern int D_089E09C0;
extern void func_089DE910(int handle, int value);
extern void func_089DE94C(int handle);
extern void func_0886503C(StreamSub *sub, void (*first)(void), void (*second)(void), int flags);
extern void func_088650A4(StreamSub *sub);
extern void func_0886504C(StreamSub *sub, StreamOwner *owner, int size);
extern void func_089E0B7C(void);
extern void func_089E0BBC(void);

void func_089E0BDC(StreamOwner *self)
{
    if (D_089D5F1F != 0) {
        func_089DE910(D_089E09C0, 0);
        func_089DE94C(D_089E09C0);
    } else {
        StreamSub *sub = &self->field_100;
        func_0886503C(sub, func_089E0B7C, func_089E0BBC, 0);
        func_088650A4(sub);
        func_0886504C(sub, self, 0x100);
    }
}
