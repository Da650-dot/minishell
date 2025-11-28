#include "minishell.h"

static char	*get_var_name(char *str, int *i)
{
	int		start;
	int		len;
	char	*name;

	start = *i;
	if (str[*i] == '?')
	{
		(*i)++;
		return (ft_strdup("?"));
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (ft_strdup("$"));
	name = malloc(len + 1);
	if (!name)
		return (NULL);
	ft_strlcpy(name, &str[start], len + 1);
	return (name);
}

static char	*append_str(char *result, char *to_append)
{
	char	*temp;

	if (!to_append)
		return (result);
	temp = ft_strjoin(result, to_append);
	free(result);
	return (temp);
}

static char	*process_dollar(char *str, int *i, t_data *data)
{
	char	*var_name;
	char	*var_value;
	char	*result;

	(*i)++;
	if (!str[*i])
		return (ft_strdup("$"));
	var_name = get_var_name(str, i);
	if (!var_name)
		return (NULL);
	if (ft_strncmp(var_name, "?", 2) == 0 && var_name[1] == '\0')
	{
		free(var_name);
		return (ft_itoa(data->exit_status));
	}
	var_value = get_env(var_name, data->envp);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	result = ft_strdup(var_value);
	return (result);
}

char	*expand_variables(char *str, t_data *data)
{
	char	*result;
	char	*temp;
	char	c[2];
	int		i;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '$' && str[i + 1])
		{
			temp = process_dollar(str, &i, data);
			result = append_str(result, temp);
			free(temp);
		}
		else
		{
			c[0] = str[i];
			c[1] = '\0';
			result = append_str(result, c);
			i++;
		}
	}
	return (result);
}

char	**expand_args(char **args, t_data *data)
{
	char	**expanded;
	int		i;
	int		count;

	if (!args)
		return (NULL);
	count = 0;
	while (args[count])
		count++;
	expanded = malloc(sizeof(char *) * (count + 1));
	if (!expanded)
		return (NULL);
	i = 0;
	while (i < count)
	{
		expanded[i] = expand_variables(args[i], data);
		if (!expanded[i])
		{
			free_array(expanded);
			return (NULL);
		}
		i++;
	}
	expanded[count] = NULL;
	return (expanded);
}
