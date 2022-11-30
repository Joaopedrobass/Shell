/*
	TODO O CORE DO SHELL NESSE FONTE FOI ESCRITO PELO STEPHEN BRENNAN
	https://brennan.io/2015/01/16/write-a-shell-in-c/
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

#define GRM_RL_BUFSIZE 1024
#define GRM_TOK_DELIM " _\t\r\n\a"
#define GRM_TOK_BUFSIZE 64

/* Pre Definicao de Funcoes */
int grm_cd(char **args);
int grm_help(char **args);
int grm_exit(char **args);
int grm_clear(char **args);
int grm_mkdir(char **args);
int grm_list(char **args);
int grm_del(char **args);
int grm_fcreate(char **args);
int grm_fread(char **args);
int grm_fwrite(char **args);

/* Variaveis publicas */
char cwd[256]; //Guarda diretorio atual

const char *builtin_str[] = {
	"cd",
	"help",
	"clear",
	"mkdir",
	"list",
	"del",
	"exit",
	"c",
	"r",
	"w"
};

const char *builtin_help[] = {
	"cd (argument)",
	"help {argument}",
	"mkdir (argument)",
	"list {argument}",
	"del (argument)",
	"c (argument)",
	"r (argument)",
	"w (argument)",
	"clear",
	"exit"
};	

int (*builtin_func[]) (char **) = {
	&grm_cd,
	&grm_help,
	&grm_clear,
	&grm_mkdir,
	&grm_list,
	&grm_del,
	&grm_exit,
	&grm_fcreate,
	&grm_fread,
	&grm_fwrite
};

/* Retorna o numero de funcoes do Shell */
int grm_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char *);
}

/* Preenche um arquivo informado */
int grm_fwrite(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "grm: Arquivo invalido.\n");
	}else{
		FILE *fl;
		
		fl = fopen(args[1], "w");
		
		fwrite(args[2], 1, sizeof(args[2]), fl); //Substitui o que esta no arquivo
		
		fclose(fl);
	}
	
	return 1;
}

/* Le o conteudo de um arquivo */
int grm_fread(char **args)
{
	if (args[1] == NULL){
		fprintf(stderr, "grm: Arquivo invalido.\n");
	}else{
		FILE *fl;
		char conteudo[GRM_RL_BUFSIZE];
		
		fl = fopen(args[1], "rb");
		
		fread(&conteudo, sizeof(char), GRM_RL_BUFSIZE, fl);
		
		printf("%s\n", conteudo);
		
		fclose(fl);
	}
	
	return 1;
}

/* Cria um arquivo no diretorio alvo */
int grm_fcreate(char **args)
{
	FILE *fl;
	
	if (args[1] != NULL){
		fl = fopen(args[1], "w+");
		if (fl == NULL){
			perror("errno");
		}
	}else{
		perror("grm");
	}
	
	return 1;
}

/* Deleta uma pasta/arquivo do diretorio alvo */
int grm_del(char **args)
{
	if (remove(strcat(cwd, args[1])) < 0)
		perror("errno");
	
	return 1;
}

/* Lista arquivos de uma pasta */
int grm_list(char **args)
{
	DIR *d;
	struct dirent *dir;
	
	/* Verifica se usuario passou algum argumento */
	if (args[1] == NULL)
		d = opendir(".");
	else
		d = opendir(strcat(cwd, args[1]));
	
	if (d)
	{
		while((dir = readdir(d)) != NULL)
			printf("%s\n", dir->d_name);
		
		closedir(d);
	}
	
	return 1;
}

/* Cria um diretorio na pasta atual */
int grm_mkdir(char **args)
{
	int check;
	
	check = mkdir(args[1], S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	if (check < 0)
		perror("errno");
	
	return 1;
}

/* Limpa a tela ao exemplo do clear(linux) e cls(windows) */
int grm_clear(char **args)
{
	system("clear");
	
	return 1;
}

/* Modifica o diretorio */
int grm_cd(char **args)
{
	if(args[1] == NULL)
	{
		fprintf(stderr, "grm: expected argument to \"cd\"\n");
	}else{
		if(chdir(args[1]) != 0)
		{
			perror("grm");
		}
	}
	
	return 1;
}

/* Imprime Help em tela */
int grm_help(char **args)
{
	int i;
	
	if (args[1] != NULL){
		if (strcmp(args[1], builtin_str[0]) == 0){
			printf("Para mudar o diretorio a uma pasta especifica\n");
			printf("cd nome_da_pasta\n\n");
			printf("Para muda para o parent directory\n");
			printf("cd ..\n");
		}else if (strcmp(args[1], builtin_str[2]) == 0){
			printf("Limpa todos os outputs realizados no shell\n");
		}else if (strcmp(args[1], builtin_str[3]) == 0){
			printf("Cria uma pasta no diretorio alvo.\n");
			printf("Ex.: mkdir teste\n");
		}else if (strcmp(args[1], builtin_str[4]) == 0){
			printf("Lista todos os arquivos em um diretorio alvo.\n");
			printf("Ex.: list /meu_jogo/assets\n");
		}else if (strcmp(args[1], builtin_str[5]) == 0){
			printf("Deleta a pasta/arquivo alvo.\n");
			printf("Ex.: del /meu_jogo/assets/img.bmp\n");
		}else if (strcmp(args[1], builtin_str[6]) == 0){
			printf("Encerra o shell e todos os seus processos.\n");
		}else if (strcmp(args[1], builtin_str[7]) == 0){
			printf("Cria um arquivo no diretorio alvo.\n");
			printf("Ex.: c teste.txt\n");
		}else if (strcmp(args[1], builtin_str[8]) == 0){
			printf("Lê o conteudo de um arquivo.\n");
			printf("Ex.: r teste.txt\n");
		}else if (strcmp(args[1], builtin_str[9]) == 0){
			printf("Escreve no arquivo determinado, substituindo seu conteudo.\n");
			printf("Ex.: w teste.txt ola,mundo.\n");
		}else{
			printf("Digite um valor valido.\n");
		}
	}else{
		printf("Graymold Shell\n");
		printf("Digite o nome do comando e aperte Enter.\n");
		printf("Os seguintes comandos estao disponiveis: \n");
		
		for(i = 0; i < grm_num_builtins(); i++)
		{
			printf("- %s\n", builtin_help[i]);
		}
	}
	
	return 1;
}

int grm_exit(char **args)
{
	return 0;
}

int grm_execute(char **args)
{

	int i;
	
	if(args[0] == NULL)
	{
		return 1;
	}
	
	for(i = 0; i < grm_num_builtins(); i++)
	{
		if(strcmp(args[0], builtin_str[i]) == 0)
		{
			return (*builtin_func[i])(args);
		}
	}
	
	printf("Digite um comando valido\n");
	
	return grm_launch(args);
}

int grm_launch(char *args)
{
	pid_t pid, wpid;
	int status;
	
	pid = fork();
	
	if(pid == 0)
	{
		if(execvp(args[0], args) == -1)
		{
			perror("grm");
		}
		exit(EXIT_FAILURE);
	}else if(pid < 0){
		perror("grm");
	}else{
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	
	return 1;
}

char **grm_split_line(char *line)
{
	int bufsize = GRM_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;
	
	if(!tokens)
	{
		fprintf(stderr, "grm: allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	token = strtok(line, GRM_TOK_DELIM);
	
	while(token != NULL)
	{
		tokens[position] = token;
		position++;
		
		if(position >= bufsize)
		{
			bufsize += GRM_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			
			if(!tokens)
			{
				fprintf(stderr, "grm: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, GRM_TOK_DELIM);
	}
	
	tokens[position] = NULL;
	return tokens;
}

char *grm_read_line(void)
{
	int bufsize = GRM_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;
	
	if(!buffer)
	{
		fprintf(stderr, "grm: allocation error\n");
		exit(EXIT_FAILURE);
	}
	
	while(1)
	{
		c = getchar();
		
		if(c == EOF || c == '\n')
		{
			buffer[position] = '\0';
			return buffer;
		}else{
			buffer[position] = c;
		}
		
		position++;
		
		if(position >= bufsize)
		{
			bufsize += GRM_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			
			if(!buffer)
			{
				fprintf(stderr, "grm: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

void grm_loop(void)
{
	char *line;
	char **args;
	int status;
	
	do{
		/* Checa diretorio para impressao */
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		
			perror("getcwd() error");
		else
			printf("%s | ", cwd);
		
		line = grm_read_line();
		args = grm_split_line(line);
		status = grm_execute(args);
		
		free(line);
		free(args);
	}while(status);

}

int main(int argc, char **argv)
{	
	grm_loop();
	
	return EXIT_SUCCESS;
}
