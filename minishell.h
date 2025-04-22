#ifndef MINISHELL_H

# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <stddef.h>
///
typedef struct t_minishell
{
	t_instructions *instru;
	int		number_of_commands;
	int		(*fd_pipes)[2];
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



#endif