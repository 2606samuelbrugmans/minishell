#ifndef MINISHELL_H

# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
# include <readline/readline.h>
# include <readline/history.h>
///
typedef struct t_minishell
{
	t_instructions *instru;
	int		number_of_commands;
	int		(*fd_pipes)[2];
	int		nested_width;
	int		nested[2];
	int		(**pipe_nested)[2];
	/// @pipe_location 0 is the command concerned 1 is write or read interaction
	/// if value is 0 it's read otherwhise it's write
	int		*pipe_location;
	int		pipes_already_found;
	char	*parsed_string;
	char	**envp;
	char	*before_from;
	char	*before_to;
	char	**local_variables;
	int		quote;
	int		doublequote;
} 	t_minishell;
/// @brief // redirection type [0] is the input [1] is the output
///// first file in the arrays of files are the executed ones

typedef struct t_instructions
{
	char 	*command;
	char	**executable;
	char 	*path_command;
	int 	absolute;
	int		number_files_to;
	int		*redirection_to;
	int		number_files_from;
	int		*redirection_from;
	char	**from_file_str;
	int		from_file;
	char	**to_file_str;
	int		to_file;
}	t_instructions;

void	child_process(t_minishell *minish, int parser);
void    close_nested_stuff(t_minishell *minish);
void 	close_stuff(t_minishell *minish);
void	store_filename(t_minishell *minish, int pars,
	char *filename, char direction);
int		get_string(t_minishell *minish, int where, int pars, char direction);
void	skip_quotes(const char *str, int base_index, int *offset_index);
int 	initialise(t_minishell *minish, char *string, int *nested);
int 	count_commands(t_minishell *minish);
int 	skip_nested_command(char *parsed_string, int index);
int 	its_a_FILE(t_minishell minish, int index, char c);
void 	pre_init_command(t_minishell *minish, int pars, int *where);
int 	get_file_and_redirection(t_minishell *minish, int where, int pars);
int 	set_up_redirection(t_minishell *minish, char *direction, int type, int pars);
int		get_Command(t_minishell *minish, int location, int *has_command, int pars);
int		find_end_index(char *str, int where, char quote);
int		is_stopper(char c);
int		skip_spaces(char *str, int where);
int 	within_executable(t_minishell *minish, int i);
int 	redirection(t_minishell *minish, int i);
int 	not_quoted(t_minishell *minish);
void 	nested(t_minishell *minish, int parser);
int 	pipe_nested(t_minishell *minish, int length);
void 	no_redirection_proc(t_minishell *minish, int parser, int can_to_pipe, int can_from_pipe);
void 	access_test(t_minishell minish, int parser);
void	execute(t_minishell minish, int parser);
void	process(t_minishell *minish);
int		run(t_minishell *minish);

#endif