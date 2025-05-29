#include "minishell.h"

int	run(t_minishell *minish)
{
	int i;

	i = 0;
	//// give_minishell(minish);
	print_minishell(minish);
	while (i < minish->number_of_commands)
	{
		if (pipe(minish->fd_pipes[i]) == -1 )
			perror("bablda");
		i++;
	}
	process(minish);
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
	close_stuff(minish);
	index = 0;
	while (index < minish->number_of_commands)
	{
		wait(NULL);
		index++;
	}
}
void	execute(t_minishell *minish, int parser)
{
	int	execror;

	execror = execve(minish->instru[parser].path_command,
		minish->instru[parser].executable, NULL);
	if (execror == -1)
		error(minish, "execution failed", parser);
}
/// 
///  no touch clean 
void access_test(t_minishell *minish, int parser)
{
	int index;
	int fd;

	index = 0;
	/// i need to know if i can have redirections in the middle of the command i think i can
	while (minish->instru[parser].number_files_from > index)
	{
		if (minish->instru[parser].redirection_from[index] == 1)
		{
			if (access(minish->instru[parser].from_file_str[index], F_OK) == 0)
			{
				if (access(minish->instru[parser].from_file_str[index], R_OK) != 0)
					error(minish, "permission denied:", parser);
				else
					fd = open(minish->instru[parser].from_file_str[index], O_RDONLY);
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
			if (minish->instru[parser].redirection_to[index] == 1)
				fd = open(minish->instru[parser].to_file_str[index], O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else 
				fd = open(minish->instru[parser].to_file_str[index], O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (index != minish->instru[parser].number_files_to - 1)
				close(fd);
			else 
				minish->instru[parser].to_file = fd;
		}
		index++;
	}
	write(2, "accessed", 9);
}
/*
void no_redirection_proc(t_minishell minish, int parser, int can_to_pipe, int can_from_pipe)
{
	int index;
	int index_two;

	index_two = 0;
	index = 0;
	while (index <minish.pipes_already_found || index_two < minish.pipes_already_found) 
	{
		if (minish.pipe_location[index] == parser)
			can_to_pipe = 1;
		else if (minish.pipe_location[index] == parser - 1)
			can_from_pipe = 1;
		if (can_to_pipe != 1)
			index++;
		if (can_from_pipe != 1)
			index_two++;
		if (can_to_pipe == 1 || can_from_pipe == 1)
			break;
	}
	if (minish.instru[parser].number_files_from == 0 && can_from_pipe == 1 )
		dup2(minish.fd_pipes[index_two][0], STDIN_FILENO);
	else if (minish.instru[parser].number_files_from != 0)
		dup2(minish.instru[parser].from_file, STDIN_FILENO);
	if  (minish.instru[parser].number_files_to == 0 && can_to_pipe == 1)
		dup2(minish.fd_pipes[index][1], STDOUT_FILENO);
	else if (minish.instru[parser].number_files_to != 0)
		dup2(minish.instru[parser].to_file, STDOUT_FILENO);
	close_stuff(minish, index, index_two);
}
*/
void no_redirection_proc(t_minishell *minish, int parser)
{
	if (minish->instru[parser].number_files_from != 0)
		dup2(minish->instru[parser].from_file, STDIN_FILENO);
	else if (parser != 0)
		dup2(minish->fd_pipes[parser - 1][0], STDIN_FILENO);
	if  (minish->instru[parser].number_files_to != 0)
		dup2(minish->instru[parser].to_file, STDOUT_FILENO);
	else if (minish->instru[parser].number_files_to == 0 && parser < minish->number_of_commands - 1)
		dup2(minish->fd_pipes[parser][1], STDOUT_FILENO);
	close_stuff(minish);
	if (minish->instru[parser].number_files_from != 0)
		close(minish->instru[parser].from_file);
	if (minish->instru[parser].number_files_from != 0)
		close(minish->instru[parser].to_file);
	
}
void	child_process(t_minishell *minish, int parser)
{
	// reduce the size for the norminette
	// in the parsing should test if the path is absolute
	/// nested(minish, parser);
	if (access(minish->instru[parser].executable[0], F_OK) == 0)
		minish->instru[parser].path_command = minish->instru[parser].executable[0];
	else
		minish->instru[parser].path_command = path_finding(minish->instru[parser].path_command, minish->envp);
	access_test(minish, parser);
	write(2, minish->instru[parser].path_command, ft_strlen(minish->instru[parser].path_command));
	no_redirection_proc(minish, parser);
	write(2, "\n ready to kill\n", 17);
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