/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_quotes.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcariot <emcariot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 13:26:05 by emcariot          #+#    #+#             */
/*   Updated: 2022/05/10 17:23:11 by emcariot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	count_d_quotes(t_token *token)
{
	int	i;
	int count;

	count = 0;
	while (token->token == WORD)
	{
		i = 0;
		while (token->val[i])
		{
			if (is_doble_quotes(token->val[i]))
				count++;
			i++;
		}
		token = token->next;
	}
	return (count);
}


void	recup_count_d_quotes(t_token *token)
{
	int countd;

	countd = count_d_quotes(token);
	if (countd % 2 == 0)
		trim_doble_quotes(token);
	else
		ft_error("Command not found\n");
}

int	count_s_quotes(t_token *token)
{
	int	i;
	int count;
	//printf("coucu\n");
	count = 0;
	i = 0;
	while (token->val[i])
	{
		if (is_simple_quotes(token->val[i]))
			count++;
		i++;
	}
	printf("count = %d\n", count);
	return (count);
}

void	recup_count_s_quotes(t_token *token)
{
	int counts;

	counts = count_s_quotes(token);
	if (counts % 2 == 0)
		token->val = ft_strtrim(token->val, "\'");
	else
		ft_error("Command not found bogoss");
}

void	parse_final_quotes(t_token *token)
{
	int	i;

	i = 0;
	while (token->token == WORD)
	{
		printf("token->val : %c\n", token->val[i]);
		i = 0;
		while (token->val[i])
		{
			if (is_doble_quotes(token->val[i]))
			{
				recup_count_d_quotes(token);
				printf("ok\n");
			}
			if (is_simple_quotes(token->val[i]))
			{
				printf("coucou\n");
				recup_count_s_quotes(token);
			}
			i++;
		}
		token = token->next;
	}
}

