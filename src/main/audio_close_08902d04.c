/* Closes the audio channel (if open) and shuts the audio library down. */
typedef struct AudioChannel {
    int handle;
} AudioChannel;

extern void func_089C5970(int handle);
extern void func_089C5988(void);
extern void func_089C59E8(void);

void func_08902D04(AudioChannel *ch)
{
    if (ch->handle != -1) {
        func_089C5970(ch->handle);
        ch->handle = -1;
    }
    func_089C5988();
    func_089C59E8();
}
