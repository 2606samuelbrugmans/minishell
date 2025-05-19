#include "minishell.h"

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
