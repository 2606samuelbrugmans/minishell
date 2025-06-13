#include "minishell.h"

int is_builtin(const char *cmd)
{
    return
    (
        ft_strcmp(cmd, "echo") == 0 ||
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "pwd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "env") == 0 ||
        ft_strcmp(cmd, "exit") == 0
    );
}

int built_in_parent(char *cmd)
{
    return
    (
        ft_strcmp(cmd, "cd") == 0 ||
        ft_strcmp(cmd, "export") == 0 ||
        ft_strcmp(cmd, "unset") == 0 ||
        ft_strcmp(cmd, "exit") == 0
    );
}
int builtin_env(char **envp)
{
    int index;

    index = 0;
    while (envp[index] != NULL)
    {
        printf("%s", envp[index]);
        index++;
    }
    return (0);
}
int exec_builtin(char **argv, t_minishell *shell)
{
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
        return builtin_env(shell->envp);
    if (ft_strcmp(argv[0], "exit") == 0)
        return builtin_exit();
    return (-1);
}

int builtin_echo(char **argv)
{
    int i = 1;
    int newline = 1;

    if (argv[1] && ft_strcmp(argv[1], "-n") == 0) {
        newline = 0;
        i++;
    }
    while (argv[i]) {
        printf("%s", argv[i]);
        if (argv[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
    return 0;
}
int builtin_cd(char **argv, t_minishell *minish)
{
    char *path;
    int home;

    if (!argv[1])
    {
        home = find_string(minish->envp, "HOME");
	    if (home == -1)
        {
            write(2, "bash: cd: HOME not set", 23);
		    return (0);
        }
        path = getenv("HOME");
    }
    else    
        path = argv[1];
    if (chdir(path) != 0)
    {
        perror("cd");
        return 1;
    }
    // Update PWD and OLDPWD in env here
    return 0;
}
int builtin_pwd(void)
{
    char cwd[500];

    if (getcwd(cwd, sizeof(cwd)))
        printf("%s\n", cwd);
    else
        perror("pwd");
    return 0;
}
int find_nth(char**envp, int meower)
{
    int index;
    int current;

    index = 0;
    current = 0;
    while (envp[index] != NULL)
    {
        if (ft_strcmp(envp[index], envp[meower]) > 0 && ft_strcmp(envp[index], envp[current]) < 0)
            current = index;
        else if (ft_strcmp(envp[current], envp[meower]) > 0)
            current = index + 1;
        index++;
    }
    return (current);
}
int find_first(char **envp)
{
    int index;
    int tmp;
    int index_two;

    index = 0;
    tmp = ft_strcmp(envp[0], envp[1]);
    if (tmp > 0)
        index_two = 1;
    else 
        index_two = 0;
    index = 2;
    while (envp[index] != NULL)
    {
        tmp = ft_strcmp(envp[index_two], envp[index]);
        if (tmp > 0)
            index_two = index;
        index++;
    }
    return (index_two);
}
void print_declare(char **envp)
{
    int *order;
    int index;

    order = malloc(ft_sstrlen(envp) + 1);
    order[0] = ft_sstrlen(envp);
    index = 2;
    order[1] = find_first(envp);
    while (index != order[0])
    {
        order[index] = find_nth(envp, order[index - 1]);
        index++;
    }
    index = 1;
    while (index != order[0])
    {
        printf("declare -x %s\n", envp[order[index]]);
        index++;
    }
}
int builtin_export(char **argv, t_minishell *minish)
{
    int index;

    index = 1;
    if (argv[1] == NULL)
        print_declare(minish->envp);
    else 
    {
        while (argv[index] != NULL)
        {
            if (ft_strchr(argv[index], '=') != NULL)
                minish->envp = ft_sstrjoin(minish->envp, argv[index]);
            index++;
        }
    }

    return (0);
}

int builtin_exit(void)
{
    return (-2);
}

int builtin_unset(char **argv, t_minishell *minish)
{
    int where[ft_sstrlen(argv) - 1];
    int index;

    index = 0;
    while (argv[index + 1] != NULL)
    {
        where[index] = find_string(minish->envp, argv[index + 1]);
        index++;
    }
    minish->envp = remake_env(minish->envp, where, index);
    return (0);
}
int is_in_where(int *repertoire, int index, int unseteds)
{
    int explore;

    explore = 0;
    while (explore != unseteds)
    {
        if (repertoire[explore] == index)
            return (0);
        explore++;
    }
    return (-1);
}

char **remake_env(char **envpsrc, int *where, int unseteds)
{
    char **envpdst;
    int length;
    int index;
    int index_two;

    length = ft_sstrlen(envpsrc) - unseteds;
    envpdst = malloc(length + 1 * sizeof(char *));
    index = 0;
    index_two = 0;
    while (envpsrc[index_two] != NULL)
    {
        if (!is_in_where(where, index_two, unseteds))
        {
            (envpdst[index] = envpsrc[index_two]);
            index++;
        }
        index_two++;
    }
    return (envpdst);
}