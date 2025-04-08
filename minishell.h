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
	int number_of_commands;
	int *fd_files;
	int	*fd_pipes;
} 	t_minishell;
/// @brieflast three are boolean 
/// absolute path used or not
///	 appending >> or not and  << here document or not
typedef struct t_instructions
{
	char 	*command;
	char 	*path_command;
	int 	absolute;
	int		appending;
	int		here_document
}	t_instructions;



#endif