/* Stops the download's three handles and forgets the file. */
typedef struct DownloadState {
    char pad_0000[0x3DFC];
    int thread;
    int semaphore;
    int file;
    short slot;
} DownloadState;

extern DownloadState *D_089D8128;
extern void func_089C5FB0(int handle);
extern void func_089C5F98(int handle);

void func_088FFA60(void)
{
    func_089C5FB0(D_089D8128->file);
    D_089D8128->file = D_089D8128->slot = -1;
    func_089C5FB0(D_089D8128->thread);
    func_089C5F98(D_089D8128->semaphore);
}
