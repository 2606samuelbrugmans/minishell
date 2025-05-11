#include "minishell.h"
int not_quoted(t_minishell *minish)
{
    if (minish->quote % 2 == 0 && minish->doublequote % 2 == 0)
        return (0);
    return (-1);
}

int redirection(t_minishell *minish, int i)
{
    if ((minish->parsed_string[i] == '>'
         || minish->parsed_string[i] == '<'))
        return (0);
    return (-1);
}

int within_executable(t_minishell *minish, int i)
{
    if (minish->parsed_string[i] != '\0' && 
		(minish->parsed_string[i] != '|' || !(not_quoted(minish))))
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
	int ready_to_stop;

	ready_to_stop = 1;
	if (quote == 0)
	{
		while (str[where] !='\0')
		{
			if (is_stopper(str[where]))
				return (where);
			where++;
		}
	}
	while (str[where]!='\0')
	{
		/*
		if (str[where]== '$' && str[where + 1] == '(')
			where = skip_nested_command(minish, where + 2);
		*/
		if (str[where] == quote)
			ready_to_stop++;
		if ((ready_to_stop % 2 == 0) && is_stopper(str[where]))
			break;
		where++;
	}
	return (where);
}
void	ft_putstr_fd(char *s, int fd)
{
	int	index;
	int	len;

	len = 0;
	while (s[len] != '\0')
		len++;
	index = 0;
	while (index < len)
	{
		write(fd, &s[index], 1);
		index++;
	}
}
char	*subordonate(char const *s,
	unsigned int start, size_t len, char *string)
{
	unsigned int	index;
	unsigned int	lendeu;

	lendeu = ft_strlen(s);
	index = 0;
	while (index + start < lendeu && index < len)
	{
		string[index] = s[start + index];
		index++;
	}
	string[index] = '\0';
	return (string);
}

char	*ft_substr(char const *s, unsigned int start,
	size_t len)
{
	char			*string;
	unsigned int	lendeu;

	lendeu = ft_strlen(s);
	if (!s)
		return (NULL);
	if (len > 0 && start <= lendeu)
	{
		if (lendeu - start > len)
			string = malloc((len + 1) * sizeof(char));
		else
			string = malloc((lendeu - start + 1) * sizeof(char));
	}
	else
		return (ft_strdup(""));
	if (!string)
		return (NULL);
	return (subordonate(s, start, len, string));
}