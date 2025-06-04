#include "minishell.h"
void print_instructions(t_instructions *instr)
{
	printf("Executable: ");
    if (instr->executable) {
        for (int i = 0; instr->executable[i] != NULL; i++) {
            printf("\n executable %d", i);
            printf("%s ", instr->executable[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");
    if (instr == NULL) {
        printf("t_instructions is NULL\n");
        return;
    }

    printf("Command: %s\n", instr->command ? instr->command : "NULL");
    printf("Path Command: %s\n", instr->path_command ? instr->path_command : "NULL");
    printf("Number of files to: %d\n", instr->number_files_to);

    printf("Redirection To: ");
    if (instr->redirection_to) {
        for (int i = 0; i < instr->number_files_to; i++) {
            printf("%d ", instr->redirection_to[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");

    printf("Number of files from: %d\n", instr->number_files_from);

    printf("Redirection From: ");
    if (instr->redirection_from) {
        for (int i = 0; i < instr->number_files_from; i++) {
            printf("%d ", instr->redirection_from[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");

    printf("From File: %d\n", instr->from_file);
    printf("To File: %d\n", instr->to_file);
    printf("From File Strings: ");
    if (instr->from_file_str) {
        for (int i = 0; instr->from_file_str[i] != NULL; i++) {
            printf("%s ", instr->from_file_str[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");

    printf("To File Strings: ");
    if (instr->to_file_str) {
        for (int i = 0; instr->to_file_str[i] != NULL; i++)
        {
            printf("%s ", instr->to_file_str[i]);
        }
    }
    else
    {
        printf("NULL");
    }
    printf("\n");
    printf("\n");
}

// Function to print the contents of t_minishell
void print_minishell(t_minishell *minish)
{
    if (minish == NULL) {
        printf("t_minishell is NULL\n");
        return;
    }
    printf("Number of commands: %d\n", minish->number_of_commands);
    printf("\n");
    printf("Pipes already found: %d\n", minish->pipes_already_found);
    printf("Parsed String: %s\n", minish->parsed_string ? minish->parsed_string : "NULL");
    printf("Environment Variables: ");
    if (minish->envp)
    {
        for (int i = 0; minish->envp[i] != NULL; i++) {
            printf("%s ", minish->envp[i]);
        }
    }
    else
    {
        printf("NULL");
    }
    printf("\n");
    printf("Local Variables: ");
    if (minish->local_variables) {
        for (int i = 0; minish->local_variables[i] != NULL; i++) {
            printf("%s ", minish->local_variables[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");
    printf("Quote: %d\n", minish->quote);
    printf("Doublequote: %d\n", minish->doublequote);
    // Print all instructions in the array (if any)
    printf("Instructions:\n");
    for (int i = 0; i < minish->number_of_commands; i++) {
        printf("Command %d:\n", i + 1);
        print_instructions(&minish->instru[i]);
    }
}

void init_minishell(t_minishell *minish) {
    if (!minish)
        return;
    minish->number_of_commands = 2;
    // Pipe locations (terminated with -1)
    minish->fd_pipes = malloc((minish->number_of_commands -1) * sizeof(int[2]));
    minish->parsed_string = strdup("< input.txt ls -la | grep txt > output.txt");
    // Environment variables
    // Local variables
    minish->local_variables = malloc(2 * sizeof(char *));
    minish->local_variables[0] = strdup("VAR=value");
    minish->local_variables[1] = NULL;
    minish->quote = 0;
    minish->doublequote = 0;
    // Allocate instruction array
    minish->instru = malloc(minish->number_of_commands * sizeof(t_instructions));
    // Fill in one instruction manually
    t_instructions *instr = &minish->instru[0];
    instr->executable = malloc(3 * sizeof(char *));
    instr->executable[0] = strdup("ls");
    instr->executable[1] = strdup("-la");
    instr->executable[2] = NULL;
    instr->command = strdup("ls -la");
    instr->path_command = strdup("/usr/bin/ls");
    //instr->number_files_to = 1;
    //instr->redirection_to = malloc(sizeof(int));
    //instr->redirection_to[0] = 1;
    instr->number_files_from = 1;
    instr->redirection_from = malloc(sizeof(int));
    instr->redirection_from[0] = 1;
    instr->from_file = 1;
    //instr->to_file = 0;
    instr->from_file_str = malloc(2 * sizeof(char *));
    instr->from_file_str[0] = strdup("input.txt");
    instr->from_file_str[1] = NULL;
    /*instr->to_file_str = malloc(2 * sizeof(char *));
    instr->to_file_str[0] = strdup("output.txt");
    instr->to_file_str[1] = NULL;
    */
    t_instructions *instro = &minish->instru[1];
    instro->executable = malloc(3 * sizeof(char *));
    instro->executable[0] = strdup("grep");
    instro->executable[1] = strdup("txt");
    instro->executable[2] = NULL;
    instro->command = strdup("grep");
    instro->path_command = strdup("/usr/bin/grep");
    instro->number_files_to = 1;
    instro->redirection_to = malloc(sizeof(int));
    instro->redirection_to[0] = 1;
    instro->number_files_from = 0;
    /*
    instro->redirection_from = malloc(sizeof(int));
    instro->redirection_from[0] = 0;
    */
    instro->from_file = 0;
    instro->to_file = 1;
    /*
    instr->from_file_str = malloc(2 * sizeof(char *));
    instr->from_file_str[0] = strdup("input.txt");
    instr->from_file_str[1] = NULL;
    */
    instro->to_file_str = malloc(2 * sizeof(char *));
    instro->to_file_str[0] = strdup("output.txt");
    instro->to_file_str[1] = NULL;
}
