#include "minishell.h"

char	*path_finding(char *pathed, char **env)
{
	int		path;
	char	*full_path;

	path = find_string(env, "PATH");
	if (path == -1)
		return (NULL);
	full_path = get_path(pathed, env[path], 5);
	full_path = get_path(pathed, env[path], 5);
	return (full_path);
}

char	*potential_pathing(char *paths, char *command_to_path, int *index)
{
	char	*potential_path;
	int		size;

	size = path_len(paths, *index);
	size += path_len(command_to_path, 0);
	potential_path = malloc((size + 2) * sizeof(char));
	if (potential_path == NULL)
		return (NULL);
	potential_path[size + 1] = '\0';
	size = 0;
	while (paths[*index] != '\0' && paths[*index] != ':')
	{
		potential_path[size] = paths[*index];
		size++;
		(*index)++;
	}
	if (paths[*index] == ':')
		(*index)++;
	potential_path[size] = '/';
	size++;
	putcommand(command_to_path, potential_path, size);
	return (potential_path);
}

int	find_string(char **env, char *path)
{
	int	index;
	int	index_two;

	index = 0;
	index_two = 0;
	while (env[index][index_two] != '\0' && env[index][index_two] != '=')
	{
		index_two = 0;
		while (path[index_two] != '\0'
			&& env[index][index_two] == path[index_two])
			index_two++;
		if (path[index_two] == '\0')
			return (index);
		index++;
	}
	return (-1);
}

char	*get_path(char *command_to_path, char *paths, int index)
{
	char	*full_path;



	if (command_to_path == NULL || command_to_path[0] == 32)
		return (NULL);
	while (paths[index] != '\0')
	{
		full_path = potential_pathing(paths, command_to_path, &index);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		if (full_path != NULL)
			free(full_path);
	}
	return (NULL);
}