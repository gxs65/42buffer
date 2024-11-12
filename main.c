#include "get_next_line.h"

int main(int ac, char **av)
{
    int     ind;
    int     fd;
    char    *line;

    fd = open(av[1], O_RDONLY);
    ind = 0;
    line = get_next_line(fd);
    while (line)
    {
        printf("Ligne no %d : %s\n", ind, line);
        line = get_next_line(fd);
        ind++;
    }
}