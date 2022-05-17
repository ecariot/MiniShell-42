/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/29 14:13:35 by mbascuna          #+#    #+#             */
/*   Updated: 2022/04/29 14:13:54 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void ft_insert_tab(char **tab, char *name, char *value)
{
	int i;
	i = 0;
	(void)value;
	while (tab[i])
		i++;
	tab[i] = ft_strdup(name);
	i++;
	tab[i] = NULL;
}

int	ft_env(t_global *global)
{
	int i;

	i = 0;
	while (global->env[i])
	{
		printf("%s\n", global->env[i]);
		i++;
	}
	return (0);
}
