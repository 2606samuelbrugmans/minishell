#include "minishell.h"

int	make_pipes(t_minishell minish)
{
	if (pipe(minish.fd_pipes)  == -1)
		return (perror("couldn't pipe"));
}
void command_after(int strt, t_minishell minish)
{
	int final_stuff;
	int index;

	index = 0;
	while (minish.parsed_string[strt] == '<' || minish.parsed_string[strt] == '>')
		strt++;
	while (minish.parsed_string[strt] == ' ')
		strt++;
	while (minish.parsed_string[strt] != ' ')
		strt++;
	final_stuff = strt;
	while (minish.parsed_string[final_stuff] != '|' || (inquotes % 2 == 0 && minish.parsed_string[final_stuff] != '|'))
		final_stuff++;
	minish.instru[pars].command = malloc((final_stuff - strt + 1) * sizeof(char));
	while (index + strt < final_stuff)
	{
		minish.instru[pars].command[index] = minish.parsed_string[strt + index]; 
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
int ft_str_join_special()
{

}
void set_up_redirection(t_minishell minish, char *direction, int type, int pars)
{
	if (direction == "from")
	{
		minish.instru[pars].number_files_from++;
		if (minish.instru[pars].number_files_from == 1)
			minish.instru[pars].redirection_from = malloc(1 * sizeof(int));
		else 
			minish.instru[pars].redirection_from
			= realloc(minish.instru[pars].redirection_from,
			minish.instru[pars].number_files_from * sizeof(int));
		minish.instru[pars].redirection_from[minish.instru[pars].number_files_from - 1]
		= type;
	}
	else
	{
		minish.instru[pars].number_files_to++;
		if (minish.instru[pars].number_files_to == 1)
			minish.instru[pars].redirection_to = malloc(1 * sizeof(int));
		else 
			minish.instru[pars].redirection_to
			= realloc(minish.instru[pars].redirection_to,
			minish.instru[pars].number_files_to * sizeof(int));
		minish.instru[pars].redirection_to[minish.instru[pars].number_files_to - 1]
		= type;
	}
}
void get_file_and_redirection(t_minishell minish, int **where, int *has_command, int pars)
{	
	if (minish.parsed_string[**where] == minish.parsed_string[**where +1])
	{
		if (minish.parsed_string[**where] == '<')
			set_up_redirection(minish, "from", 2, pars);
		else if (minish.parsed_string[**where] == '>')
			set_up_redirection(minish, "to", 2, pars);
	}
	else if (minish.parsed_string[**where] == '<')
		set_up_redirection(minish, "from", 1, pars);
	else if (minish.parsed_string[**where] == '>')
		set_up_redirection(minish, "to", 1, pars);
	**where = ft_str_join_special(minish, **where, minish.parsed_string[**where]);
}
void pre_init_command(t_minishell minish, int pars, int *where)
{
	int quotes;
	int has_command;

	/// we can deal with some parsing errors here
	has_command = 0;
	while (minish.parsed_string[*where])
	{
		if (minish.parsed_string[*where] == '"')
			quotes++;
		if (quotes % 2 == 0 && minish.parsed_string[*where] == '<'
		|| minish.parsed_string[*where] == '>')
			get_file_and_redirection(minish, &where, &has_command, pars);
		else if (quotes % 2 == 0 &&  minish.parsed_string[*where] != ' ')
			get_command(minish, &where, &has_command);
		if (minish.parsed_string[*where] == '|' && quotes % 2 == 0)
			break;
		// if we found all redirections and the command
		if (has_command == 2)
			break;
		if (*where == 0)
			return (printf("risked going below 0"));
		*where++
	}
}
its_a_FILE(char *string, int *index)
{
	/// skips stuff presumed to be files
	/// has to deal with quotes as delimiters or space which can be skipped if 
}
int count_commands(char *string)
{
	int index;
	int commands;
	int quotes;
	int found_stuff;
	int single_quote;

	single_quote = 0;
	quotes = 0;
	commands = 0;
	index = 0;
	while (string[index] != '\0')
	{
		/// unsure about the quotes within single quotes and vice versa
		if (string[index] == 39)
			single_quote++;
		else if (string[index] == '"')
			quotes += 1;
		else if ((string[index] == '>' || string[index] == '<')
				&& (quotes % 2 == 0 && single_quote % 2 == 0))
		{
			found_stuff = 0;
			its_a_FILE(string, &index);
		}
		else if (string[index] != ' ' && 
			found_stuff == 0 && quotes % 2 == 0 && single_quote % 2 == 0)
		{
			found_stuff = 1;
			commands++;
		}
		index++;
	}
	return (commands);
}
int find_the_end(char *string)
{
	int end;

	end = 0;
	while (string[end] != '\0')
		end++;
	return (end);
}
int initialise(t_minishell minish, char **envp, char *string)
{
	int pars;
	int where;

	where = find_the_end(string);
	minish.number_of_commands = count_commands(string);
	minish.instru = malloc((minish.number_of_commands + 1) * sizeof(t_instructions));
	pars = 0;
	minish.parsed_string = string;
	minish.envp = envp;
	while (pars < minish.number_of_commands)
	{
		pre_init_command(minish, &where, pars);
		pars++;
	}
	make_pipes(minish);
}

int main(char **envp)
{
	char 			*prompt;
	char 			*string;
	t_minishell		minish;

	minish = malloc(1 * sizeof(t_minishell));
	while (1)
	{
		string = readline(prompt);
		initialise(minish, envp, string);
		if (run(minish) != 0)
			break;

	}
	free_everything(minish);
}