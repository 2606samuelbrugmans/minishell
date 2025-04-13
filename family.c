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
	parser = 0;
	while ( parser < minish.pipes_already_found)
	{
		close(minish.fd_pipes[parser][0]);
		close(minish.fd_pipes[parser][1]);
		parser++;
	}
}
void	execute(t_minishell minish, int parser)
{
	int	execror;

	execror = execve(minish.instru[parser].path_command,
		minish.instru[parser].executable, NULL);
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
	while (minish.instru[parser].number_files_from > index)
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
	while (minish.instru[parser].number_files_to > index)
	{
		if (access(minish.instru[parser].to_file_str[index], F_OK) == 0)
		{
			if (access(minish.instru[parser].to_file_str[index], W_OK) != 0)
				error("permission denied:", minish, -1);
		}
		index++;
	}
}

void no_redirection_proc(t_minishell minish, int parser, int can_to_pipe, int can_from_pipe)
{
	int index;
	int index_two;

	index_two = 0;
	index = 0;
	while (index < minish.pipes_already_found || index_two < minish.pipes_already_found) 
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
	if  (minish.instru[parser].number_files_to == 0 && can_to_pipe == 1)
		dup2(minish.fd_pipes[index][1], STDOUT_FILENO);
	close_stuff(minish, index, index_two);
}

void	child_process(t_minishell minish, int parser)
{
	// reduce the size for the norminette
	// in the parsing should test if the path is absolute
	access_test(minish, parser);
	no_redirection_proc(minish, parser);
	open_stuff(minish, parser);
	execute(minish);
}