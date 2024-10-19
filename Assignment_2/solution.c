#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "merkle_tree.h"
#include <stdio.h>

#define MAX_NAME_LEN 100
// Function to clean the student name by removing quotes and commas
char* clean_variable(const char* name) {
    int length = strlen(name);
    char* clean_name = malloc(length + 1);
    int j = 0;

    // Loop through the input string
    for (int i = 0; i < length; i++) {
        if (name[i] != '"' && name[i] != ',') {  // Ignore quotes and commas, but keep spaces
            clean_name[j++] = name[i];
        }
    }

    clean_name[j] = '\0';  // Null-terminate the cleaned string
    printf("Name: %s\n",clean_name);
    return clean_name;
}

void print_merkle_proof(MerkleProof *proof, const char *student_name) {
    if (proof == NULL) {
        printf("Student '%s' not found in the Merkle Tree.\n", student_name);
        return;
    }

    printf("Proof for '%s':\n", student_name);
    for (int i = 0; i < proof->count; i++) {
        char *hash_str = hash_to_string(proof->siblings[i]);
        printf("Level %d sibling hash: %s (Direction: %s)\n",
               i + 1,
               hash_str,
               proof->directions[i] == 1 ? "Left" : "Right");
        free(hash_str);
    }
}

int main() {
    FILE *file = fopen("students.txt", "r");
    if (!file) {
        perror("Failed to open students.txt");
        return EXIT_FAILURE;
    }

    //create tree
    MerkleTree *tree = create_merkle_tree();

    char line[MAX_NAME_LEN];

    printf("Strings that will be hashed:\n");
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = '\0';
        add_element(tree, line);
    }

    fclose(file);

    printf("\n---------------------------------------------------------\n");

    printf("\nMerkle Tree structure:\n\n");
    print_tree(tree);// print tree

    printf("\n---------------------------------------------------------\n");

    char student_name[MAX_NAME_LEN]; // user input for the name
    printf("\nEnter the student name (First Last) to generate proof: ");
    fgets(student_name, sizeof(student_name), stdin);
    printf("\n");

    student_name[strcspn(student_name, "\n")] = '\0';//remove newline

    MerkleProof *proof = generate_proof(tree, student_name);// generate proof

    print_merkle_proof(proof, student_name);//print proof

    printf("\n---------------------------------------------------------\n");

    const unsigned char *root_hash = get_root_hash(tree);

    if (verify_proof(root_hash, student_name, proof)) {
        printf("\nThe proof for '%s' is valid!\n", student_name);
    } else {
        printf("\nThe proof for '%s' is invalid.\n", student_name);
    }

    free_merkle_proof(proof);// free proof

    free_merkle_tree(tree);// free merkle tree

    return EXIT_SUCCESS;
}
