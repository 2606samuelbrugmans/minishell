#include "minishell.h"
/*
int	make_pipes(t_minishell *minish, int commands)
{
	if (minish->pipes_already_found == 0)
	{
		minish->fd_pipes = malloc(1 * sizeof(int[2]));
		minish->pipe_location = malloc(1 * sizeof(int));
	}
	else 
	{
		minish->fd_pipes = realloc(minish->fd_pipes,
			 (1 + minish->pipes_already_found)* sizeof(int[2]));
		minish->pipe_location = realloc(minish->pipe_location,
			 (1 + minish->pipes_already_found) * sizeof(int));
	}
	minish->pipe_location[minish->pipes_already_found] = commands;
	minish->pipes_already_found++;
}
int	make_pipes(t_minishell *minish, int commands)
{
	minish->fd_pipes = malloc(minish->number_of_commands * sizeof(int[2]));
	if (!minish->fd_pipes)
		return (-1);
}
*/
int	get_Command(t_minishell *minish, int location, int *has_command, int pars)
{
	// if hascommand = 0 make the executable and the command if not
	int	index;

	index = 0;
	if (*has_command == 0)
		index = get_string(minish, location, pars,'c');
	else 
		index = get_string(minish, location, pars, 'e');
	*has_command = 1;
	return (index);
}

void	set_up_redirection(t_minishell *minish, char direction, int type, int pars)
{
	if (direction == '<')
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
int get_file_and_redirection(t_minishell *minish, int where, int pars)
{

	if (minish->parsed_string[where] == minish->parsed_string[where +1])
	{
		if (minish->parsed_string[where] == '<')
			set_up_redirection(minish, '<', 2, pars);
		else if (minish->parsed_string[where] == '>')
			set_up_redirection(minish, '>', 2, pars);
	}
	else if (minish->parsed_string[where] == '<')
		set_up_redirection(minish, '<', 1, pars);
	else if (minish->parsed_string[where] == '>')
		set_up_redirection(minish, '>', 1, pars);
	where = get_string(minish, where, pars, minish->parsed_string[where]);
	
	return (where);
}
void pre_init_command(t_minishell *minish, int pars, int *where)
{
	int has_command;

	/// we can deal with some parsing errors here
	has_command = 0;
	while (minish->parsed_string[*where])
	{
		if (redirection(minish, *where))
			(*where) = get_file_and_redirection(minish, *where, pars);
		else if (minish->parsed_string[*where] != ' ')
			(*where) = get_Command(minish, *where, &has_command, pars);
		if ((minish->parsed_string[*where] == '|'))
			break;
		(*where)++;
	}
}
/*
int its_a_FILE(t_minishell minish, int index, char c)
{
	/// A SINGLE QUOTE IN THIS CASE IS A PARSING ERROR ACCORDING TO CHATGPT
	/// I DON4T HAVE BASH AT HOME SORRY
	while (minish.parsed_string[index] == 'c')
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
*/
/*
int skip_nested_command(char *parsed_string, int index)
{
	int parentheses;
	int index_two;

	index_two = 0;
	parentheses = 1;
	while (parsed_string[index + index_two] != '\0')
	{
		skip_quotes(parsed_string, index, &index_two);
		if (parsed_string[index + index_two] == '(')
			parentheses++;
		if (parsed_string[index + index_two] == ')')
			parentheses--;
			index_two++;
		if (parentheses == 0)
			break;
	}
	if (type && (*type == 0 || type == 1))
		make_executable(minish, index, index_two, parser);
	return (index + index_two);
}
*/
int count_commands(t_minishell *minish)
{
	int index;
	int commands;

	commands = 0;
	index = 0;
	while (minish->parsed_string[index] != '\0')
	{
		/*
		if (minish->parsed_string[index] == '$' && minish->parsed_string[(index) + 1] == '('
		&& minish->quote % 2 == 0)
			index = skip_nested_command(minish, index + 2);
		*/
		if (not_quoted(minish) && minish->parsed_string[index] == '|')
			commands++;
		if (minish->doublequote % 2 == 0 && minish->parsed_string[index] == '\'')
			minish->quote++;
		if (minish->quote % 2 == 0 && minish->parsed_string[index] == '\"')
			minish->doublequote++;
		index++;
	}
	minish->fd_pipes = malloc(commands * sizeof(int[2]));
	return (commands + 1);
}

/*
int count_commands(t_minishell *minish)
{
	int index;
	int commands;
	int found_stuff;

	commands = 0;
	index = 0;
	while (minish->parsed_string[index] != '\0')
	{
		/// unsure about the quotes within single quotes and vice versa
		if (minish->parsed_string[index] == 39 && minish->doublequote % 2 == 0)
			minish->quote++;
		if (minish->parsed_string[index] == '"' && minish->quote % 2 == 0)
		minish->doublequote++;
		else if (redirection(minish))
		{
			found_stuff = 0;
			its_a_FILE(*minish, &index, minish->parsed_string[index]);
		}
		// it's gonna be a problem if there's no command after
		else if (minish->parsed_string[index] == '|' && not_quoted(minish))
			make_pipes(minish, commands);
		else if (minish->doublequote != ' ' && 
			found_stuff == 0 && not_quoted(minish))
		{
			found_stuff = 1;
			commands++;
		}
		index++;
	}
	minish->doublequote = 0;
	minish->quote = 0;
	return (commands);
	
}
*/
int initialise(t_minishell *minish, char *string)
{
	int pars;
	int where;

	minish->parsed_string = string;
	minish->doublequote = 0;
	minish->quote = 0;
	minish->number_of_commands = count_commands(minish);
	minish->instru = malloc((minish->number_of_commands) * sizeof(t_instructions));
	pars = 0;
	where = 0;
	while (pars < minish->number_of_commands)
	{
		pre_init_command(minish, pars, &where);
		pars++;
	}
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	char 			*prompt;
	char 			*string;
	t_minishell		*minish;

	if (argc != 1 || !argv)
		return (-1);
	prompt = ft_strdup("");
	minish = malloc(1 * sizeof(t_minishell));
	while (1)
	{
		minish->envp = envp;
		string = readline(prompt);
		/*
		if (parsing_errors(minish) != 0)
			continue;
		*/
		initialise(minish, string);
		run(minish);
	}
	/*
	free_everything(minish);
	*/
	return (0);
}