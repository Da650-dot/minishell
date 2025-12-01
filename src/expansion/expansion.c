#include "minishell.h"

// MUDE O NOME desta função:
char	*expand_variables_raw(char *str, t_data *data)  // ERA expand_variables
{
	char	*result;
	char	*temp;
	char	c[2];
	int		i;
	bool	in_single_quotes;
	bool	in_double_quotes;

	result = ft_strdup("");
	i = 0;
	in_single_quotes = false;
	in_double_quotes = false;
	while (str[i])
	{
		if (str[i] == '\'' && !in_double_quotes)
			in_single_quotes = !in_single_quotes;
		else if (str[i] == '\"' && !in_single_quotes)
			in_double_quotes = !in_double_quotes;
		if (str[i] == '$' && str[i + 1] && !in_single_quotes)
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

char *remove_quotes(char *str)
{
    char *result;
    int i = 0;
    int j = 0;
    bool in_single = false;
    bool in_double = false;
    
    if (!str)
        return (NULL);
    
    result = malloc(ft_strlen(str) + 1);
    if (!result)
        return (NULL);
    
    while (str[i])
    {
        if (str[i] == '\'' && !in_double)
        {
            in_single = !in_single;
            i++;
            continue;
        }
        else if (str[i] == '"' && !in_single)
        {
            in_double = !in_double;
            i++;
            continue;
        }
        
        result[j++] = str[i++];
    }
    result[j] = '\0';
    
    char *final = ft_strdup(result);
    free(result);
    return (final);
}

char *expand_variables(char *str, t_data *data)
{
    char *expanded;
    char *final;
    
    expanded = expand_variables_raw(str, data);
    final = remove_quotes(expanded);
    free(expanded);
    return (final);
}