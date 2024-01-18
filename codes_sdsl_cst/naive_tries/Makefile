CPP=g++

BINS=intTernarySearchTree\
intTrie\
pathsTernarySearchTree\
pathsTrie\
pathsTrieMap\
suffixPathsTernarySearchTree\
suffixPathsTrieMap\
ternarySearchTree\
trie


all: clean bin

bin: $(BINS)

intTernarySearchTree:
	$(CPP) -o intTernarySearchTree intTernarySearchTree.cpp

intTrie:
	$(CPP) -o intTrie intTrie.cpp

pathsTernarySearchTree:
	$(CPP) -o pathsTernarySearchTree pathsTernarySearchTree.cpp

pathsTrie:
	$(CPP) -o pathsTrie pathsTrie.cpp

pathsTrieMap:
	$(CPP) -o pathsTrieMap pathsTrieMap.cpp

suffixPathsTernarySearchTree:
	$(CPP) -o suffixPathsTernarySearchTree suffixPathsTernarySearchTree.cpp

suffixPathsTrieMap:
	$(CPP) -o suffixPathsTrieMap suffixPathsTrieMap.cpp

ternarySearchTree:
	$(CPP) -o ternarySearchTree ternarySearchTree.cpp

trie:
	$(CPP) -o trie trie.cpp

clean:
	rm -f $(BINS) $(OBJECTS)