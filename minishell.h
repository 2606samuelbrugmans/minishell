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
	char	*from_file_str;
	int		from_file;
	char	*to_file_str;
	int		to_file;
} 	t_minishell;
/// @brieflast three are boolean 
/// absolute path used or not
///	 appending >> or not and  << here document or not
typedef struct t_instructions
{
	char 	*command;
	char	**executable;
	char 	*path_command;
	int 	absolute;
	int		redirection_type;
}	t_instructions;



#endif