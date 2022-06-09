/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_plan_bis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbascuna <mbascuna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/08 22:06:55 by emcariot          #+#    #+#             */
/*   Updated: 2022/06/09 09:03:43 by mbascuna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../minishell.h"


int	check_redir_in(t_token *token, t_cmd *cmd)
{
	if (token->token == REDIR_IN)
	{
		if (check_redir_i_position(token, cmd) == 1)
			return (1);
		token = token->next;
		if (check_access(cmd, token->val))
		{
			perror(token->val);
			ft_lst_clear2(&cmd, free);
			return (1);
		}
	}
	return (0);
}

int	check_redir_out(t_token *token, t_cmd *cmd)
{
	if (token->token == REDIR_OUT)
	{
		if (check_redir_o_position(token, cmd) == 1)
			return (1);
		token = token->next;
		if (check_ambiguious_args(token->val, cmd))
		{
			ft_error("ambiguous redirect", 2);
			ft_lst_clear2(&cmd, free);
			return (1);
		}
		else
			redir_out(cmd, token->val);
	}
	return (0);
}

int	ana_append_in(t_token *token, t_cmd *cmd)
{
	if (token->token == APPEND_IN)
	{
		if (!check_heredoc(token, cmd))
		{
			token = token->next;
			ft_heredoc(token->val);
		}
		else
		{
			ft_error("Syntax error", 2);
			ft_lst_clear2(&cmd, free);
			printf("error\n");
			return (1);
		}
	}
	return (0);
}

int	ana_append_out(t_token *token, t_cmd *cmd)
{
	if (token->token == APPEND_OUT)
	{
		if (!check_append_o(token, cmd))
		{
			token = token->next;
			append_out(cmd, token->val);
		}
		else
		{
			ft_error("Syntax error", 2);
			ft_lst_clear2(&cmd, free);
			printf("error\n");
			return (1);
		}
	}
	return (0);
}

t_token	*ft_fill_cmdval(t_cmd *cmd, t_token *token)
{
	int	i;

	i = 0;
	while (token->token == WORD)
	{
		cmd->expand[i] = 0;
		if (token->expand)
			cmd->expand[i] = 1;
		cmd->val[i] = ft_strdup(token->val);
		token = token->next;
		i++;
	}
	cmd->val[i] = NULL;
	return (token);
}

// t_token	*find_redir(t_token *token, t_cmd *cmd)
// {
// 	if (token->token == REDIR_IN)
// 		token = ana_redir_in(token, cmd);
// 	else if (token->token == REDIR_OUT)
// 		token = ana_redir_out(token, cmd);
// 	else if (token->token == APPEND_IN)
// 		token = ana_append_in(token, cmd);
// 	else if (token->token == APPEND_OUT)
// 		token = ana_append_out(token, cmd);
// 	return (token);
