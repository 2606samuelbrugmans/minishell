#include    "minishell.h"





    
    //// where_nest [0][0] is the number of substuttion commands
	//// where_nest [0][1] is the number of strings in executable which have a substitution commands so how big (x - 1) can get also
	//// 
	//// where_nest [x][0] except for x = 0 is which stirng in  char	**executable the following nest is tied to
	//// where_nest [x][1] the number of nested commands within the executable thus the size of the array at x / 2
	//
	/// where_nest at [x][2] is the starting index of the nested command in the executable
	/// where_nest at [x][3] is the ending index of the nested command in the executable
	/// where_nest at [x][y] from now on is only starting and ending index
	/// fourth element is the  end of such a substitution command
	/// rinse and repeat until none are left
char *conc_it(char *original, char **the_done, int *the_soyence)
{
    char *buffer;
    int i;
    int j;

    i = 0;
    j = 3;
    buffer = ft_substr(original, 0, the_soyence[2]);
    while (i < the_soyence[1])
    {
        buffer = ft_strjoin(buffer, the_done[i]);
        buffer = ft_strjoin(buffer, ft_substr(original, the_soyence[j], the_soyence[j+1]));
        i++;
        j+=2;
    }
    
    buffer = ft_strjoin(buffer, ft_substr(original, the_soyence[i+2], the_soyence[4]));    
    return (buffer);
}
void concatenate_it(t_minishell *minish,int parser,char ***string,int **where_nest)
{
    int i;

    i = 1;
    while (i <= where_nest[0][1])
    {
        minish->instru[parser].executable[where_nest[i][1]]
        = conc_it(minish->instru[parser].executable[where_nest[i][1]], string[i - 1], where_nest[i]);
        i++;
    }
    


}
