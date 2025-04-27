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
int **check_for_nest(t_minishell *minish, int parser)
{
	int **nests;
	int	x;
	int index;
	int y;
	int c;

	minish->doublequote = 0;
	minish->quote = 0;
	index = 0;
	x = 0;
	y = 0;
	while (minish->instru[parser].executable[x] != '\0')
	{
		while (minish->instru[parser].executable[x][y] != '\0')
		{
			if (minish->instru[parser].executable[x][y] == '$' 
			&& minish->instru[parser].executable[x][y] == '(' && not_quoted(minish))
			{
				c = skip_nested_command(minish->instru[parser].executable[x], y +2);
				index += adjust_nests(x, y + 2, c-1, nests);
				c = y;
			}
			else 
				y++;
			seek_quote(minish);
			x++;
		}
		x++;
	}
	return (nests);
}
char ***get_nested_strings(t_minishell *minish,int parser, char ***string, int **where_nest)
{
	int i;
	int j;
	int y;

	i = 1;
	string = malloc((where_nest[0][1]) *sizeof(char **));
	while (i < where_nest[0][1])
	{
		y = 3;
		j = 0;
		string[i] = malloc((where_nest[i][1] / 2) *sizeof(char *));
		while (j < where_nest[i][1] / 2)
		{
			string[i][j] = malloc((where_nest[i][y] - where_nest[i][y - 1] + 1) * sizeof(char));
			string[i][j] = ft_substr(minish->instru[parser].executable[where_nest[i][0]]
				,where_nest[i][y - 1], where_nest[i][y] - where_nest[i][y - 1]);
			y+=2;
			j++;
		}
		
	}
	
	return (string);
}
void nested(t_minishell *minish, int parser)
{
	//// where_nest [0][0] is the number of substuttion commands
	//// where_nest [0][1] is the number of strings in executable wich have a substitution commands so how big (x - 1) can get also
	//// 
	//// where_nest [x][0] except for x = 0 is which stirng in  char	**executable the following nest is tied to
	//// where_nest [x][1] the number of nested commands within the executable thus the size of the array at x / 2
	//
	/// where_nest at [x][2] is the starting index of the nested command in the executable
	/// where_nest at [x][3] is the ending index of the nested command in the executable
	/// where_nest at [x][y] from now on is only starting and ending index
	/// fourth element is the  end of such a substitution command
	/// rinse and repeat until none are left
	int 	**where_nest;
	char	***string;
	int i;
	ssize_t bytes;
	char *buffer;
	
	where_nest = check_for_nest(minish, parser);
	string = get_nested_strings(minish, parser, string);
	pipe_nested(minish, where_nest[0][0]);
	i = 0;
	while (i < where_nest[0][0])
	{
		if (pipe(minish->pipe_nested[minish->nested[0]][i]) == -1 )
			perrorr("bablda");
		if (where_nest[i][1] == -1)
			initialise(minish, minish->instru[parser].executable[where_nest[i][0]],
			(int[2]){minish->nested[0] + 1, i});
		i++;
	}
	while (i < where_nest[0][0])
	{
		bytes = read(minish->pipe_nested[minish->nested[0]][i][0],
		minish->instru[parser].executable[where_nest[i]], 
		sizeof(minish->instru[parser].executable[where_nest[i]]) - 1);
		if (bytes > 0)
			minish->instru[parser].executable[where_nest[i]][bytes] = '\0';
		i++;
	}
	minish->nested_width = where_nest[0][0];
}