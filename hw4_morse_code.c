#include <stdio.h> //standard I/O library for reading and writing files
#include <stdlib.h> //standard library for dynamic memory allocation
#include <string.h> //string handling functions
#include <ctype.h> //character handling functions like toupper/tolower


//the alphabet and numbers (no special characters)
//char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";



//it is like a container that holds a letter or number and two pointers to make a tree.
typedef struct tree_node {

    char data; //the letter or number that this tree node has.

    struct tree_node *left;      //points to the left branch in the tree.
    struct tree_node *right;     //points to the right branch in the tree.

} tree_node;



//this function makes a new tree node and puts a letter or number inside it.
tree_node *create_node(char data) {

 //asking the computer for some memory to hold our node.
    tree_node* new_node = (tree_node*)malloc(sizeof(tree_node));
  
    if(new_node != NULL) { //if we got the memory:

        new_node->left = NULL;        //setting the left branch to nothing.
        new_node->right = NULL;      //setting the right branch to nothing.
        new_node->data = data;      //puting the data (letter or number) inside the node.
    }
    return new_node; //gives us back the new node.
}


//function prototypes:

//reads user input dynamically allocating memory
char *input();
//encodes text into morse code
void encode_morse(char *input, int *letters, int *dots, int *dashes, int *numbers);
//decodes morse code into text
void decode_morse(tree_node *root, char *input, int *letters, int *dots, int *dashes, int *numbers);
//builds the morse code tree using binary search trees
void build_tree(tree_node *root);
//deletes/frees the morse code tree
void delete_tree(tree_node *root);
//shows char. stat. from input
void char_statistics(int letters, int numbers, int dots, int dashes);
//reset character statistics (back to 0)
void reset_statistics(int *letters, int *numbers, int *dots, int *dashes);



int main(int argc, char *argv[]){

//making the root of the Morse code tree. It starts with nothing inside.
    tree_node *root = create_node('\0');
    build_tree(root);       //building the whole Morse code tree.

//variables to keep track of all the letters, numbers, dots, and dashes that was used.
    int cumulative_letters = 0;
    int cumulative_numbers = 0;
    int cumulative_dots = 0;
    int cumulative_dashes = 0;


// COMMAND LINE SWITCH HANDLING
// Case 1: encode from command line: morse.exe 1 hello world
if (argc >= 3 && strcmp(argv[1], "1") == 0) {

    // Build the full input text from argv[2] onward
    char buffer[1024] = "";

    for (int i = 2; i < argc; i++) {
        strcat(buffer, argv[i]);
        if (i < argc - 1) {
            strcat(buffer, " ");   // add space between the words
        }
    }

    // Encode the full string
    encode_morse(buffer, &cumulative_letters, &cumulative_dots,
                 &cumulative_dashes, &cumulative_numbers);

    // Print statistics AFTER encoding
    char_statistics(cumulative_letters, cumulative_numbers,
                    cumulative_dots, cumulative_dashes);

    // Reset statistics
    reset_statistics(&cumulative_letters, &cumulative_numbers,
                     &cumulative_dots, &cumulative_dashes);

    delete_tree(root);
    return 0;
}


// Case 2: decode from command line: morse.exe 2 .... .-.. ---
if (argc >= 3 && strcmp(argv[1], "2") == 0) {

    // Build the full Morse input from argv[2] onward
    char buffer[1024] = "";

    for (int i = 2; i < argc; i++) {
        strcat(buffer, argv[i]);
        if (i < argc - 1) {
            strcat(buffer, " ");   // keep spaces between Morse groups
        }
    }

    // Decode the full Morse string
    decode_morse(root, buffer, &cumulative_letters, &cumulative_dots,
                 &cumulative_dashes, &cumulative_numbers);

    // Print statistics AFTER decoding
    char_statistics(cumulative_letters, cumulative_numbers,
                    cumulative_dots, cumulative_dashes);

    // Reset statistics
    reset_statistics(&cumulative_letters, &cumulative_numbers,
                     &cumulative_dots, &cumulative_dashes);

    delete_tree(root);
    return 0;
}

// If none of the above matched and argc > 1 → invalid usage
if (argc > 1) {
    printf("Invalid command line usage.\n");
    printf("Usage:\n");
    printf("  %s 1 text        # encode text to morse\n", argv[0]);
    printf("  %s 2 morse_code  # decode morse to text\n", argv[0]);
    delete_tree(root);
    return 0;
}


while(1){   //infinite loop for menu interaction with user

//options for the menu for the user to choose
    printf("Select from the menu:\n");
    printf("1. Encode Morse Code (text to morse)\n");
    printf("2. Decode Morse Code (morse to text)\n");
    printf("3. Character statistics\n");
    printf("4. Exit\n");
    printf("\nMenus item: \n");

    int choice;  //variable to hold the user's choice

    scanf("%d",&choice);
    getchar();  //after reading the user choice, there would be a newline char (\n) left by scanf

    switch(choice){

    case 1:
        printf ("\nEnter a text: \n");
        char *input_1 = input();    //read input from the user
        encode_morse(input_1, &cumulative_letters, &cumulative_dots, &cumulative_dashes, &cumulative_numbers);  //changing the user's words to Morse code.
        printf ("\n");
        free (input_1);     //free allocated memory
        break;

    case 2:

        printf("\nEnter a Morse code: \n");
        char *input_2 = input();     //read Morse code input from user
        decode_morse(root, input_2, &cumulative_letters, &cumulative_dots, &cumulative_dashes, &cumulative_numbers);  //change the Morse code to regular words.
        printf("\n");
        free(input_2);      //free allocated memory
        break;

    case 3:

        char_statistics(cumulative_letters, cumulative_numbers, cumulative_dots, cumulative_dashes);  //show the user how many letters, numbers, dots, and dashes we've used.
        reset_statistics(&cumulative_letters, &cumulative_numbers, &cumulative_dots, &cumulative_dashes);  // Reset cumulative statistics.
        printf("\nStatistics have been reset.\n");  // Inform user of reset.
        break;

    case 4:

        printf("\nExiting program.\n");
        delete_tree(root);  //free allocated memory for Morse tree

        return 0;       //exits the program

    default:
        printf ("\nInvalid choice. Please try again.\n");  //handling invalid choices

    }

}

    return 0; //ending the program successfully :)
}



//function to read input dynamically, character by character
char *input() {

    int cnt = 0;    //counter to keep track of the number of characters read

    char *line = (char *) malloc(sizeof(char) * 1);     //allocating initial memory for line

    if (line == NULL) {     //check if memory allocation failed
        fprintf(stderr, "Error: Unable to allocate memory\n");  //(standard error, print error message)
        exit(1);     //exit the program due to memory allocation failure
    }

    line[0] = '\0';  //set the initial line to be an empty string

    char newchar;   //variable to hold each character read

    while (scanf("%c", &newchar) == 1 && newchar != '\n') {     //reads until newline character is found

        cnt++; //increment character count

        char *newarr = realloc(line, sizeof(char) * (cnt + 1));     //reallocate memory for new character and null terminator (+1)

        if (newarr == NULL) {       //check if memory allocation failed
            fprintf(stderr, "Error: Unable to allocate memory\n");      //(standard error, print error message)
            free(line);         //free previously allocated memory to avoid leaks
            exit(1);        //exit program
        }

        line = newarr;      //point line to the new memory
        line[cnt - 1] = newchar;        //add the new character to the array
        line[cnt] = '\0';       //null-terminate the string
    }

    return line;    //return the dynamically allocated input string
}



//function to encode text into Morse code
void encode_morse(char *input, int *letters, int *dots, int *dashes, int *numbers) {

    //writing the characters in morse code:
    //in morse code characters from A to Z then from 0 to 9

    char *morse[] = {
        ".-", "-...", "-.-.", "-..", ".", "..-.", "--.",  // A-G
        "....", "..", ".---", "-.-", ".-..", "--", "-.",  // H-N
        "---", ".--.", "--.-", ".-.", "...", "-", "..-",  // O-U
        "...-", ".--", "-..-", "-.--", "--..",            // V-Z
        
        "-----", ".----", "..---", "...--", "....-",      // 0-4
        ".....", "-....", "--...", "---..", "----."       // 5-9
    };

    for(int i = 0; i < strlen(input); i++) {

        char c = tolower(input[i]);         //convert character to lowercase for uniform indexing

        if (c >= 'a' && c <= 'z') {         //if character is a letter

            (*letters)++;                   //update letter count

            printf("%s ", morse[c - 'a']);  //print corresponding Morse code

            //update cumulative dot and dash statistics
            for(int j = 0; j < strlen(morse[c - 'a']); j++) {

                if(morse[c - 'a'][j] == '.') {
                    (*dots)++;
                    
                } else if(morse[c - 'a'][j] == '-') {
                    (*dashes)++;
                }
            }

        } else if (c >= '0' && c <= '9') {      //if character is a digit
                
            (*numbers)++;  //updating number count.

            printf("%s ", morse[c - '0' + 26]);      //print corresponding Morse code

            //update cumulative dot and dash statistics
            for(int j = 0; j < strlen(morse[c - '0' + 26]); j++) {

                if(morse[c - '0' + 26][j] == '.') {
                    (*dots)++;

                } else if(morse[c - '0' + 26][j] == '-') {
                    (*dashes)++;
                }
            }

        } else if (c == ' ') {          //if character is a space

            printf("  ");           //print double spaces to indicate separation between words

        } else {            //handle any invalid character

            printf("Invalid character ");
        }
    }
    printf("\n"); //print a newline at the end of the encoded message
}



//function to decode Morse code to text (not yet implemented)
void decode_morse(tree_node *root, char *input, int *letters, int *dots, int *dashes, int *numbers) {
    int i = 0;  //starting at the first character

//create a space to store the decoded message
    char *decoded = (char *)malloc(sizeof(char) * (strlen(input) + 1));  // +1 for ending

    if (decoded == NULL) {

        printf("Memory allocation error.\n");
        return;
    }

    int j = 0;  //where we are in the decoded message
    tree_node *temp = root;  //starting at the top of our Morse tree

//goes through each character in the input
    while (i <= strlen(input)) {

        if (input[i] == '.' || input[i] == '-') {

            //traverse through the tree, based on dots and dashes
            if (input[i] == '.') {

                (*dots)++;  //count the dot

                if (root->left != NULL) {

                    root = root->left;  //move left for dot

                } else {

                    printf("\nInvalid Morse code detected.\n");
                    free(decoded);
                    return;
                }

            } else if (input[i] == '-') {

                (*dashes)++;  //count the dash

                if (root->right != NULL) {

                    root = root->right;  //move right for dash

                } else {

                    printf("\nInvalid Morse code detected.\n");
                    free(decoded);
                    return;
                }
            }

        } else if (input[i] == ' ' || input[i] == '\0') {

            //when we hit a space or the end of input, finish the letter
            if (root != temp) {

                decoded[j++] = root->data;  //add the letter to our message
                
                //count the letter or number accordingly
                if (root->data >= '0' && root->data <= '9') {

                    (*numbers)++;

                } else if (root->data >= 'a' && root->data <= 'z') {

                    (*letters)++;
                }

                root = temp;  //goes back to the top of the tree
            }

            //skip multiple spaces between words
            while (input[i] == ' ' && input[i + 1] == ' ') {

                i++;
            }

        } else {

            //if we find a character that doesn't belong
            printf("\nInvalid Morse code character detected.\n");
            free(decoded);
            return;
        }

        i++;  //move to the next character
    }

    //handle the last character if there is no space at the end
    if (root != temp) {

        decoded[j++] = root->data;

        if (root->data >= '0' && root->data <= '9') {

            (*numbers)++;

        } else if (root->data >= 'a' && root->data <= 'z') {

            (*letters)++;
        }
    }

    decoded[j] = '\0';  //end the message

    //print the decoded message
    printf("The text is: %s\n", decoded);

    //frees the space we used to store the message
    free(decoded);
}

//function to build the morse code tree using binary search trees
void build_tree(tree_node* root){

//creating a binary search tree to store the morse code characte

//level 1 is the node of the tree (the start)

//level 2 of the tree

    root->left = create_node('e');  // .
    root->right = create_node('t'); // -

//level 3

    root->left->left = create_node('i'); // ..
    root->left->right = create_node('a'); // .-

    root->right->left = create_node('n'); // -.
    root->right->right = create_node('m'); // --

//level 4

    root->left->left->left = create_node('s'); // ...
    root->left->left->right = create_node('u'); // ..-

    root->left->right->left = create_node('r'); // .-.
    root->left->right->right = create_node('w'); // .--

    root->right->left->left = create_node('d'); // -..
    root->right->left->right = create_node('k'); // -.-

    root->right->right->left = create_node('g'); // --.
    root->right->right->right = create_node('o'); // ---

//level 5

    root->left->left->left->left = create_node('h'); // ....
    root->left->left->left->right = create_node('v'); // ...-

    root->left->left->right->left = create_node('f'); // ..-.
    root->left->left->right->right = create_node(' '); // ..--

    root->left->right->left->left = create_node('l'); // .-..

    root->left->right->right->left = create_node('p'); // .--.
    root->left->right->right->right = create_node('j'); // .---

    root->right->left->left->left = create_node('b'); // -...
    root->right->left->left->right = create_node('x'); // -..-

    root->right->left->right->left = create_node('c'); // -.-.
    root->right->left->right->right = create_node('y'); // -.--

    root->right->right->left->left = create_node('z'); // --..
    root->right->right->left->right = create_node('q'); // --.-

    root->right->right->right->left = create_node(' '); // ---.
    root->right->right->right->right = create_node(' '); // ----

//level 6

    root->left->left->left->left->left = create_node('5'); // .....
    root->left->left->left->left->right = create_node('4'); // ....-

    root->left->left->left->right->right = create_node('3'); // ...--

    root->left->left->right->right->right = create_node('2'); // ..---

    root->left->right->right->right->right = create_node('1'); // .----

    root->right->left->left->left->left = create_node('6'); // -....

    root->right->right->left->left->left = create_node('7'); // --...

    root->right->right->right->left->left = create_node('8'); // ---..

    root->right->right->right->right->left = create_node('9'); // ----.
    root->right->right->right->right->right = create_node('0'); // -----

//end of the tree
}



//function to delete/free the morse code tree
void delete_tree(tree_node* root){

//deleting the binary search tree and freeing the memory allocated to it

    if (root == NULL){

        return;     //if there's nothing to delete, stop
    }

    delete_tree(root->left);       //delete the left branch
    delete_tree(root->right);      //delete the right branch

    free(root);     //deletes the current node

}



//function to calculate character statistics and save to a file
void char_statistics(int letters, int numbers, int dots, int dashes) {

    // Print the statistics to the screen so the user can see it
    printf("\nCharacter Statistics:\n");
    printf("----------------------\n");
    printf("-> Letters: %d\n", letters); //shows how many letters there are
    printf("-> Numbers: %d\n", numbers); //shows how many numbers there are
    printf("----------------------\n");
    printf("Morse Characters:\n");
    printf("-> Dots: %d\n", dots); //shows how many dots there are
    printf("-> Dashes: %d\n", dashes); //shows how many dashes there are
    printf("----------------------\n");

    
    //save the statistics to a file called 'Statistics.txt'

    FILE *fp = fopen("Statistics.txt", "a");    //opens the file for writing

    if (fp == NULL) {

        //if the file cannot be opened, print an error message and stop
        printf("Error opening statistics file for writing.\n");
        return;
    }

    //writing the same statistics to the file
    fprintf(fp, "\nCharacter Statistics:\n");
    fprintf(fp, "----------------------\n");
    fprintf(fp, "-> Letters: %d\n", letters);
    fprintf(fp, "-> Numbers: %d\n", numbers);
    fprintf(fp, "----------------------\n");
    fprintf(fp, "Morse Characters:\n");
    fprintf(fp, "-> Dots: %d\n", dots);
    fprintf(fp, "-> Dashes: %d\n", dashes);
    fprintf(fp, "----------------------\n");

    fclose(fp);      //close the file after writing
    printf("\nStatistics saved to 'Statistics.txt'.\n\n");    //let the user know the statistics were saved
}



//function to reset all the statistics to zero
void reset_statistics(int *letters, int *numbers, int *dots, int *dashes) {
    *letters = 0;
    *numbers = 0;
    *dots = 0;
    *dashes = 0;
}