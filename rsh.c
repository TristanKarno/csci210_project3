#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define N 12
#define MAXARGS 20

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

static int tokenize(char *line, char *argv[]) {
	int argc = 0;
	for (char *tok = strtok(line, " \t"); tok; tok = strtok(NULL, " \t")) {
		if (argc < MAXARGS) argv[argc++] = tok;
		else break;
	}
	argv[argc] = NULL;
	return argc;
}

int isAllowed(const char*cmd) {
	// TODO
	// return 1 if cmd is one of the allowed commands
	// return 0 otherwise

	for (int i = 0; allowed[i]; ++i) {
		if (strcmp(cmd, allowed[i]) == 0) return 1;
	}
	
	return 0;
}

static int is_builtin(const char *cmd) {
	return strcmp(cmd, "cd")==0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "help") == 0;
}

static void builtin_help(void) {
	puts("The allowed commands are:");
	puts("1: cp");
	puts("2: touch");
	puts("3: mkdir");
	puts("4: ls");
	puts("5: pwd");
	puts("6: cat");
	puts("7: grep");
	puts("8: chmod");
	puts("9: diff");
	puts("10: cd");
	puts("11: exit");
	puts("12: help");
}

static void builtin_cd(int argc, char *argv[]) {
	if (argc > 2) {puts("-rsh: cd: too many arguments"); return; }
	if (argc == 1) return;
	(void)chdir(argv[1]);
}

static int run_external(char *const argv[]) {
	pid_t pid; 
	int rc = posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ);

	if (rc != 0) return rc;
	int status;
	(void)waitpid(pid, &status, 0);
	return 0;

}

int main() {

    // TODO
    // Add variables as needed

    char line[256];
	char *argv[MAXARGS + 1];

    while (1) {

		fprintf(stderr,"rsh>");

		if (fgets(line,256,stdin)==NULL) continue;

		if (strcmp(line,"\n")==0) continue;

		line[strlen(line)-1]='\0';

		// TODO
		// Add code to spawn processes for the first 9 commands
		// And add code to execute cd, exit, help commands
		// Use the example provided in myspawn.c

		char *p = line;
		while (*p && isspace((unsigned char)*p)) p++;
		if (*p == '\0') continue;

		int argc = tokenize(p, argv);
		if (argc == 0) continue;

		if (is_builtin(argv[0])) {
			if (strcmp(argv[0], "exit") == 0) return 0;
			else if (strcmp(argv[0], "help") == 0) builtin_help();
			else builtin_cd(argc, argv);
		} else if (isAllowed(argv[0])) {
			(void)run_external(argv);
		} else {
			puts("NOT ALLOWED!");
		}

    }
    return 0;
}
