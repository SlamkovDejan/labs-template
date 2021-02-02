#include <stdio.h>
#include <stdlib.h>

void solution(FILE *input_test){
    // code of the solution for the problem goes here
    // always put '\n' on the last executed printf()

    // also, trail every scanf(args) with a fprinf(input_test, args) which will write the input
    // read with the scanf(args) to the 'input_test.txt' file

    int a, b, c;
    scanf("%d %d %d", &a, &b, &c);
    fprintf(input_test, "%d %d %d\n", a, b, c); // the same 'args' as scanf, only without '&' and a '\n' at the end

    printf("Result: %d\n", a + b + c);
}

// don't change anything below this line

// constants for annotating test cases in I/O files
#define START "STARTTEST\n"
#define END "ENDTEST\n"

// opens a file with given permissions and displays an error message if the file can't be opened for some reason
FILE* open_file(char *file_name, char *permissions);

// reopens a file with given permissions, i.e. assigns a different file to an already existing stream; used for redirecting stdin/stdout
// displays an error message if the file can't be reopened for some reason
FILE* reopen_file(char *new_file_name, char *permissions, FILE *old_file, char *error_message);

// closes a file and displays an error message if the file can't be closed for some reason
void close_file(FILE *file, char *error_message);

// generates 'input_test.txt', 'expected.txt', and optionally 'input.txt'
int main()
{
    // redirecting stdin and stdout
    FILE *newin, *newout, *input_test;
    newin = reopen_file("input.txt", "r", stdin, "Error redirecting the std input stream to file!");
    newout = reopen_file("expected.txt", "w", stdout, "Error redirecting the output stream to file!");
    input_test = open_file("input_test.txt", "w");

    int num_of_tests;
    scanf("%d", &num_of_tests);

    // creating 'expected.txt' and 'input_test.txt' from 'input.txt'
    for(int i=0; i<num_of_tests; i++){
        printf("%s", START);
        fprintf(input_test, "%s", START);
        solution(input_test);
        fprintf(input_test, "%s", END);
        printf("%s", END);
    }

    close_file(newin, "Error closing the file std input stream!");
    close_file(newout, "Error closing the file std output stream!");
    close_file(input_test, "Error closing file: 'input_test.txt'");

    // redirecting stdout from 'output.txt' back to the screen/console
    reopen_file("CONOUT$", "w", stdout, "Error redirecting back to console stdout!");
    printf("Done!\n");

    return 0;
}

FILE* open_file(char *file_name, char *permissions){
    FILE *file;
    errno_t err = fopen_s(&file, file_name, permissions);
    if(err){
        fprintf(stderr, "Error opening file: %s!\n", file_name);
        exit(-1);
    }
    return file;
}

FILE* reopen_file(char *new_file_name, char *permissions, FILE *old_file, char *error_message){
    FILE *new_file_stream;
    errno_t err = freopen_s(&new_file_stream, new_file_name, permissions, old_file);
    if(err){
        fprintf(stderr, "%s\n", error_message);
        exit(-1);
    }
    return new_file_stream;
}

void close_file(FILE *file, char *error_message){
    int fails = fclose(file);
    if(fails){
        fprintf(stderr, "%s\n", error_message);
        exit(-1);
    }
}
