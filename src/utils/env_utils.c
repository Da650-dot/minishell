
#include "../includes/minishell.h"

/**
 * @brief Busca variável de ambiente
 * @param key Nome da variável (ex: "HOME", "USER")
 * @param envp Array de environment
 * @return Valor da variável ou NULL se não encontrada
 */
char	*get_env(char *key, char **envp)
{
	int		i;
	int		key_len;
	char	*value;

	if (!key || !envp)
		return (NULL);
	
	key_len = ft_strlen(key);
	i = 0;
	
	while (envp[i])
	{
		// Verifica se começa com "KEY="
		if (ft_strncmp(envp[i], key, key_len) == 0
			&& envp[i][key_len] == '=')
		{
			// Retorna ponteiro para o valor (após o '=')
			value = &envp[i][key_len + 1];
			return (value);
		}
		i++;
	}
	
	return (NULL);
}
