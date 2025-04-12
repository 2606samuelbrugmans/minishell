#include "minishell.h"

int	make_pipes(t_minishell minish, int commands)
{
	if (minish.pipes_already_found == 0)
	{
		minish.fd_pipes = malloc(1 * sizeof(int[2]));
		minish.pipe_location = malloc(1 * sizeof(int));
	}
	else 
	{
		minish.fd_pipes = realloc(minish.fd_pipes,
			 (1 + minish.pipes_already_found)* sizeof(int[2]));
		minish.pipe_location = realloc(minish.pipe_location,
			 (1 + minish.pipes_already_found) * sizeof(int));
	}
	minish.pipe_location[minish.pipes_already_found] = commands;
	minish.pipes_already_found++;
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
// // pipe is after or before gotta make an array
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
void its_a_FILE(t_minishell minish, int *index, char c)
{
	/// A SINGLE QUOTE IN THIS CASE IS A PARSING ERROR ACCORDING TO CHATGPT
	/// I DON4T HAVE BASH AT HOME SORRY
	while (minish.parsed_string[*index] == c)
		*index++;
	while (minish.parsed_string[*index] == ' ')
		*index++;
	while (minish.parsed_string[*index] != '|' && minish.parsed_string[*index] != '\0')
	{
		if (minish.parsed_string[*index] == 39)
			lookfor_single_quote(minish, &index);
		if (minish.parsed_string[*index] == '"')
			lookfor_double_quote(minish, &index);
		if (minish.parsed_string[*index] == ' ')
			break;
		*index++;
	}
	
}
int count_commands(t_minishell minish)
{
	int index;
	int commands;
	int found_stuff;

	commands = 0;
	index = 0;
	while (minish.parsed_string[index] != '\0')
	{
		/// unsure about the quotes within single quotes and vice versa
		if (minish.parsed_string[index] == 39 && minish.doublequote % 2 == 0)
			minish.quote++;
		if (minish.parsed_string[index] == '"' && minish.quote % 2 == 0)
			minish.doublequote++;
		else if ((minish.parsed_string[index] == '>' || minish.parsed_string[index] == '<')
				&& not_quoted(minish))
		{
			found_stuff = 0;
			its_a_FILE(minish, &index, minish.parsed_string[index]);
		}
		// it's gonna be a problem if there's no command after
		else if (minish.parsed_string[index] == '|' && not_quoted(minish))
			make_pipes(minish, commands);
		else if (minish.doublequote != ' ' && 
			found_stuff == 0 && not_quoted(minish))
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
	minish.parsed_string = string;
	minish.number_of_commands = count_commands(minish);
	minish.instru = malloc((minish.number_of_commands + 1) * sizeof(t_instructions));
	pars = 0;
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