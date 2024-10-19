#include "merkle_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__APPLE__)
#include <CommonCrypto/CommonDigest.h>
#define SHA256_DIGEST_LENGTH CC_SHA256_DIGEST_LENGTH
#define SHA256(data, len, hash) CC_SHA256(data, len, hash)
#else
#include <openssl/sha.h>
#endif

#define MAX_HASH_LEN 65
#define HASH_SIZE SHA256_DIGEST_LENGTH
#define MAX_HASH_HEX_LENGTH (HASH_SIZE * 2 + 1)

typedef struct Node {
    unsigned char hash[HASH_SIZE];
    struct Node *left;
    struct Node *right;
    struct Node *parent;
} Node;

struct MerkleTree {
    Node *root;
    Node **leaves;
    int leaf_count;
    int capacity;
};

void *sha256_hash(const void *input, size_t length, unsigned char *output) {
    SHA256((const unsigned char *)input, length, output);
}

char* hash_to_string(const unsigned char *hash) {
    char *output = malloc(MAX_HASH_HEX_LENGTH);
    for(int i = 0; i < HASH_SIZE; i++) {
        sprintf(output + (i * 2), "%02x", hash[i]);
    }
    output[MAX_HASH_HEX_LENGTH - 1] = 0;
    return output;
}

Node* create_node(const unsigned char *hash) {
    Node *node = malloc(sizeof(Node));
    memcpy(node->hash, hash, HASH_SIZE);
    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    return node;
}

MerkleTree* create_merkle_tree() {
    MerkleTree *tree = malloc(sizeof(MerkleTree));
    tree->root = NULL;
    tree->leaves = NULL;
    tree->leaf_count = 0;
    tree->capacity = 0;
    return tree;
}

void add_element(MerkleTree *tree, const char *data) {

    char* clean_data = clean_variable(data);  //check string

    unsigned char hash[HASH_SIZE];
    sha256_hash(clean_data, strlen(clean_data), hash);
    free(clean_data);

    Node *new_node = create_node(hash);

    if (tree->leaf_count == tree->capacity) {
        tree->capacity = tree->capacity == 0 ? 1 : tree->capacity * 2;
        tree->leaves = realloc(tree->leaves, tree->capacity * sizeof(Node*));
    }
    tree->leaves[tree->leaf_count++] = new_node;

    // Rebuild the tree
    int n = tree->leaf_count;
    Node **current_level = tree->leaves;
    while (n > 1) {
        int next_level_size = (n + 1) / 2;
        Node **next_level = malloc(next_level_size * sizeof(Node*));
        
        for (int i = 0; i < n - 1; i += 2) {
            Node *left = current_level[i];
            Node *right = current_level[i + 1];
            
            unsigned char combined[HASH_SIZE * 2];
            memcpy(combined, left->hash, HASH_SIZE);
            memcpy(combined + HASH_SIZE, right->hash, HASH_SIZE);
            
            unsigned char parent_hash[HASH_SIZE];
            sha256_hash(combined, HASH_SIZE * 2, parent_hash);
            Node *parent = create_node(parent_hash);
            
            parent->left = left;
            parent->right = right;
            left->parent = parent;
            right->parent = parent;
            
            next_level[i / 2] = parent;
        }
        
        // If there's an odd number of nodes, promote the last node to the next level
        if (n % 2 != 0) {
            next_level[next_level_size - 1] = current_level[n - 1];
        }
        
        if (current_level != tree->leaves) {
            free(current_level);
        }
        current_level = next_level;
        n = next_level_size;
    }
    
    tree->root = current_level[0];
    if (current_level != tree->leaves) {
        free(current_level);
    }
}

bool find_element(MerkleTree *tree, const char *data) {
    unsigned char hash[HASH_SIZE];
    sha256_hash(data, strlen(data), hash);
    for (int i = 0; i < tree->leaf_count; i++) {
        if (memcmp(tree->leaves[i]->hash, hash, HASH_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

void remove_element(MerkleTree *tree, const char *data) {
    unsigned char hash[HASH_SIZE];
    sha256_hash(data, strlen(data), hash);
    for (int i = 0; i < tree->leaf_count; i++) {
        if (memcmp(tree->leaves[i]->hash, hash, HASH_SIZE) == 0) {
            free(tree->leaves[i]);
            for (int j = i; j < tree->leaf_count - 1; j++) {
                tree->leaves[j] = tree->leaves[j + 1];
            }
            tree->leaf_count--;
            // Rebuild the tree
            MerkleTree *new_tree = create_merkle_tree();
            for (int k = 0; k < tree->leaf_count; k++) {
                add_element(new_tree, hash_to_string(tree->leaves[k]->hash));
            }
            free_merkle_tree(tree);
            *tree = *new_tree;
            free(new_tree);
            return;
        }
    }
}

MerkleProof* generate_proof(MerkleTree* tree, const char* data) {

    char* clean_data = clean_variable(data);  // check input
    unsigned char student_hash[HASH_SIZE];
    sha256_hash(clean_data, strlen(clean_data), student_hash);
    free(clean_data);

    // find node corresponding to input
    Node* target_node = NULL;
    for (int i = 0; i < tree->leaf_count; i++) {
        if (memcmp(tree->leaves[i]->hash, student_hash, HASH_SIZE) == 0) {
            target_node = tree->leaves[i];
            break;
        }
    }

    if (target_node == NULL) {
        return NULL;
    }
    // initialize merkleproof struct
    MerkleProof* proof = malloc(sizeof(MerkleProof));
    proof->count = 0;
    proof->siblings = NULL;
    proof->directions = NULL;

    // traverse from target to root
    Node* current = target_node;
    while (current->parent != NULL) {
        proof->count++;
        proof->siblings = realloc(proof->siblings, proof->count * sizeof(unsigned char*));
        proof->directions = realloc(proof->directions, proof->count * sizeof(int));

        Node* sibling;
        if (current->parent->left == current) {
            sibling = current->parent->right;  // sibling on right
            proof->directions[proof->count - 1] = 1;  // Left direction
        } else {
            sibling = current->parent->left;  // sibling on left
            proof->directions[proof->count - 1] = 0;  // Right direction
        }

        if (sibling != NULL) {
            // store sibling's hash
            proof->siblings[proof->count - 1] = malloc(HASH_SIZE);
            memcpy(proof->siblings[proof->count - 1], sibling->hash, HASH_SIZE);
        } else {
            // in case of no siblings ( odd number ) replace with empty string
            proof->siblings[proof->count - 1] = NULL;
        }
        current = current->parent; //go up
    }
    return proof;
}

bool verify_proof(const unsigned char *root_hash, const char *data, MerkleProof *proof) {
    unsigned char current_hash[HASH_SIZE];
    sha256_hash(data, strlen(data), current_hash);
    
    for (int i = 0; i < proof->count; i++) {
        unsigned char combined[HASH_SIZE * 2];
        if (proof->directions[i] == 1) {
            memcpy(combined, current_hash, HASH_SIZE);
            memcpy(combined + HASH_SIZE, proof->siblings[i], HASH_SIZE);
        } else {
            memcpy(combined, proof->siblings[i], HASH_SIZE);
            memcpy(combined + HASH_SIZE, current_hash, HASH_SIZE);
        }
        sha256_hash(combined, HASH_SIZE * 2, current_hash);
    }

    return memcmp(current_hash, root_hash, HASH_SIZE) == 0;
}

void print_node(Node *node, int depth) {
    if (node == NULL) return;
    
    for (int i = 0; i < depth; i++) printf("  ");
    char *hash_str = hash_to_string(node->hash);
    printf("%s\n", hash_str);
    free(hash_str);
    
    print_node(node->left, depth + 1);
    print_node(node->right, depth + 1);
}

void print_tree(MerkleTree *tree) {
    print_node(tree->root, 0);
}

void print_leaf_hashes(MerkleTree *tree) {
    for (int i = 0; i < tree->leaf_count; i++) {
        char *hash_str = hash_to_string(tree->leaves[i]->hash);
        printf("%s\n", hash_str);
        free(hash_str);
    }
}

void free_node(Node *node) {
    if (node == NULL) return;
    free_node(node->left);
    free_node(node->right);
    free(node);
}

void free_merkle_tree(MerkleTree *tree) {
    free_node(tree->root);
    free(tree->leaves);
    free(tree);
}

void free_merkle_proof(MerkleProof *proof) {
    for (int i = 0; i < proof->count; i++) {
        free(proof->siblings[i]);
    }
    free(proof->siblings);
    free(proof->directions);
    free(proof);
}

const unsigned char* get_root_hash(MerkleTree *tree) {
    if (tree && tree->root) {
        return tree->root->hash;
    }
    return NULL;
}

int get_proof_count(MerkleProof *proof) {
    if (proof) {
        return proof->count;
    }
    return 0;
}

const unsigned char* get_proof_sibling(MerkleProof *proof, int index) {
    if (proof && index >= 0 && index < proof->count) {
        return proof->siblings[index];
    }
    return NULL;
}