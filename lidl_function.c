#include "minishell.h"
int not_quoted(t_minishell minish)
{
    if (minish.quote % 2 == 0 && minish.doublequote % 2 == 0)
        return (0);
    return (-1);
}
