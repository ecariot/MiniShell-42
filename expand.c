/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcariot <emcariot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 18:26:40 by emcariot          #+#    #+#             */
/*   Updated: 2022/06/08 09:54:32 by emcariot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_expand_cmd(t_global *global, t_cmd *cmd, char **split_path)
{
	t_env	*env;
	char	*tmp;

	env = find_name(&global->head_env,
			&cmd->val[0][1], ft_strlen(&cmd->val[0][1]));
	if (!env)
	{
		ft_free_tab(global->env);
		ft_free_tab(split_path);
		perror("Command not Found");
		return (1);
	}
	tmp = ft_strdup(env->var_value);
	if (ft_search_builtin(cmd, tmp, global) == 0)
		ft_free_tab(split_path);
	else
	{
		cmd->path = find_binary(split_path, env->var_value);
		if (!cmd->path)
			ft_expand_echo(cmd, global, cmd->val[0]);
	}
	free(tmp);
	ft_free_list(global);
	return (0);
}

void	ft_expand_args(t_global *global, t_cmd *cmd, int i)
{
	t_env	*env;

	env = find_name(&global->head_env,
			&cmd->val[i][1], ft_strlen(&cmd->val[i][1]));
	if (env)
		ft_strcpy(cmd->val[i], env->var_value);
}

char **split_expand(char *str)
{
	char **split;
	char *tmp;
	int i;
	int j;
	int words = 0;

	i = 0;
	str = ft_strtrim(str, "\"$");
	while (str[i])
	{
		if ((ft_isalnum(str[i]) && str[i + 1] == '\'') || (ft_isalnum(str[i]) && !str[i + 1]))
			words++;
		else if (str[i] == '\'')
			words++;
		i++;
	}
	split = (char **)malloc(sizeof(char *) * (words + 1));
	i = 0;
	j = 0;
	while (i < words)
	{
		j += i;
		while (ft_isalnum(str[j]))
			j++;
		tmp = ft_strdup_bis(&str[j - (j - i)], j - i);
		split[i] = ft_strtrim(tmp, "$\"");
		free(tmp);
		if (str[j] == '\'')
			split[++i] = ft_strdup("\'");
		i++;
	}
	free(str);
	split[words] = NULL;
	return (split);
}

void ft_expand_echo(t_cmd *cmd, t_global *global, char *str)
{
	t_env *env;
	char **split;
	int i;

	env = global->head_env;
	if (cmd->val[1] != NULL && cmd->val[1][1] == '?')
		printf("%d", g_exit_status);
	else if (str[0] == '$' && !str[1])
	{
		ft_lst_clear3(&global->head_env, free);
		ft_lst_clear(&global->head, free);
		ft_lst_clear2(&global->headcmd, free);
		ft_error("Command not found", NOTFOUND);
	}
	else
	{
		split = split_expand(str);
		i = 0;
		while (split[i])
		{
			env = find_name(&global->head_env, split[i], ft_strlen(split[i]));
			if (env)
				printf("%s", env->var_value);
			i++;
		}
		ft_free_tab(split);
	}
}

void	ft_expand_cmd_first(t_global *global)
{
	t_cmd *cmd;
	char **split;
	t_env *env;
	int i;
	int j;

	cmd = global->headcmd;
	while (cmd->next)
	{
		if (!ft_strchr(cmd->val[0], '$'))
			break ;
		if (!cmd->val[1])
		{
			split = ft_split_many(cmd->val[0], "$\"");
			if ((split[0] != NULL && split[1] == NULL) || split[0] == NULL)
			{
				ft_free_tab(split);
				break ;
			}
		}
		else
			break ;
		i = 0;
		while (split[i] != NULL)
		{
			env = find_name(&global->head_env, split[i], ft_strlen(split[i]));
			if (env && i == 0)
			{
				free(split[0]);
				split[0] = ft_strdup(env->var_value);
				// split[i] = env->var_value;
			}
			else if (env && i > 0)
				split[0] = ft_strjoin(split[0], env->var_value);
			else if (i != 0)
				split[0] = ft_strjoin(split[0], split[i]);
			i++;
		}
		ft_free_tab(cmd->val);
		cmd->val = ft_split(split[0], ' ');
		ft_free_tab(split);
		j = 0;
		if (cmd->val[1] != NULL)
		{
			while (cmd->val[j])
			{
				cmd->expand[j] = 0;
				j++;
			}
		}
		cmd = cmd->next;
	}
}
