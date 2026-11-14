/* Clears the global menu request block. */
typedef struct MenuRequest {
    int state;
    int value;
    int result;
    int pending;
} MenuRequest;

extern MenuRequest D_09B6D830;

void func_089E1BB0(void)
{
    D_09B6D830.state = 0;
    D_09B6D830.value = 0;
    D_09B6D830.result = -1;
    D_09B6D830.pending = 0;
}
