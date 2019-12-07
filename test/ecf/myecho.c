#include <unistd.h>
#include <stdio.h>
int main(int argc, char *argv[], char *envp[])
{
    printf("Command-line arguments:\n");
    for (int i = 0; i < argc; i++) {
        printf("    argv[ %d]: %s\n", i, argv[i]);
    }
    
	printf("Environment variables: \n");
    for (int i = 0; envp[i] != NULL; i++) {
    	printf(".   evnp[ %d]: %s\n", i, envp[i]);
    }
    
    printf("Command-line arguments' number: %d\n", argc);
    
    return 0;
}
