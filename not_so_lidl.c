#include "minishell.h"

void skip_quotes(const char *str, int base_index, int *offset_index)
{
    int i;

    if (!str || !offset_index)
        return ;
    i = base_index + *offset_index;
    if (str[i] != '"' && str[i] != '\'')
        return ;
    i++; // Skip opening quote
    while (str[i] != '\0' && str[i] != str[base_index + *offset_index])
    {
        if (str[i] == '\\' && str[i + 1] != '\0')
            i++; // Skip escaped character
        i++;
    }
    if (str[i] == str[base_index + *offset_index])
        i++; // Skip closing quote
    *offset_index = i - base_index;
}
int	find_start(const char *str, int i, char c)
{
	while (str[i] == c)
		i++;
	while (str[i] != '\0')
	{
		if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
			return (i);
		i++;
	}
	return (-1);
}


int	get_string(t_minishell *minish, int where, int pars, char direction)
{
	int		start;
	int		end;
	char	quote;
	char	*str;

	if (direction == '>' || direction == '<')
	{
		start = find_start(minish->parsed_string, where, minish->parsed_string[where]);
		printf("\nstart is %d\n", start);
	}
	else 
		start = where;
	quote = -1;
	where = start;
	if (minish->parsed_string[where] == '\'' || minish->parsed_string[where] == '"')
		quote = minish->parsed_string[where];
	end = find_end_index(minish->parsed_string, where, &quote);
	str = remove_quote(ft_substr(minish->parsed_string, where, end), quote);
	write(1, "\n", 2);
	write(1, &direction, 1);
	write(1, "\n", 2);
	store(minish, pars, str, direction);
	if (minish->parsed_string[end] != '\0')
		return (end + 1);
	return (end);
}
int number_strings(char **array)
{
    int count;

	count = 0;
    while (array[count] != NULL)
        count++;
    return count;
}

void	store(t_minishell *minish, int pars,
	char *filename, char direction)
{
	int n;

	n = 0;
	if (!filename)
		return ;
	if (direction == '>')
		minish->instru[pars].to_file_str[minish->instru->number_files_to - 1] = filename;
	else if (direction == '<')
		minish->instru[pars].from_file_str[minish->instru->number_files_from - 1] = filename;
	else if (direction == 'c')
		minish->instru[pars].command = filename;
	if (direction == 'e' || direction == 'c')
	{
		printf(" %c",direction);
		if (direction == 'c')
			minish->instru[pars].executable = malloc(2 * sizeof(char *));
		else
		{
			n = number_strings(minish->instru[pars].executable);
			minish->instru[pars].executable = realloc(minish->instru[pars].executable, (n + 2) * sizeof(char *));
		}
		minish->instru[pars].executable[n] = filename;
		minish->instru[pars].executable[n + 1] = NULL;
	}
	write(1, filename, ft_strlen(filename));
}
char	*remove_quote(char *string, char quote)
{
	char *new;
	int new_size;
	int index;
	int other_index;

	if (quote == -1)
		return (string);
	index = 0;
	new_size = ft_strlen(string);
	new_size -= count_quote(string, quote);
	new = malloc((new_size + 1) * sizeof(char));
	other_index = 0;
	if (!new)
		return (NULL);
	while (string[index] != '\0')
	{
		if (string[index] != quote)
		{
			new[other_index] = string[index];
			other_index++;
		}
		index++;
	}
	new[other_index] = '\0';
	return (new);
}