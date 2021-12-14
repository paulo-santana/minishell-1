#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <errno.h>

#define MAXCOM 1000
#define MAXLIST 100

#define clear() printf("\033[H\033[J")

int	take_input(char *str)
{
	char *buf;
	
	buf = readline("$ ");
	if (strlen(buf) != 0)
	{
		add_history(buf);
		strcpy(str, buf);
		return 0;
	}
	return 1;
}

void	pwd(void)
{
	char	pwd[2048];
	
	getcwd(pwd, sizeof(pwd));
	printf("%s\n", pwd);
}

void	exec_args(char **parsed)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork: ");
		exit(errno);
	}
	else if (pid == 0)
	{
		if (execvp(parsed[0], parsed) < 0)
		{
			perror("execvp: ");
		}
		exit(0);
	}
	else
	{
		wait(NULL);
		return ;	
	}

}

int	cmd_handler(char **parsed)
{
	int		cmd_num;
	int		i;
	int		cmd_index;
	char	*cmd_lst[3];
	char	*user;

	i = 0;
	cmd_num = 3;
	cmd_lst[0] = "pwd";
	cmd_lst[1] = "cd";
	cmd_lst[2] = "exit";
	while (i < cmd_num)
	{
		if (!strcmp(parsed[0], cmd_lst[i]))
		{
			cmd_index = i + 1;
			break ;
		}
		i++;
	}
	if (cmd_index == 1)
	{
		pwd();
		return (0);
	}
	else if (cmd_index == 2)
	{
		if (chdir(parsed[1]));
			return (0);
	}
	else if (cmd_index == 3)
		exit(0);
	return (1);
}

void	parse_args(char *str, char **parsed)
{
	int	i;

	i = 0;
	while (i < MAXLIST)
	{
		parsed[i] = strsep(&str, " ");
		if (parsed[i] == NULL)
			break;
		if (!strlen(parsed[i]))
			i--;
		i++;
	}
}

int	parse_str(char *str, char **parsed)
{
	parse_args(str, parsed);
	if (!cmd_handler(parsed))
		return (0);
	return (1);
}

int main(void)
{
	char	str[MAXCOM];
	char	*parsed[MAXLIST];
	int		i;

	clear();
	i = 0;
	while (1)
	{
		pwd();
		if (take_input(str))
			continue ;
		if (parse_str(str, parsed))
		{
			exec_args(parsed);
		}
	}
	return (0);
}
