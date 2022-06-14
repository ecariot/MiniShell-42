/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/07 18:26:40 by emcariot          #+#    #+#             */
/*   Updated: 2022/06/14 10:33:21 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_count_cmd(char *str)
{
	int		i;
	int		words;

	i = 0;
	words = 0;
	while (str[i])
	{
		if ((ft_isalnum(str[i]) && str[i + 1] == '\'')
			|| (ft_isalnum(str[i]) && str[i - 1] == '$')
			|| (ft_isalnum(str[i]) && str[i - 1] == '='))
			words++;
		else if (str[i] == '\'')
			words++;
		i++;
	}
	return (words);
}

char	**split_expand(char *str)
{
	char	**split;
	char	*tmp;
	int		i;
	int		j;
	int		k = 0;
	int		words;

	i = 0;
	str = ft_strtrim(str, "\"");
	words = ft_count_cmd(str);
	split = (char **)malloc(sizeof(char *) * (words + 1));
	j = 0;
	while (k < words)
	{
		if (str[i] == '$' || str[i] == '=')
		{
			j++;
			while (ft_isalnum(str[j]))
				j++;
		}
		tmp = ft_strdup_bis(&str[(i)], j - i);
		split[k] = ft_strtrim(tmp, "\"\'");
		free(tmp);
		if (str[j++] == '\'')
		{
			split[++k] = ft_strdup("\'");
		}
		else
			j--;
		i += j - i;
		k++;
	}
	free(str);
	split[words] = NULL;
	return (split);
}

void	ft_init_expand(t_cmd *cmd)
{
	int	i;

	i = 0;
	if (cmd->val[1] != NULL)
	{
		while (cmd->val[i])
		{
			cmd->expand[i] = 0;
			i++;
		}
	}
}

int	ft_print_val(char **split, t_cmd *cmd)
{
	cmd->expand[0] = 0;
	ft_free_tab(split);
	return (1);
}

int	ft_expand_cmd_first(t_global *global)
{
	t_cmd	*cmd;
	char	**split;

	cmd = global->headcmd;
	while (cmd->next)
	{
		if (!ft_strchr(cmd->val[0], '$') || cmd->val[1] != NULL)
			break ;
		else if (!cmd->val[1])
		{
			split = ft_split_many(cmd->val[0], "$\"");
			if ((split[0] != NULL && split[1] == NULL) || split[0] == NULL)
			{
				return (ft_print_val(split, cmd));
			}
		}
		split = ft_create_cmd(split, global);
		ft_free_tab(cmd->val);
		cmd->val = ft_split(split[0], ' ');
		ft_init_expand(cmd);
		ft_free_tab(split);
		cmd = cmd->next;
	}
	return (0);
}
