#include "minishell.h"

int is_builtin(const char *cmd) {
    return (
        ft_strcmp(cmd, "echo") == 0 ||
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "env") == 0 ||
        ft_strcmp(cmd, "exit") == 0
    );
}
int built_in_parent(char *cmd) {
    return (
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "exit") == 0
    );
}
int exec_builtin(char **argv, t_minishell *shell) {
    if (ft_strcmp(argv[0], "echo") == 0)
        return builtin_echo(argv);
    if (ft_strcmp(argv[0], "cd") == 0)
        return builtin_cd(argv, shell);
    if (ft_strcmp(argv[0], "pwd") == 0)
        return builtin_pwd();
    if (ft_strcmp(argv[0], "export") == 0)
        return builtin_export(argv, shell);
    if (ft_strcmp(argv[0], "unset") == 0)
        return builtin_unset(argv, shell);
    if (ft_strcmp(argv[0], "env") == 0)
        return builtin_env(shell->env);
    if (ft_strcmp(argv[0], "exit") == 0)
        return builtin_exit(argv, shell);
    return 1;
}
int builtin_echo(char **argv) {
    int i = 1;
    int newline = 1;

    if (argv[1] && ft_strcmp(argv[1], "-n") == 0) {
        newline = 0;
        i++;
    }
    while (argv[i]) {
        ft_printf("%s", argv[i]);
        if (argv[i + 1])
            ft_printf(" ");
        i++;
    }
    if (newline)
        ft_printf("\n");
    return 0;
}
int builtin_cd(char **argv, t_minishell *shell)
{
    char *path;
    
    if (!argv[1])
        path = getenv("HOME");
    else    
        path = argv[1];
    if (chdir(path) != 0) {
        perror("cd");
        return 1;
    }
    // Update PWD and OLDPWD in env here
    return 0;
}
int builtin_pwd(void)
{
    char cwd[PATH_MAX];

    if (getcwd(cwd, sizeof(cwd)))
        ft_printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}
