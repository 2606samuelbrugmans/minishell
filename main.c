#include "minishell.h"

int	make_pipes(t_minishell minish)
{
	minish.fd_pipes[minish.number_of_commands];
	
}
void command_after(int strt, char *string, t_minishell minish)
{
	int final_stuff;
	int index;

	index = 0;
	while (string[strt] == '<' || string[strt] == '>')
		strt++;
	while (string[strt] == ' ')
		strt++;
	while (string[strt] != ' ')
		strt++;
	final_stuff = strt;
	while (string[final_stuff] != '|' || (inquotes % 2 == 0 && string[final_stuff] != '|'))
		final_stuff++;
	minish.instru[pars].command = malloc((final_stuff - strt + 1) * sizeof(char));
	while (index + strt < final_stuff)
	{
		minish.instru[pars].command[index] = string[strt + index]; 
		index++;
	}
	minish.instru[pars].command[index] = '\0';
}

void find_command(int location, int redirection_index, char *string, t_minishell minish)
{
	int index;

	index = 0;
	minish.instru[pars].command = malloc((redirection_index - location + 1) * sizeof(char));
	while (index + location < redirection_index)
	{
		minish.instru[pars].command[index] = string[location + index]; 
		index++;
	}
	minish.instru[pars].command[index] = '\0';
}
int	after_or_before(char *string, int location, int redirection_index)
{
	int before;

	before = -1;
	if (redirection_index == -1)
		return -1;
	while (location < redirection_index)
	{
		if (string[location] != ' ')
			before = 0;

	}
}
char *make_command(t_minishell minish,char *string, int location, int redirection_index)
{
	int where;

	where = after_or_before(string, location, redirection_index);
	if (where == 0)
		find_command(location, redirection_index, string, minish);
	else
		command_after(location, redirection_index, string, minish)
}

int locate(char *string, int location)
{
	int inquotes;

	inquotes = 0;
	while (string[location] != '\0' || (string[location] != '|' && inquotes % 2 == 0))
	{
		if (string[location] == '"')
			inquotes++;
		if (string[location] == '<' || string[location] == '>')
			return (location);
		location++;
	}
	return (-1);
}

void pre_init_command(t_minishell minish, int *location, char *string, int pars)
{
	int redirection_index;
	int redirection_type;

	redirection_index = locate(string, *location);
	if (redirection_index == -1)
		minish.instru[pars].redirection_type = 0;
	else if (string[redirection_index] == '>' && string[redirection_index + 1] == '>')
		minish.instru[pars].redirection_type = 4;
	else if  (string[redirection_index] == '>' && string[redirection_index + 1] == ' ')
		minish.instru[pars].redirection_type = 3;
	else if  (string[redirection_index] == '<' && string[redirection_index + 1] == '<')
		minish.instru[pars].redirection_type = 2;
	else if  (string[redirection_index] == '<' && string[redirection_index + 1] == ' ')
		minish.instru[pars].redirection_type = 1;
	make_command(minish, string, *location, redirection_index);
	make_path(minish);
	while (string[*location] != '|' || string[*location] != '\0')
		*location++;
}

int count_commands(char *string)
{
	int index;
	int pipes;
	int quotes;

	quotes = 0;
	pipes = 0;
	index = 0;
	while (string[index] != '\0')
	{
		/// unsure about the quotes
		if (string[index] == '"')
			quotes += 1;
		if (string[index] == '|' && quotes % 2 == 0)
			pipes +=1;
		index++;
	}
	return (pipes);
}
int initialise(t_minishell minish, char **envp, char *string)
{
	int pars;
	int where;

	where = 0;
	pars = 0;
	minish.number_of_commands = count_commands(string) + 1;
	minish.instru = ((minish.number_of_commands + 1) * sizeof(t_instructions));
	while (pars <= minish.number_of_commands )
	{
		pre_init_command(minish, &where, string, pars);
		pars++;
	}
	make_pipes(minish);

}

int main(char **envp)
{
	char 			*prompt;
	char 			*string;
	t_minishell		minish;

	while (1)
	{
		string = readline(prompt);
		initialise(minish, envp, string);
		if (run(minish) != 0)
			break;

	}
	free_everything(minish);
}