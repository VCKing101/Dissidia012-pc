typedef struct SendOption {
    signed char value;
} SendOption;

extern int func_08900858(void *session, void *data, SendOption option);

int func_0890117C(void *session, void *data)
{
    SendOption option;

    option.value = 0;
    return func_08900858(session, data, option);
}
