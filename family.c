#include "minishell.h"

int	run(t_minishell minish)
{
	if (pipe(minish.fd_pipes) == -1 )
		perrorr("bablda");
	else 
		process(minish);

}
void	process(t_minishell minish)
{
	pid_t	forked;
	int parser;
	
	parser = 0;
	while (parser < minish.number_of_commands)
	{
		forked = fork();
		if (forked == -1)
			perror("couldn't fork");
		else if (forked == 0)
			child_process(minish, parser);
		parser++;
	}
	parser = 0;
	while (parser <= minish.number_of_commands)
	{
		wait(NULL);
		parser++;
	}
	close(minish.fd_pipes[0]);
	close(minish.fd_pipes[1]);
}
void	execute(t_minishell minish)
{
	int	execror;

	execror = execve(path, args, NULL);
	if (execror == -1)
		error("execution failed", minish);
}
/// 
///  no touch clean 
void access_test(t_minishell minish, int parser)
{
	int index;

	index = 0;
	/// i need to know if i can have redirections in the middle of the command i think i can
	while (minish.instru[parser].number_files_from < index)
	{
		if (minish.instru[parser].redirection_from[index] == 1)
		{
			if (access(minish.instru[parser].from_file_str[index], F_OK) == 0)
			{
				if (access(minish.instru[parser].from_file_str[index], R_OK) != 0)
					error("permission denied:", minish, -1);
			}
			else 
				error("no such file or directory:", minish, -1);
		}
		index++;
	}
	index = 0;
	while (minish.instru[parser].number_files_to < index)
	{
		if (access(minish.instru[parser].to_file_str[index], F_OK) == 0)
		{
			if (access(minish.instru[parser].to_file_str[index], W_OK) != 0)
				error("permission denied:", minish, -1);
		}
		index++;
	}
}

void no_redirection_proc(t_minishell minish, int parser)
{
	if (minish.instru[parser].number_files_from == 0 && parser != 0)
		dup2(minish.fd_pipes[0], STDIN_FILENO);
	if  (minish.instru[parser].number_files_to == 0 && parser != minish.number_of_commands - 1)
		dup2(minish.fd_pipes[1], STDOUT_FILENO);
	if (minish.instru[parser].number_files_from != 0)
		close(minish.fd_pipes[0]);
	if (minish.instru[parser].number_files_to != 0)
		close(minish.fd_pipes[1]);
}

void	child_process(t_minishell minish, int parser)
{
	// reduce the size for the norminette
	// in the parsing should test if the path is absolute
	no_redirection_proc(minish, parser);
	access_test(minish, parser);
	open_stuff(minish, parser);
	execute(minish);
}