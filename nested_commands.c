#include "minishell.h"

int pipe_nested(t_minishell *minish, int length)
{
	if (minish->nested == 0)
		minish->pipe_nested = malloc(1 * sizeof(minish->pipe_nested));
	else 
		minish->pipe_nested = realloc(minish->pipe_nested,
		(minish->nested[0] + 1) * sizeof(minish->pipe_nested));
	return (0);
}
int **check_for_nest(t_minishell *minish, int *args_containing, int parser, int *num_nests)
{
	int **nests;


}
void nested(t_minishell *minish, int parser)
{
	//// where_nest first element is the number of substittion commands
	//// second element is which arg its tied to
	//// third element is the first index if only one it4s 0 
	/// fourth element is the  end of such a substitution command
	/// rinse and repeat until none are left
	int **where_nest;
	int i;
	ssize_t bytes;
	int length;
	int	*num_nests;
	char *new_arg;

	where_nest = check_for_nest(minish, &length, parser, &num_nests);
	i = 0;
	pipe_nested(minish, length);
	while (i < length)
	{
		if (pipe(minish->pipe_nested[minish->nested[0]][i]) == -1 )
			perrorr("bablda");
		if (where_nest[i][1] == -1)
			initialise(minish, minish->instru[parser].executable[where_nest[i][0]],
			(int[2]){minish->nested[0] + 1, i});
		i++;
	}

	while (i < length)
	{
		bytes = read(minish->pipe_nested[minish->nested[0]][i][0],
		minish->instru[parser].executable[where_nest[i]], 
		sizeof(minish->instru[parser].executable[where_nest[i]]) - 1);
		if (bytes > 0)
			minish->instru[parser].executable[where_nest[i]][bytes] = '\0';
		i++;
	}
	minish->nested_width = length;
}