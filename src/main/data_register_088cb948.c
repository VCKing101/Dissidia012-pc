/* Registers the data with the heap once and links it. */
typedef struct DataLink088CB948 {
    void *first;
    void *second;
} DataLink088CB948;

extern int D_089D77F0;
extern DataLink088CB948 *D_089D77F4;
int func_088FE708(int heap, void *data, int size, int flags);
void func_088FE944(int heap, void *data, int handle);
void func_088CB84C(DataLink088CB948 *link, void *value);
void func_088CB948(void *data, int size)
{
    int handle;
    if (D_089D77F4->second != 0) {
        return;
    }
    handle = func_088FE708(D_089D77F0, data, size, 0);
    func_088FE944(D_089D77F0, data, handle);
    func_088CB84C(D_089D77F4, data);
}
