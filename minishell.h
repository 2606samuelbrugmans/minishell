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
	int		*fd_pipes;
	char	*parsed_string;
	char	**envp;
	char	*before_from;
	char	*before_to;
	int		instru_unclosed_quote;
	int		instru_unclosed_doublequote;
} 	t_minishell;
/// @brief // redirection type [0] is the input [1] is the output
///// first file in the arrays of files are the executed ones

typedef struct t_instructions
{
	char 	*command;
	char	**executable;
	char 	*path_command;
	int 	absolute;
	int		redirection_type[2];
	char	**from_file_str;
	int		from_file;
	char	**to_file_str;
	int		to_file;

}	t_instructions;



#endif