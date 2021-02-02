#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int my_main(){
    // your code goes here
    // always put '\n' on the last executed printf()

    return 0;
}

// don't change anything below this line

// constants for annotating test cases in I/O files
#define START "STARTTEST\n"
#define END "ENDTEST\n"

// structure for holding the information needed for one test case
typedef struct Test{
    char* input; // the test case input
    char* program_output; // the program output (the one in "my_main()")
    char* expected_output; // the expected output from the program (what "my_main()" should produce)
} Test;

// enumeration for distinguishing the fields in a Test struct
typedef enum Test_Field{
    INPUT, // corresponds with 'char *input' in the Test struct
    OUTPUT, // corresponds with 'char *program_output' in the Test struct
    EXPECTED // corresponds with 'char *expected_output' in the Test struct
} Test_Field;

// opens a file with given permissions and displays an error message if the file can't be opened for some reason
FILE* open_file(char *file_name, char *permissions);

// reopens a file with given permissions, i.e. assigns a different file to an already existing stream; used for redirecting stdin/stdout
// displays an error message if the file can't be reopened for some reason
FILE* reopen_file(char *new_file_name, char *permissions, FILE *old_file, char *error_message);

// closes a file and displays an error message if the file can't be closed for some reason
void close_file(FILE *file, char *error_message);

// reads the contents of a file line by line and stores the information in an array of Test structures
// NOTE: stores the info in a given field of the Test structure
void read_file(FILE *file, Test *tests, Test_Field field);

// runs the test cases on the program and displays the results to the console
void validate(int num_of_tests);

int main()
{
    FILE *newin, *newout;
    // redirecting the stdin(keyboard) and stdout(screen/console) to different files
    newin = reopen_file("input.txt", "r", stdin, "Error redirecting the std input stream to file!");
    newout = reopen_file("output.txt", "w", stdout, "Error redirecting the std output stream to file!");

    int num_of_tests;
    scanf("%d", &num_of_tests);

    // executing the program ("my_main()") 'num_of_tests' times
    for(int i=0; i<num_of_tests; i++){
        // annotating each different output with "STARTTEST" and "ENDTEST" to distinguish the outputs
        // from the different test cases
        printf("%s", START);
        my_main(); // executing the program and writing the output of it to 'output.txt'
        printf("%s", END);
    }

    close_file(newin, "Error closing the file std input stream!");
    close_file(newout, "Error closing the file std output stream!");

    // redirecting stdout from 'output.txt' back to the screen/console
    reopen_file("CONOUT$", "w", stdout, "Error redirecting back to console stdout!");

    validate(num_of_tests);

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

void validate(int num_of_tests){

    FILE *input, *output, *expected;

    // opening 'input_test.txt'
    input = open_file("input_test.txt", "r");

    // opening 'output.txt'
    output = open_file("output.txt", "r");

    // opening 'expected.txt'
    expected = open_file("expected.txt", "r");

    // declaring an array of Test structures
    Test tests[num_of_tests];

    // reading the contents of each of the 3 files
    read_file(input, tests, INPUT); // storing the contents of 'input_test.txt' to the 'input' fileds of the Test array
    read_file(output, tests, OUTPUT); // storing the contents of 'output.txt' to the 'program_output' fileds of the Test array
    read_file(expected, tests, EXPECTED); // storing the contents of 'expected.txt' to the 'expected_output' fileds of the Test array

    int num_passed_tests = 0;
    for(int i=0; i<num_of_tests; i++){ // iterating through every test

        printf("Test case %d:\n", i+1); // displaying the test case order
        printf("Input:\n%s", tests[i].input); // displaying the test case input
        printf("Your output:\n%s", tests[i].program_output); // displaying the output of the program for the test case
        printf("Expected output:\n%s", tests[i].expected_output); // displaying the expected output of the program for the test case

        // checking if the program output matches the expected output(1/0)
        int test_passed = strcmp(tests[i].program_output, tests[i].expected_output) == 0;
        if(test_passed){
            num_passed_tests++;
        }
        printf("Test case passed: %s\n", test_passed == 1 ? "true" : "false"); // displaying if the program passed the test case

        printf("-----------------");
        printf("\n");
    }

    printf("\nTest cases passed: %d/%d\n", num_passed_tests, num_of_tests);
    if(num_of_tests == num_passed_tests)
        printf("Congratulations!! All the tests passed!\n");

    // freeing up the allocated memory
    for(int i=0; i<num_of_tests; i++){
        free(tests[i].input);
        free(tests[i].expected_output);
        free(tests[i].program_output);
    }

    // closing all the associated files
    close_file(input, "Error closing file: input_test.txt");
    close_file(output, "Error closing file: output.txt");
    close_file(expected, "Error closing file: expected.txt");
}

void read_file(FILE *file, Test *tests, Test_Field field){

    int test_order = 0;
    // reading 300B from the file (expecting one line from the file to max 300B)
    const size_t line_size = 300;
    // allocating memory for the line
    char *line = malloc(line_size);

    // the text length of one test case
    size_t text_length;
    // the actual text of one test case
    char *text;

    // going through the file line by line
    while (fgets(line, line_size, file) != NULL)  {
        if(strcmp(line, START) == 0){ // if the line is "STARTTEST"
            // allocate the initial memory needed for the test case contents
            text_length = 2;
            text = malloc(text_length);
            strcpy(text, "");
        }
        else  if(strcmp(line, END) == 0){ // if the line is "ENDTEST"
            // depending on the 'filed' argument, we store the contents of the test to the
            // corresponding Test field in the array
            if(field == INPUT){
                tests[test_order].input = malloc(text_length);
                strncpy(tests[test_order].input, text, text_length);
            }
            else if(field == OUTPUT){
                tests[test_order].program_output = malloc(text_length);
                strncpy(tests[test_order].program_output, text, text_length);
            }
            else if(field == EXPECTED){
                tests[test_order].expected_output = malloc(text_length);
                strncpy(tests[test_order].expected_output, text, text_length);
            }
            free(text);

            test_order++;
        }
        else{
            // if the test case has multiple lines of input/output/expected output
            text_length += strlen(line);
            text = realloc(text, text_length);
            strncat(text, line, text_length);
        }
        free(line);
    }
    free(text);
    free(line);
}
