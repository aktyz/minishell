/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_expander_env.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mwiecek <mwiecek@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 18:13:09 by zslowian          #+#    #+#             */
/*   Updated: 2025/04/28 21:27:46 by mwiecek          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*identify_var(char *str);

/*
*  After splitting the user's input into tokens, we have to expand
*  the variables. After the expansion is done, quote characters are
*  removed from the original word unless they are between quotes.
*/

/**
 * This function will look up our env list for a environment variable:
 * var, being passed as "<env var name>".
 *
 */
static int	var_exists(t_global *global, char *var)
{
	t_list			*env;
	t_minishell_env	*content;

	env = global->env;
	while (env && env->content)
	{
		content = (t_minishell_env *) env->content;
		if (ft_strncmp(content->name_value[0], var, ft_strlen(var)) == 0)
			return (0);
		env = env->next;
	}
	return (1);
}

static char	*search_env_var(t_global *global, char *var)
{
	char			*str;
	t_list			*env;
	t_minishell_env	*content;

	env = global->env;
	while (env && env->content)
	{
		content = (t_minishell_env *) env->content;
		if (ft_strncmp(content->name_value[0], var, ft_strlen(var)) == 0)
			break ;
		env = env->next;
	}
	str = ft_strdup(content->name_value[1]);
	return (str);
}

char	*recover_val(t_token *token, char *str, t_global *global)
{
	char	*value;
	char	*var_name;
	char	*var;

	var = identify_var(str);
	var_name = ft_strtrim(var, "=");
	value = NULL;
	free_ptr((void **)&var);
	if (var_name && var_exists(global, var_name) == 0)
	{
		if (token != NULL)
			token->var_exists = true;
		value = search_env_var(global, var_name);
	}
	else if (var_name && var_name[0] == '?' && var_name[1] == '\0')
	{
		value = ft_itoa(global->last_exit_code);
		token->status_request = true;
	}
	free_ptr((void **)&var_name);
	return (value);
}

char	*identify_var(char *str)
{
	char	*var;
	char	*tmp;
	int		start;
	int		len;
	int		i;

	i = 0;
	start = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			start = i + 1;
			break ;
		}
		i++;
	}
	len = var_length(str);
	var = ft_substr(str, start, len);
	if (!var)
		return (NULL);
	tmp = ft_strjoin(var, "=");
	free_ptr((void **)&var);
	var = tmp;
	return (var);
}

char	*get_new_token_string(char *oldstr, char *var_value,
		int newstr_size, int index)
{
	int		i;
	int		j;
	char	*new_str;

	i = 0;
	j = 0;
	new_str = malloc(sizeof(char) * newstr_size);
	if (!new_str)
		return (NULL);
	while (oldstr[i])
	{
		if (oldstr[i] == '$' && i == index)
		{
			copy_var_value(new_str, var_value, &j);
			i = i + var_length(oldstr + index) + 1;
			if (oldstr[i] == '\0')
				break ;
		}
		new_str[j++] = oldstr[i++];
	}
	new_str[j] = '\0';
	return (new_str);
}
