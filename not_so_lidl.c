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
int	get_string(t_minishell *minish, int where, int pars, char direction)
{
	int		start;
	int		end;
	char	quote;
	char	*filename;

	where = skip_spaces(minish->parsed_string, where);
	quote = 0;
	if (minish->parsed_string[where] == '\'' || minish->parsed_string[where] == '"')
		quote = minish->parsed_string[where++];
	start = where;
	end = find_end_index(minish->parsed_string, where, quote);
	filename = ft_substr(minish->parsed_string, start, end - start);
	store_filename(minish, pars, filename, direction);
	if (quote && minish->parsed_string[end] == quote)
		end++;
	return (end);
}

void	store_filename(t_minishell *minish, int pars,
	char *filename, char direction)
{
	t_instructions	*instr;

	instr = &minish->instru[pars];
	if (!filename)
		return ;
	if (direction == '>')
		instr->to_file_str[instr->number_files_to - 1] = filename;
	else if (!ft_strncmp(direction, "from", 5))
		instr->from_file_str[instr->number_files_from - 1] = filename;
}
