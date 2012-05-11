#ifdef WIN32

#include "wsock.h"

/* Workaround an invalid inlining management of MSYS when compiled in debug mode */
#ifdef CS_DEBUG

void nlFD_CLR(SOCKET fd, fd_set *set)
{
    u_int i;

    for(i=0;i<set->fd_count;i++)
    {
        if(set->fd_array[i] == fd)
        {
            while(i < set->fd_count-1)
            {
                set->fd_array[i] = set->fd_array[i+1];
                i++;
            }
            set->fd_count--;
            break;
        }
    }
}

void nlFD_SET(SOCKET fd, /*@out@*/ fd_set *set)
{
    if(set->fd_count < FD_SETSIZE)
        set->fd_array[set->fd_count++]=fd;
}

int nlWSAFDIsSet(SOCKET fd, fd_set *set)
{
    int i = (int)set->fd_count;

    while(i-- != 0)
    {
        if (set->fd_array[i] == fd)
            return 1;
    }
    return 0;
}

#endif // CS_DEBUG

#endif // WIN32
