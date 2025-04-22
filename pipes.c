#include "minishell.h"
void close_stuff(t_minishell *minish)
{
	int i;

	i = 0;
	while (i <= minish->number_of_commands )
	{
		close(minish->fd_pipes[i][1]);
        close(minish->fd_pipes[i][0]);
        i++;
	}
}
void    close_nested_stuff(t_minishell *minish)
{
    int i;

    i = 0;
	while (i <= minish->nested_width )
	{
		close(minish->pipe_nested[minish->nested[0]][i][1]);
        close(minish->pipe_nested[minish->nested[0]][i][0]);
        i++;
	}
}