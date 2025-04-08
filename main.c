#include "minishell.h"
int count_commands(char *prompt)
{
	int index;
	int pipes;
	int quotes;

	quotes = 0;
	pipes = 0;
	index = 0;
	while (prompt[index] != '\0')
	{
		/// unsure about the quotes
		if (prompt[index] == '"')
			quotes += 1;
		if (prompt[index] == '|' && quotes % 2 == 0)
			pipes +=1;
		index++;
	}
	return (pipes);
}
int initialise(t_minishell minish, char **envp, char *prompt)
{
	int pars;
	int where;

	where = 0;
	pars = 0;
	minish.number_of_commands = count_commands(prompt) + 1;
	minish.instru = ((minish.number_of_commands + 1) * sizeof(t_instructions));
	while (pars <= minish.number_of_commands )
	{
		minish.instru[pars] = init_command(prompt, &where);
		pars++;
	}


}

int main(char **envp)
{
	char 			*prompt;
	t_minishell		minish;

	while (1)
	{
		readline(prompt);
		initialise(minish, envp, prompt);
		if (run(minish) != 0)
			break;
	}
	



}