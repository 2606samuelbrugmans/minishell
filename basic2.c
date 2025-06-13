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
int	ft_strcmp(const char *s1, const char *s2)
{
	size_t			index;
	unsigned char	c1;
	unsigned char	c2;

	index = 0;
	while ((s1[index] != '\0' || s2[index] != '\0'))
	{
		c1 = (unsigned char) s1[index];
		c2 = (unsigned char) s2[index];
		if (c1 != c2)
			return (c1 - c2);
		index++;
	}
	return (0);
}
char	*ft_strchr(const char *s, int c)
{
	int		index;
	char	convc;

	convc = (char)c;
	index = 0;
	if (convc == '\0')
	{
		while (s[index] != '\0')
			index++;
		return ((char *)&s[index]);
	}
	while (s[index] != '\0')
	{
		if (s[index] == convc)
			return ((char *)&s[index]);
		index++;
	}
	return (NULL);
}
