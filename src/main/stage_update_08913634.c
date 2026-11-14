extern int func_088EB17C(void *lock);
extern void func_088EB65C(void *lock);
extern void func_08914C54(void *stage);
extern void func_08916F38(void);

typedef struct Stage {
    char pad_000[0x68];
    int state;                  /* 0x68 */
    float timer;                /* 0x6C */
    char pad_070[0x720 - 0x70];
    char lock[4];               /* 0x720 */
} Stage;

void func_08913634(Stage *stage)
{
    if (stage->state == 5) {
        if (stage->timer <= 2.0f) {
            return;
        }
        if (func_088EB17C(stage->lock) != 0) {
            func_08914C54(stage);
            func_08916F38();
        }
        func_088EB65C(stage->lock);
    }
}
