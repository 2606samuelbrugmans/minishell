#ifndef MINISHELL_H

# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
///
typedef struct t_instructions
{
	char 	*command;
	char	**executable;
	char 	*path_command;
	int		number_files_to;
	int		*redirection_to;
	int		number_files_from;
	int		*redirection_from;
	char	**from_file_str;
	int		from_file;
	char	**to_file_str;
	int		to_file;
}	t_instructions;

typedef struct t_minishell
{
	t_instructions *instru;
	int		number_of_commands;
	int		(*fd_pipes)[2];
	/// @pipe_location 0 is the command concerned 1 is write or read interaction
	/// if value is 0 it's read otherwhise it's write
	int		*pipe_location;
	int		pipes_already_found;
	char	*parsed_string;
	char	**envp;
	char	**local_variables;
	int		quote;
	int		doublequote;
} 	t_minishell;
/// @brief // redirection type [0] is the input [1] is the output
///// first file in the arrays of files are the executed ones

void 	close_parent(t_minishell *minish);
char	*remove_quote(char *string, char quote);
void	child_process(t_minishell *minish, int parser);
void	close_stuff(t_minishell *minish, int parser);
void	store(t_minishell *minish, int pars,
	char *filename, char direction);
void	ft_putnbr_fd(int n, int fd);
int		get_string(t_minishell *minish, int where, int pars, char direction);
void	skip_quotes(const char *str, int base_index, int *offset_index);
int 	count_commands(t_minishell *minish);
void 	pre_init_command(t_minishell *minish, int pars, int *where);
int 	get_file_and_redirection(t_minishell *minish, int where, int pars);
void 	set_up_redirection(t_minishell *minish, char direction, int type, int pars);
int		get_Command(t_minishell *minish, int location, int *has_command, int pars);
int		find_end_index(char *str, int where, char *quote);
int		is_stopper(char c);
int		initialise(t_minishell *minish, char *string);
void	init_minishell(t_minishell *minish);
int		skip_spaces(char *str, int where);
int 	within_executable(t_minishell *minish, int i);
int 	redirection(t_minishell *minish, int i);
int 	not_quoted(t_minishell *minish);
void 	nested(t_minishell *minish, int parser);
int 	pipe_nested(t_minishell *minish, int length);
void 	no_redirection_proc(t_minishell *minish, int parser);
void 	access_test(t_minishell *minish, int parser);
void	execute(t_minishell *minish, int parser);
void	process(t_minishell *minish);
int		run(t_minishell *minish);
void	error(t_minishell *minish, char *reason, int parser);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s1);
void	store(t_minishell *minish, int pars,
	char *filename, char direction);
int	count_quote(char *string, char c);
char	*ft_substr(char const *s, unsigned int start,
	size_t len);
int	ft_strcmp(const char *s1, const char *s2);

////////////////////////////////// paths
char	*path_finding(char *pathed, char **env);
char	*potential_pathing(char *paths, char *command_to_path, int *index);
int		find_string(char **env, char *path);
char	*get_path(char *command_to_path, char *paths, int index);
int	path_len(char *string, int index);
void	putcommand(char *command_to_path, char *potential_path, int size);
t_instructions init_instructions(t_instructions instr);
/////////////// debug
void print_minishell(t_minishell *minish);
void print_instructions(t_instructions *instr);






#endif