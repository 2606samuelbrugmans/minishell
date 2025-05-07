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
int	find_start(const char *str, int i)
{
	char c;

	c = str[i];

	while (str[i] == 'c')
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
		start = find_start(minish->parsed_string, where);
	quote = 0;
	if (minish->parsed_string[where] == '\'' || minish->parsed_string[where] == '"')
		quote = minish->parsed_string[where];
	start = where;
	end = find_end_index(minish->parsed_string, where, quote);
	str = ft_substr(minish->parsed_string, start, end - start);
	store(minish, pars, str, direction);
	if (quote && minish->parsed_string[end] == quote)
		end++;
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
	t_instructions	*instr;
	int n;

	n = 0;
	instr = &minish->instru[pars];
	if (!filename)
		return ;
	if (direction == '>')
		instr->to_file_str[instr->number_files_to - 1] = filename;
	else if (direction == '<')
		instr->from_file_str[instr->number_files_from - 1] = filename;
	else if (direction == 'c')
		instr->command = filename;
	if (direction == 'e' || direction == 'c')
	{
		if (direction == 'c')
			instr->executable = malloc(2 * sizeof(char *));
		else
		{
			n = number_strings(instr->executable);
			realloc(instr->executable, (n + 2) * sizeof(char *));
			instr->executable[n] = filename;
			instr->executable[n + 1] = '\0';
		}
	}
}
