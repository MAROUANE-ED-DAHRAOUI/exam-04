/*
Allowed functions:
pipe, fork, dup2, execvp, close, exit, wait

Write the following function:

	int picoshell(char **cmds[])

It has to simulate the pipe. cmds[i] contains the command with its arguments.
Use execvp(cmds[i][0], cmds) to execute a command.
It has to return 1, if a command fail. Do not leak file descriptors.
In the exam, a main file will be given to test your picoshell
*/

#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

int picoshell(char **cmds[])
{
	int i = 0;
	int prev_fd = -1;
	int fd[2];
	pid_t pid;
	int status;
	int ret = 0;

	while (cmds[i]) {
		if (cmds[i + 1] && pipe(fd) == -1)
			return 1;

		pid = fork();
		if (pid == -1) {
			if (cmds[i + 1]) {
				close(fd[0]);
				close(fd[1]);
			}
			return 1;
		}

		if (pid == 0) 
		{
			if (prev_fd != -1) 
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd);
			}

			if (cmds[i + 1]) 
			{
				close(fd[0]);
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execvp(cmds[i][0], cmds[i]);
			exit(EXIT_FAILURE);
		}

		if (prev_fd != -1)
			close(prev_fd);
		if (cmds[i + 1]) {
			close(fd[1]);
			prev_fd = fd[0];
		}
		i++;
	}

	while (wait(&status) > 0) {
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
			ret = 1;
	}
	return ret;
}

int main() {
    char *cmd1[] = {"echo", "'squalala!'",NULL};
	char *cmd2[] = {"cat", NULL};
	char *cmd3[] = {"sed", "s/a/b/g",NULL};
    char **cmds[] = {cmd1, cmd2, cmd3, NULL};

    printf("Test 3:\n");
    int result = picoshell(cmds);
    printf("Return: %d \n", result);
    return 0;
}
