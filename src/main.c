#include <libc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

enum READ {SAME_LINE, SKIP_LINE};

int		get_itoa_size(num)
{
	int		size;

	size = 1;
	while ((num = num / 10) > 0)
		size++;
	return (size);
}

char* itoa(int num)
{
	char *res;
    int i;

	i = get_itoa_size(num);
	res = malloc(sizeof(char) * (i + 1));
	res[i] = '\0';
	while (num > 0)
	{
		res[--i] = (num % 10) + '0';
		num = num / 10;
	}
    return res;
}

char *concat(char *str1, char *str2)
{
	char	*res;
	int		size;
	
	size = strlen(str1) + strlen(str2);
	res = malloc(sizeof(char) * (size + 1));
	res[size] = '\0';
	strcpy(res, str1);
	strcat(res, str2);
	return (res);
}

char *get_kill_command(char *target)
{
	return concat("kill ", target);
}

int		show_picture()
{
	pid_t pid;
	
	pid = fork();
	if (pid < 0)
	{
		fprintf(stderr, "fork failed");
		return (1);
	}
	else if (pid == 0)
	{
		execlp("open", "open", "-a", "Preview", "imgs/nuitdehors.jpg",NULL);
	}
	else
	{
		wait(NULL);
		sleep(3);
		int  pid2;
		FILE *fp = popen("lsof -c Preview | grep nuitdehors.jpg | cut -d ' ' -f2", "r");
		fscanf(fp, "%d", &pid2);
		pclose(fp);
		system(get_kill_command(itoa(pid2)));
	}
	return (0);
}

void	simulate_typewriter(char *line, int skip_line)
{
	int 	size;
	int		sub_size;
	int		i;
	char	*sub;
	struct timespec time;

	time.tv_sec  = 0;
	time.tv_nsec = 150000000L;
	size = strlen(line);
	i = 0;
	while (i < size)
	{
		sub_size = (size < i) ? size : i;
		sub = malloc(sizeof(char) * (sub_size + 1));
		sub[sub_size] = '\0';
		memcpy(sub, line, sub_size);
		printf("\r%s", sub);
		free(sub);
		sub = 0;
		nanosleep(&time, NULL);
		fflush(stdout);	
		i = i + 1;
	}
	sleep(1);
	if (skip_line == SKIP_LINE){
		printf("\n");
	}
}

int		main()
{
	FILE * fd;
    char *line = NULL;
	size_t len = 0;
    ssize_t read;

    fd = fopen("script.txt", "r");
	if (fd == 0){
		return (1);
	}
    while ((read = getline(&line, &len, fd)) != -1) {
		if (*line == '#'){
			show_picture();
		}else{
        	// printf("%s", line);
			simulate_typewriter(line, SKIP_LINE);
		}
    }
	fclose(fd);
    if (line)
        free(line);
	return (0);
}
