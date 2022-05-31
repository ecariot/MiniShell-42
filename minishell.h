/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: emcariot <emcariot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/20 12:38:08 by emcariot          #+#    #+#             */
/*   Updated: 2022/05/31 15:58:11 by emcariot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <signal.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <sys/ioctl.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdbool.h>
# include "libft/libft.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


extern int g_exit_status;

typedef enum
{
	REDIR_IN = 1,
	REDIR_OUT = 2,
	APPEND_OUT = 3,
	APPEND_IN = 4,
	WORD = 5,
	NEW_LINE = 6,
	PIPE = 7,
} token_type;

typedef enum
{
	SUCCESS = 0,
	PIPE_FAIL = 3,
	FORK_FAIL = 4,
	ALLOCATION_FAIL = 5,
	SYNTAX_QUOTES = 6,
	SYNTAX_REDIR = 7,
	AMBIGUOUS_REDIR = 8,
	TOOMANY = 24,
	ERROR = 1,
	MISUSE = 2,
	CANTEXEC = 126,
	NOTFOUND = 127,
	CSIGINT = 130
} exit_status;


typedef struct s_token
{
	char	token;
	char	*val;
	int	len;
	int	expand;
	bool	inquotes;
	struct s_token *prev;
	struct s_token *next;
} t_token;

typedef struct s_cmd
{
	int count;
	// int fd_pipe[2];
	char **val;
	int *expand;
	char *path;
	int output;
	int input;
	bool pipe;
	pid_t	pid;
	struct s_cmd *next;
	struct s_cmd *prev;
} t_cmd;


typedef struct s_env
{
	char *declare;
	char *var_name;
	char *var_value;
	char *var_sign;
	struct s_env *prev;
	struct s_env *next;
} t_env;

typedef struct s_global
{
	bool	exit;
	char **env;
	char **sorted_env;
	int exit_status;
	t_env   *head_env;
	t_token *head;
	t_cmd   *headcmd;
}	t_global;


/* PARSING */
int		count_option(char *line);
int		analize_cmd(t_cmd **comd, t_global *g);
void	ft_print_cmd(t_cmd **cmd);
t_cmd	*ft_init_cmd();
void	find_token(t_token *token, t_cmd *cmd);
int		list_len(t_token **head);


// PIPE
int			check_pipe_position(t_token *token, t_cmd *cmd);

//REDIR - PARSE & EXPAND
int		check_redir_o_position(t_token *token, t_cmd *cmd);
int		check_redir_i_position(t_token *token, t_cmd *cmd);
int		check_append_o(t_token *token, t_cmd *cmd);
int		check_heredoc(t_token *token, t_cmd *cmd);
int		redir_out(t_cmd *cmd, char *file_name);
int		redir_in(t_cmd *cmd, char *file_name);
int		append_out(t_cmd *cmd, char *file_name);
int		ft_heredoc(char *lim);
int		check_access(t_cmd *cmd, char *file_name);
int		check_ambiguious_args(char *file_name, t_cmd *cmd);
int		last_check_redir_o(char *file_name, t_cmd *cmd);

//QUOTE - TRIM
int			is_doble_quotes(char c);
int			is_simple_quotes(char c);
void		trim_doble_quotes(t_cmd *cmd);
void		trim_simple_quotes(t_cmd *cmd);
void		trim_global_quotes(t_token *token);

//QUOTE - PARSE
int			count_d_quotes(t_cmd *cmd);
int			count_s_quotes(t_cmd *cmd);
void		recup_count_d_quotes(t_token *token);
void		recup_count_s_quotes(t_token *token);
int			start_with_simple(t_cmd *cmd);
int			start_with_dobles(t_cmd *cmd);
int 		is_empty_string(char *str);
void		delete_quotes(t_cmd *cmd, int i, int j);
void		delete_quotes_bis(t_cmd *cmd, int i);
int			error_quotes(t_cmd *cmd);
int			start_with_dollar(t_cmd *cmd);
int			last_call_quotes(t_cmd *cmd, t_token *token, t_global *global);

/* UTILS */
char		**ft_free_tab(char **tab);
void		ft_error(char *msg, int exit_status);
t_token 	*lstlast(t_token *lst);
void		ft_lst_clear(t_token **lst, void (*del)(void *));
char		*ft_strdup_bis(const char *s1, int len);
void		ft_lstaddback(t_token **alst, t_token *new);
void		ft_lstaddback2(t_cmd **alst, t_cmd *new);
void		ft_lstaddback3(t_env **alst, t_env *new);
t_cmd		*lstlast2(t_cmd *lst);
void		ft_lst_clear2(t_cmd **head, void (*del)(void *));
void		ft_lst_clear3(t_env **head, void (*del)(void *));
void 		ft_lst_insert(t_env **head_env, t_env *new);
int			ft_wrong(char *str);
void ft_lst_delone3(t_env *env, void (*del)(void *));
void	ft_close(t_global *global);

// INIT_ENV
void	ft_init_list_env(t_env **head_env, t_global *global);
void	ft_init_env(t_global *global, char **envp);
t_env  *create_var_env(char *name, char *value);
t_env *ft_init_var_env();
char *init_var_name(char *str, char c);
void	ft_print_export(t_global *global);
char *init_sign(char *name);
void ft_print_env(t_env **head);
char *edit_name(char *str, char c);
void	ft_init_sorted_env(t_global *global);

// PARSE ENV
char	*search_envp(t_env **head_env, char *str);
char	**ft_split_envp(t_env **head_env, char *str);
char *find_binary(char **split_path, char *av);
t_env	*find_name(t_env **head_env, char *var, size_t len);

// SIGNAL
// void siginthandler();
// void signalslash();
void	ft_signal(int i);
void	handle_sigint(int sig);


//LEXER
void	ft_test_lex(char *line, t_global *global);
void init_token_list(char *line, t_token **head);
int ft_lex(char *str, t_token *token);


// EXE
void	ft_execution(t_global *global);
void	ft_exe(t_global *global, t_cmd *cmd);
int	ft_search_builtin(t_cmd *cmd, t_global *global);


// BUILTIN
int	ft_echo(t_cmd * cmd, t_global *global);
int	ft_pwd(void);
int	ft_cd(t_cmd *cmd, t_global *global);
int	ft_env(t_global *global);
int	ft_export(t_cmd *cmd, t_global *global);
void	ft_change_env(char *name, char *value, t_global *global);
int	ft_exit(t_global *global, t_cmd *cmd);
int	ft_unset(t_cmd *cmd, t_global *global);
void ft_insert_tab(char **tab, char *name);

// EXPAND ENV
int check_name(char *token);
char *check_value(char *token);
void ft_expand_cmd_first(t_global *global);
void ft_expand_echo(t_cmd *cmd, t_global *global, char *str);

void parse_execution(t_global *global);
void	ft_free_list(t_global *global);
void ft_expand_cmd(t_global *global, t_cmd *cmd, char **split_path);

#endif
