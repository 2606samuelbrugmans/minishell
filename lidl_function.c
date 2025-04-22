#include "minishell.h"
int not_quoted(t_minishell minish)
{
    if (minish.quote % 2 == 0 && minish.doublequote % 2 == 0)
        return (0);
    return (-1);
}
int redirection(t_minishell minish, int i)
{
    if ((minish.parsed_string[i] == '>'
         || minish.parsed_string[i] == '<') && not_quoted(minish))
        return (0);
    return (-1);
}
int within_executable(t_minishell *minish, int i)
{
    if (minish->parsed_string[i] != '\0' && 
		(minish->parsed_string[i] != '|' || !(not_quoted(*minish))))
        return (0);
    return (-1);
}
int	skip_spaces(char *str, int where)
{
	while (str[where] == ' ')
		where++;
	return (where);
}

int	is_stopper(char c)
{
	return (c == ' ' || c == '|' || c == '<' || c == '>' || c == '\0');
}

int	find_end_index(char *str, int where, char quote)
{
	while (str[where])
	{
		if (quote && str[where] == quote)
			break;
		if (!quote && is_stopper(str[where]))
			break;
		where++;
	}
	return (where);
}
