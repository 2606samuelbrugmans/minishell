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

int	get_Command(t_minishell *minish, int location, int *has_command, int pars)
{
	// return value can be an error
	int	index;
	char *arg;

	index = 0;
	*has_command = 2;
	while (within_executable(minish, location + index))
	{
		if (!not_quoted(minish))
			location =(get_quoted_command(minish, location, index, pars));
		index++;
	}
	minish->instru[pars].command = get_string(minish, location, index);
	make_executable(minish);
	location += index;
	index = 0;
	while (within_executable(minish, location + index))
	{
		while (minish->parsed_string[location] == ' ')
			location++;
		while (minish->parsed_string[location] != ' ')
		{
			if (minish->parsed_string[location] == '"' || minish->parsed_string[location] == 39)
				index = (get_quoted_command(minish, location, index, pars));
			index++;
		}
		arg = get_string(minish, location, index);
		complete_executable(minish, arg);
		location += index;
		index = 0;
	}
	return (location + index);
}

int locate(char *string, int location)
{
	int inquotes;

	inquotes = 0;
	while (string[location] != '\0' || (string[location] != '|'  ))
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
void set_up_redirection(t_minishell *minish, char *direction, int type, int pars)
{
	if (direction == "from")
	{
		minish->instru[pars].number_files_from++;
		if (minish->instru[pars].number_files_from == 1)
			minish->instru[pars].redirection_from = malloc(1 * sizeof(int));
		else 
			minish->instru[pars].redirection_from
			= realloc(minish->instru[pars].redirection_from,
				minish->instru[pars].number_files_from * sizeof(int));
		minish->instru[pars].redirection_from[minish->instru[pars].number_files_from - 1]
		= type;
	}
	else
	{
		minish->instru[pars].number_files_to++;
		if (minish->instru[pars].number_files_to == 1)
			minish->instru[pars].redirection_to = malloc(1 * sizeof(int));
		else 
			minish->instru[pars].redirection_to
			= realloc(minish->instru[pars].redirection_to,
			minish->instru[pars].number_files_to * sizeof(int));
		minish->instru[pars].redirection_to[minish->instru[pars].number_files_to - 1]
		= type;
	}
}
void get_file_and_redirection(t_minishell *minish, int **where, int *has_command, int pars)
{	
	if (minish->parsed_string[**where] == minish->parsed_string[**where +1])
	{
		if (minish->parsed_string[**where] == '<')
			set_up_redirection(minish, "from", 2, pars);
		else if (minish->parsed_string[**where] == '>')
			set_up_redirection(minish, "to", 2, pars);
	}
	else if (minish->parsed_string[**where] == '<')
		set_up_redirection(minish, "from", 1, pars);
	else if (minish->parsed_string[**where] == '>')
		set_up_redirection(minish, "to", 1, pars);
	**where = ft_str_join_special(minish, **where, minish->parsed_string[**where]);
}
void pre_init_command(t_minishell *minish, int pars, int *where)
{
	int has_command;

	/// we can deal with some parsing errors here
	has_command = 0;
	while (minish->parsed_string[*where])
	{
		if (minish->parsed_string[*where] == '"')
			minish->doublequote++;
		else if (minish->parsed_string[*where] == 39)
			minish->quote++;
		if (redirection(minish))
			get_file_and_redirection(minish, &where, &has_command, pars);
		else if (minish->parsed_string[*where] != ' ')
			get_command(minish, *where, &has_command, pars);
		if (minish->parsed_string[*where] == '|' && not_quoted(minish))
			break;
		// if we found all redirections and the command
		if (has_command == 2)
			break;
		*where++
	}
}
int its_a_FILE(t_minishell minish, int index, char c)
{
	/// A SINGLE QUOTE IN THIS CASE IS A PARSING ERROR ACCORDING TO CHATGPT
	/// I DON4T HAVE BASH AT HOME SORRY
	while (minish.parsed_string[index] == c)
		index++;
	while (minish.parsed_string[index] == ' ')
		index++;
	while (minish.parsed_string[index] != '|' && minish.parsed_string[index] != '\0')
	{
		if (minish.parsed_string[index] == 39)
			return (lookfor_single_quote(minish, index));
		if (minish.parsed_string[index] == '"')
			return (lookfor_double_quote(minish, index));
		if (minish.parsed_string[index] == ' ')
			break;
		index++;
	}
	return (index);
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
		else if ((redirection(minish))
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
int initialise(t_minishell *minish, char **envp, char *string)
{
	int pars;
	int where;

	where = find_the_end(string);
	minish->parsed_string = string;
	minish->doublequote = 0;
	minish->quote = 0;
	minish->number_of_commands = count_commands(*minish);
	minish->instru = malloc((minish->number_of_commands + 1) * sizeof(t_instructions));
	pars = 0;
	minish->envp = envp;
	while (pars < minish->number_of_commands)
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
	t_minishell		*minish;

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