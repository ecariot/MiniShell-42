/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcariot <emcariot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/14 10:26:07 by mbascuna          #+#    #+#             */
/*   Updated: 2022/04/27 10:01:14 by emcariot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_init_env(t_global *global, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	global->env = (char **)malloc(sizeof(char *) * i + 1);
	if(!global->env)
		ft_error("Error\n");
	i = 0;
	while (envp[i])
	{
		global->env[i] = ft_strdup(envp[i]);
		i++;
	}
	global->env[i] = NULL;
}

int	main(int ac, char **av, char **envp)
{
	char *line;
	t_global	global;
	t_token	*head;
	(void)ac;
	(void)av;
	pid_t	pid;

	global.head = NULL;
	ft_init_env(&global, envp);
	// if (signal(SIGINT, siginthandler) == SIG_ERR)
	// 	write(1, "boobs", 6);
	// if (signal(SIGQUIT,signalslash) == SIG_ERR)
	// 	write(1, "marche pas", 11);
	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("\033[01;32m ​💥​ Minishell Happiness ​💥​ ➜ \e[00m");
		if (!line)
		{
			write(1, "\n", 1);
			break ;
		}
		if (line && ft_strncmp(line, "exit", 4) == 0)
			break ;
		add_history(line);
    	init_line(line, &global.head);
		ft_signal(0);
		pid = fork();
		if (pid == 0)
		{
			ft_signal(1);
			ft_exe(&global, line);
		}
		wait(&pid);
    	ft_lst_clear(&global.head, free);
		ft_signal(2);
	}
}

