
int* _errno()
{
    static int real_errno = 0;
    return &real_errno;
}