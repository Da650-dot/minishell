#include "minishell.h"

bool	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	i = 1;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

int	find_env_index(char **envp, char *key)
{
	int	i;
	int	key_len;

	if (!envp || !key)
		return (-1);
	key_len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, key_len) == 0
			&& envp[i][key_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	**add_env_var(char **envp, char *new_var)
{
	char	**new_envp;
	int		i;
	int		count;
	int		index;
	char	*key;
	char	*equals;

	equals = ft_strchr(new_var, '=');
	if (!equals)
		return (envp);
	key = ft_substr(new_var, 0, equals - new_var);
	index = find_env_index(envp, key);
	free(key);
	if (index != -1)
	{
		free(envp[index]);
		envp[index] = ft_strdup(new_var);
		return (envp);
	}
	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * (count + 2));
	if (!new_envp)
		return (envp);
	i = 0;
	while (i < count)
	{
		new_envp[i] = envp[i];
		i++;
	}
	new_envp[count] = ft_strdup(new_var);
	new_envp[count + 1] = NULL;
	free(envp);
	return (new_envp);
}

char	**remove_env_var(char **envp, char *key)
{
	char	**new_envp;
	int		i;
	int		j;
	int		count;
	int		index;

	index = find_env_index(envp, key);
	if (index == -1)
		return (envp);
	count = 0;
	while (envp[count])
		count++;
	new_envp = malloc(sizeof(char *) * count);
	if (!new_envp)
		return (envp);
	i = 0;
	j = 0;
	while (i < count)
	{
		if (i != index)
		{
			new_envp[j] = envp[i];
			j++;
		}
		else
			free(envp[i]);
		i++;
	}
	new_envp[j] = NULL;
	free(envp);
	return (new_envp);
}
