#include <libc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#include <stdarg.h>


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
	int 	size2;
	
	size = strlen(str1);
	size2 = strlen(str2);
	res = malloc(sizeof(char) * (size + size2 + 1));
	res[size + size2] = '\0';
	strcpy(res, str1);
	strcat(res, str2);
	return (res);
}

/*
char *concat(int nbr_pieces, ...)
{
	char	*res;
	int		size;
    int		i;
	va_list args;
    va_start(args, nbr_pieces);
	if (nbr_pieces <= 1){
		return NULL;
	}
	i = nbr_pieces;
	size = 0;
	while (i--){
		char *line = va_arg(args, char *);
		size = strlen(str)
	}
	while (nbr_pieces--){
		char *line = va_arg(args, char *);
		printf("%s\n", line);
	}

	
	// size = strlen(str1) + strlen(str2);
	// res = malloc(sizeof(char) * (size + 1));
	// res[size] = '\0';
	// strcpy(res, str1);
	// strcat(res, str2);
	// return (res);
	va_end(args);
	return "test";
}
*/
char *get_kill_command(char *target)
{
	return concat("kill ", target);
}

int		ft_atoi(const char *str, int *k)
{
	long	res;
	int		signe;

	signe = 1;
	res = 0;

	while (str[*k] == '\t' || str[*k] == '\n' || str[*k] == '\r' ||
			str[*k] == '\v' || str[*k] == '\f' || str[*k] == 32)
		(*k)++;
	if (str[*k] == '+' || str[*k] == '-')
	{
		if (str[*k] == '-')
			signe = -1;
		(*k)++;
	}
	while (48 <= str[*k] && str[*k] <= 57)
	{
		res = res * 10 + (str[*k] - 48);
		(*k)++;
	}
	return ((int)(res * signe));
}

int		show_picture(char *picture_name)
{
	pid_t 	pid;
	int		size;
	int		duration;
	int		k;
	
	k = 0;
	pid = fork();
	duration = ft_atoi(picture_name, &k);
	size = strlen(&picture_name[k]);
	/*
		In our case, a '\n' is added at the end of the line because of a break of line 
	*/
	if (picture_name[k + size - 1] == '\n'){
		picture_name[k + size - 1] = '\0';
	}
	if (pid < 0)
	{
		fprintf(stderr, "fork failed");
		return (1);
	}
	else if (pid == 0)
	{
		execlp("open", "open", "-a", "Preview", concat("imgs/", &picture_name[k]), NULL);
		// execlp("open", "open", "-a", "Preview", "imgs/nuitdehors.jpg",NULL);
	}
	else
	{
		wait(NULL);
		sleep(1 * duration);
		int  pid2;
		FILE *fp = popen(concat(concat("lsof -c Preview | grep ", &picture_name[k]), " | cut -d ' ' -f2"), "r");
		// FILE *fp = popen("lsof -c Preview | grep nuitdehors.jpg | cut -d ' ' -f2", "r");
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
	time.tv_nsec = 100000000L;
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
	// concat(3, "abc", "def", "hij");
	// printf(concat("abc", "def", "hij"));
    fd = fopen("script.txt", "r");
	if (fd == 0){
		return (1);
	}
    while ((read = getline(&line, &len, fd)) != -1) {
		if (*line == '#'){
			show_picture(&(line[1]));
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
