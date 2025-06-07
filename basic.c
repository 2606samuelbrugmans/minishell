#include "minishell.h"
size_t	ft_strlen(const char *s)
{
	size_t	index;

	index = 0;
	while (s[index] != '\0')
		index++;
	return (index);
}
char	*ft_strdup(const char *s1)
{
	int		index;
	char	*s2;

	index = 0;
	while (s1[index] != '\0')
		index++;
	s2 = malloc((index + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	index = 0;
	while (s1[index] != '\0')
	{
		s2[index] = s1[index];
		index++;
	}
	s2[index] = '\0';
	return (s2);
}
int	count_quote(char *string, char c)
{
	int index;
	int counted;

	index = 0;
	counted = 0;
	while (string[index] != c)
	{
		if (string[index] == c)
			counted++;
		index++;
	}
	return (counted);
}
int	path_len(char *string, int index)
{
	int	second_index;

	second_index = 0;
	while (string[index + second_index] != '\0'
		&& string[index + second_index] != ':'
		&& string[index + second_index] != ' ')
		second_index++;
	return (second_index);
}
void	putcommand(char *command_to_path, char *potential_path, int size)
{
	int	j;

	j = 0;
	while (command_to_path[j] != '\0' && command_to_path[j] != ' ')
	{
		potential_path[size + j] = command_to_path[j];
		j++;
	}
}
t_instructions init_instructions(t_instructions instr)
{
	
	instr.command = NULL;
	instr.executable = NULL;
	instr.path_command = NULL;
	instr.number_files_to = 0;
	instr.redirection_to = NULL;
	instr.number_files_from = 0;
	instr.redirection_from = NULL;
	instr.from_file_str = NULL;
	instr.from_file = -1;
	instr.to_file_str = NULL;
	instr.to_file = -1;

	return (instr);
}
void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n >= 0 && n <= 9)
	{
		c = (n % 10) + 48;
		write(fd, &c, 1);
	}
	else if (n == -2147483648)
		write(fd, "-2147483648", 12);
	else if (n < 0)
	{
		n *= -1;
		write(fd, "-", 1);
		ft_putnbr_fd(n, fd);
	}
	else
	{
		c = (n % 10) + 48;
		ft_putnbr_fd(n / 10, fd);
		write(fd, &c, 1);
	}
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
