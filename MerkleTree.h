#pragma once
#include "Hashing.h"
#include "Nodes.h"
class MerkleTree {
	MerkleNode* root;

	int my_memcmp(const unsigned char* a, const unsigned char* b, int size) {
		for (int i = 0; i < size; ++i) {
			if (a[i] != b[i]) {
				return a[i] - b[i];
			}
		}
		return 0;
	}
public:
	// compare two merkle trees
    bool compareMerkleTrees(MerkleNode* root1, MerkleNode* root2) {
        if (root1 == nullptr && root2 == nullptr) {
            return true;
        }

        if (root1 == nullptr || root2 == nullptr) {
            return false;
        }

        if (root1->hashType != root2->hashType) {
            return false;
        }

        if (root1->hashType == 1) { // Instructor hash
            if (root1->instructorHash != root2->instructorHash) {
                return false;
            }
        }
        else if (root1->hashType == 2) { // SHA hash
            if (my_memcmp(root1->shaHash, root2->shaHash, SHA256_DIGEST_LENGTH) != 0) {
                return false;
            }
        }

        for (int i = 0; i < 2; ++i) {
            if (!compareMerkleTrees(root1->getChild(i), root2->getChild(i))) {
                return false;
            }
        }

        return true;
    }

};