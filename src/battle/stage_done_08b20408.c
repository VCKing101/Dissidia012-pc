typedef struct StageHandle {
    int field_00;
    int state;      /* 0x04 */
    void *handle;   /* 0x08 */
} StageHandle;

extern void func_08B20260(StageHandle *stage, void *handle);

int func_08B20408(StageHandle *stage)
{
    if (stage->handle == 0) {
        return -1;
    }
    func_08B20260(stage, stage->handle);
    stage->handle = 0;
    stage->state = 3;
    return 0;
}
