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
	while (parser <= minish.number_of_commands)
	{
		forked = fork();
		if (forked == -1)
			perror("couldn't fork");
		else if (forked == 0)
			child_process(minish, parser);
	}
	parser = 0;
	while (parser <= minish.number_of_commands)
	{
		wait(NULL);
		parser++;
	}
	parser = 0;
	while (parser <= minish.number_of_commands)
	{
		close(minish.fd_pipes[parser]);
		parser++;
	}
}
void	execute(t_minishell minish, int file)
{
	int	execror;

	execror = execve(path, args, NULL);
	if (execror == -1)
		error("execution failed", args, path, file);
}

void access_test_and_open(t_minishell minish, int parser)
{
	/// i need to know if i can have redirections in the middle of the command i think i can
	if ()
	if (access(minish.from_file_str, F_OK) == 0)
	{
		if (access(minish.from_file_str, R_OK) != 0)
			error("permission denied:", minish, -1);
	}
	if (parser == 0)
		minish.from_file = open(minish.from_file_str, O_RDONLY, 0644);
	else if (parser == minish.number_of_commands)
		minish.to_file = open(minish.to_file_str, O_WRONLY | O_CREAT | O_TRUNC, 0755);
	if (openfile == -1)
		error("no such file or directory:", minish, -1);
	/*
	if (access(minish.instru[parser].executable[0], F_OK) == 0)
		path = args[0];
	else
		path = path_finding(argv, env, 1);
	*/
	if (minish.instru[parser].path_command == NULL)
		error("command not found:", minish);

	minish.instru[parser].executable = ft_split(minish.instru[parser].command, ' ');

}
void no_redirection_proc(t_minishell minish, int parser)
{

	if (parser != 0)
		dup2(minish.fd_pipes[parser - 1]);
	dup2(minish.fd_pipes[parser], STDOUT_FILENO);
}

void	child_process(t_minishell minish, int parser)
{
	// reduce the size for the norminette
	// in the parsing should test if the path is absolute
	
	if (minish.instru[parser].redirection_type == 0)
		no_redirection_proc(minish, parser);
	if (minish.instru[parser].redirection_type == 4)
		append_proc(minish, parser)
	if (minish.instru[parser].redirection_type == 3)
		x_type_proc(minish, parser);
	
	access_test_and_open(minish, parser);
	if (parser == 0)
		dup2(minish.from_file, STDIN_FILENO);
	else 
		dup2(minish.fd_pipes[parser - 1], STDIN_FILENO);
	if (parser == minish.number_of_commands)
		dup2(minish.to_file, STDOUT_FILENO);
	else 
		dup2(minish.fd_pipes[parser]);
	close(fd[0]);
	execute(path, minish.instru->command, openfile);
}