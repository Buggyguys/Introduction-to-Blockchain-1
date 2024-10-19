#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <stdbool.h>

typedef struct MerkleTree MerkleTree;
typedef struct MerkleProof MerkleProof;

struct MerkleProof {
    unsigned char **siblings;
    int *directions;
    int count;
};

MerkleTree* create_merkle_tree();
void add_element(MerkleTree *tree, const char *data);
bool find_element(MerkleTree *tree, const char *data);
void remove_element(MerkleTree *tree, const char *data);
MerkleProof *generate_proof(MerkleTree *tree, const char *data);
bool verify_proof(const unsigned char *root_hash, const char *data, MerkleProof *proof);
void print_tree(MerkleTree *tree);
void print_leaf_hashes(MerkleTree *tree);
void free_merkle_tree(MerkleTree *tree);
void free_merkle_proof(MerkleProof *proof);
int get_proof_count(MerkleProof *proof);
const unsigned char* get_proof_sibling(MerkleProof *proof, int index);
const unsigned char* get_root_hash(MerkleTree *tree);
char* hash_to_string(const unsigned char *hash);
//addition
char* clean_variable(const char* name);
#endif // MERKLE_TREE_H