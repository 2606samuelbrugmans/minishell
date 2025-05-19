#include "minishell.h"
void print_instructions(t_instructions *instr)
{
	printf("Executable: ");
    if (instr->executable) {
        for (int i = 0; instr->executable[i] != NULL; i++) {
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
        for (int i = 0; instr->to_file_str[i] != NULL; i++) {
            printf("%s ", instr->to_file_str[i]);
        }
    } else {
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

    printf("Pipe locations: ");
    if (minish->pipe_location) {
        for (int i = 0; minish->pipe_location[i] != -1; i++) {
            printf("%d ", minish->pipe_location[i]);
        }
    } else {
        printf("NULL");
    }
    printf("\n");

    printf("Pipes already found: %d\n", minish->pipes_already_found);

    printf("Parsed String: %s\n", minish->parsed_string ? minish->parsed_string : "NULL");

    printf("Environment Variables: ");
    if (minish->envp) {
        for (int i = 0; minish->envp[i] != NULL; i++) {
            printf("%s ", minish->envp[i]);
        }
    } else {
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