#include "minishell.h"

int	run(t_minishell *minish)
{
	int i;

	i = 0;
	//// give_minishell(minish);
	if (built_in_parent(minish->instru[0].executable[0]) && minish->number_of_commands == 1)
		exec_builtin(minish->instru[0].executable, minish);
	else
	{
		print_minishell(minish);
		while (i < minish->number_of_commands)
		{
			if (pipe(minish->fd_pipes[i]) == -1 )
				perror("bablda");
			i++;
		}
		process(minish);
	}
	return (0);
}
void	process(t_minishell *minish)
{
	pid_t	forked;
	int		parser;
	int		index;

	parser = 0;
	while (parser < minish->number_of_commands)
	{
		forked = fork();
		if (forked == -1)
			perror("couldn't fork");
		else if (forked == 0)
			child_process(minish, parser);
		parser++;
	}
	index = 0;
	close_parent(minish);
	while (index < minish->number_of_commands)
	{
		wait(NULL);
		index++;
	}
}
void	execute(t_minishell *minish, int parser)
{
	int	execror;

	write(2, "reached execution", 18);
	execror = execve(minish->instru[parser].path_command,
		minish->instru[parser].executable, NULL);
	if (execror == -1)
		error(minish, "execution failed", parser);
}
///  no touch clean 
void access_test(t_minishell *minish, int parser)
{
	int index;
	int fd;

	index = 0;
	while (minish->instru[parser].number_files_from > index)
	{
		if (minish->instru[parser].redirection_from[index] == 1)
		{
			if (access(minish->instru[parser].from_file_str[index], F_OK) == 0)
			{
				if (access(minish->instru[parser].from_file_str[index], R_OK) != 0)
					error(minish, "permission denied:", parser);
				else
				{
					fd = open(minish->instru[parser].from_file_str[index], O_RDONLY, 777);
					if (fd == -1)
						error(minish, "couldn't open file", parser);
				}
				if (index != minish->instru[parser].number_files_from - 1)
					close(fd);
				else
					minish->instru[parser].from_file = fd;
			}
			else
				error(minish, "no such file or directory:", parser);
		}
		index++;
	}
	index = 0;
	while (minish->instru[parser].number_files_to > index)
	{
		if (access(minish->instru[parser].to_file_str[index], F_OK) == 0)
		{
			if (access(minish->instru[parser].to_file_str[index], W_OK) != 0)
				error(minish, "permission denied:", parser);
		}
		if (minish->instru[parser].redirection_to[index] == 1)
		{
			fd = open(minish->instru[parser].to_file_str[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error(minish, "couldn't open file", parser);
		}
		else
		{
			fd = open(minish->instru[parser].to_file_str[index], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				error(minish, "couldn't open file", parser);
		}
		if (index != minish->instru[parser].number_files_to - 1)
			close(fd);
		else 
			minish->instru[parser].to_file = fd;
		index++;
	}
}

void no_redirection_proc(t_minishell *minish, int parser)
{
	if (minish->instru[parser].number_files_from != 0)
		dup2(minish->instru[parser].from_file, STDIN_FILENO);
	else if (parser != 0)
		dup2(minish->fd_pipes[parser - 1][0], STDIN_FILENO);
	if  (minish->instru[parser].number_files_to != 0)
		dup2(minish->instru[parser].to_file, STDOUT_FILENO);
	else if (minish->instru[parser].number_files_to == 0 && parser != minish->number_of_commands - 1)
		dup2(minish->fd_pipes[parser][1], STDOUT_FILENO);
	close_stuff(minish, parser);
	if (minish->instru[parser].number_files_from != 0)
		close(minish->instru[parser].from_file);
	if (minish->instru[parser].number_files_to != 0)
		close(minish->instru[parser].to_file);
}

void	child_process(t_minishell *minish, int parser)
{
	// reduce the size for the norminette
	// in the parsing should test if the path is absolute
	/// nested(minish, parser);
	if (is_builtin(minish->instru[parser].executable[0]))
		minish->instru[parser].path_command = minish->instru[parser].executable[0];
	else if (access(minish->instru[parser].executable[0], F_OK) == 0)
		minish->instru[parser].path_command = minish->instru[parser].executable[0];
	else
		minish->instru[parser].path_command = path_finding(minish->instru[parser].executable[0], minish->envp);
	access_test(minish, parser);
	no_redirection_proc(minish, parser);
	if (is_builtin(minish->instru[parser].path_command))
		exec_builtin(minish->instru[parser].executable, minish);
	else
		execute(minish, parser);
}

void	error(t_minishell *minish, char *reason, int parser)
{
	int	index;

	index = 0;
	ft_putstr_fd(reason, 2);
	if (minish->instru[parser].path_command)
		free(minish->instru[parser].path_command);
	if (minish->instru[parser].executable)
	{
		while (minish->instru[parser].executable[index] != NULL)
		{
			free(minish->instru[parser].executable[index]);
			index++;
		}
		free(minish->instru[parser].executable);
	}
	exit(-1);
}

