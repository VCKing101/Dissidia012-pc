typedef struct DownloadState {
    char pad_0000[0x3E08];
    short slot;
} DownloadState;

extern DownloadState *D_089D8128;

void func_088FFB2C(int slot)
{
    D_089D8128->slot = slot;
}
