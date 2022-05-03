/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcariot <emcariot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 14:26:42 by emcariot          #+#    #+#             */
/*   Updated: 2022/05/03 19:53:32 by emcariot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe_position(t_token *token, t_cmd *cmd)
{
	(void)cmd;
	if (token->next->token != WORD && token->next != NULL)
		ft_putstr_fd("error syntax \n", 2);
	if (token->prev == NULL)
		ft_putstr_fd("error syntax \n", 2);
	return (0);
}
