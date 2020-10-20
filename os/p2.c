#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#define MAX_CHARS           128

static void input_clean_stdin (void) {

	int c = 0;
	do {
		c = getchar ();
	} while (c != '\n' && c != EOF);

}

// returns a newly allocated c string
static char *input_get_line (void) {

	size_t lenmax = 128, len = lenmax;
	char *line = (char *) malloc (lenmax), *linep = line;

	if (line) {
		int c = 0;

		for (;;) {
			c = fgetc (stdin);

			if (c == EOF || c == '\n') break;

			if (--len == 0) {
				len = lenmax;
				char * linen = (char *) realloc (linep, lenmax *= 2);

				if(linen == NULL) {
					free (linep);
					return NULL;
				}

				line = linen + (line - linep);
				linep = linen;
			}

			if ((*line++ = c) == '\n') break;
		}

		*line = '\0';
	}

	return linep;

}

static char *get_dir (const char *base_dir) {

	char *retval = NULL;
	char *dirname = strdup (base_dir);

	char command[MAX_CHARS] = { 0 };

	// checar si ya existe la carpeta
	do {
		int statval = 0;
		if (fork () == 0) {
			sprintf (command, "stat %s", dirname);
			execlp ("sh", "sh", "-c", command, NULL);
		}
		else {
			wait (&statval);
			if (WIFEXITED (statval)) {
				if (WEXITSTATUS (statval) == 0) {
					// dirname exists!
					input_clean_stdin ();
					printf ("\n\nEl directorio %s ya existe, introduce otro: ", dirname);
					free (dirname);
					dirname = input_get_line ();
					printf ("\n");
				}

				else {
					retval = strdup (dirname);
					free (dirname);
					break;
				}
			}

			else {
				printf ("Child did not terminate with exit\n");
			}
		}
	} while (1);

	return retval;

}

int main (int argc, char **argv) {

	int pidPadre = 0;
	int pid = 0;
	int status = 0;
	int hijo1 = 0;
	int hijo2 = 0;
	int hijo3 = 0;
	char commline[MAX_CHARS] = { 0 };
	pidPadre = getpid ();

	if (argc >= 3) {
		char *dirname = get_dir (argv[1]);

		// Primera bifurcación: Crear una carpeta
		pid = fork ();
		switch (pid) {
			case -1:
				perror("Error en creacion del hijo\n");
				exit(-1);
			case 0:
				sprintf(commline,"mkdir %s", dirname);
				execlp("sh","sh","-c",commline,NULL);
				perror("Error de execvp\n");
				exit(-1);
				break;
			default:
				//El padre espera hasta que termine el hijo
				while(wait(&status) != pid);
				if(status == 0){
					printf ("\nSe ha creado el directorip: %s\n", dirname);
					printf("Ejecucion normal del hijo %d\n", pid);
					hijo1=pid;
				}
				else{
				printf("Error del hijo %d\n", pid);
				}
		}

		//Segunda bifurcación: Crear una archivo
		pid = fork();
		switch(pid)
		{
			case -1:
				perror("Error en creacion del hijo\n");
				exit(-1);
			case 0:
				sprintf(commline,"touch %s/%s", dirname,  argv[2]);
				execlp("sh","sh","-c",commline,NULL);
				perror("Error de execvp\n");
				exit(-1);
				break;
			default:
				//El padre espera hasta que termine el hijo
				while(wait(&status) != pid);
				if(status == 0){
					printf("Ejecucion normal del hijo %d\n", pid);
					hijo2=pid;}
				else{
					printf("Error del hijo %d\n", pid);}
		}
		//Tercera bifurcacion: Impresion ls-l
		pid = fork();
		switch(pid)
		{
			case -1:
				perror("Error en creacion del hijo\n");
				exit(-1);
			case 0:
				sprintf(commline, "ls -l > %s/%s", dirname, argv[2]);
				execlp("sh","sh","-c",commline,NULL);
				perror("Error de execvp");
				exit(-1);
				break;
			default:
				//El padre espera hasta que termine el hijo
				while(wait(&status) != pid);
				if(status == 0){
					printf("Ejecucion normal del hijo %d\n", pid);
					hijo3=pid;}
				else{
					printf("Error del hijo %d\n", pid);}
		}
		
		// Impresion de datos
		sprintf(commline,"echo soy el proceso padre %d >> %s/%s",pidPadre, dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo el proceso %d creo el directorio >> %s/%s",hijo1,dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo el proceso %d creo el archivo >> %s/%s",hijo2, dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo el proceso %d escribio en el >> %s/%s",hijo3, dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo Quiroz Tapia Aramis >> %s/%s", dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo Liam Daniel Villa Vázquez >> %s/%s", dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo Erick Salas Romero >> %s/%s", dirname, argv[2]);
		system(commline);
		sprintf(commline,"echo Uriel Castañeda Gomez >> %s/%s", dirname, argv[2]);
		system(commline);

		free (dirname);
	}

	return 0;

}