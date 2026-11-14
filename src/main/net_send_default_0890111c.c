/* Sends with the default (zero) option byte. */
typedef struct SendOption {
    signed char value;
} SendOption;

extern int func_08900630(void *session, void *data, SendOption option);

int func_0890111C(void *session, void *data)
{
    SendOption option;

    option.value = 0;
    return func_08900630(session, data, option);
}
