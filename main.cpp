#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Each node in the Trie
class TrieNode {
public:
    // Each node has up to 26 children (for each letter)
    TrieNode* children[26];

    // Marks if this node completes a word
    bool isEndOfWord;

    // Constructor
    TrieNode() {
        isEndOfWord = false;
        for (int i = 0; i < 26; i++) {
            children[i] = nullptr;
        }
    }
    
};

// The Trie class
class Trie {
private:
    TrieNode* root;

    // Helper function to find all words from a node
    // Input: current node, current word formed so far, results vector to store words
    // Output: none (modifies results vector by reference)
    // Purpose: Recursively find all complete words starting from the given node
    
    void clear(TrieNode* node){
        if (!node) return;
        for (int i = 0; i < 26; ++i) {
            clear(node->children[i]);
        }
        delete node;
    }
    void findAllWords(TrieNode* node, string currentWord, vector<string>& results) {   // mina Gamil
        if (!node) return;

        if (node->isEndOfWord) {
            results.push_back(currentWord);
        }

        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                char nextChar = 'a'+i;
                findAllWords(node->children[i], currentWord + nextChar, results); // rahma:currentWord + nextChar to complete the actual word 
            }
        }
    }
    //helper function to count words
    int help_count_word(TrieNode* node) {
        int count = 0;
        if (!node) { return 0; }
        if (node->isEndOfWord) { count++; }
        for (int i = 0; i < 26; i++) {
            count += help_count_word(node->children[i]);
        }
        return count;
    }
    bool helper_remove_word(TrieNode* node, const string& word, int depth) {
        if (!node) { return false; }
        if (depth == word.size()) {
            if (!node->isEndOfWord) { return false; }
            node->isEndOfWord = false;
            for (int i = 0; i < 26; ++i) {
                if (node->children[i]) return false;
            }
                return true;
            }
            int index = word[depth] - 'a';
            if (!node->children[index]) return false; // Word not found

            bool delete_Child = helper_remove_word(node->children[index], word, depth + 1);

            if (delete_Child) {
                delete node->children[index];
                node->children[index] = nullptr;
                if (!node->isEndOfWord) {
                    for (int i = 0; i < 26; ++i) {
                        if (node->children[i]) { return false; }
                    }
                    return true;
                }
            }
            return false;
        }
    string  longest_word(TrieNode* node, string current, string& longest) {
        if (!node) {
            return longest;
        }
        if (node->isEndOfWord) {
            if (current.length() > longest.length()) {
                longest = current;
            }
        }
        for (int i = 0; i < 26; i++) {
            if (node->children[i]) {
                char c = 'a' + i;
                longest_word(node->children[i], current + c, longest);
            }
        }
        return longest;
    }
public:
    // Constructor
    // Input: none
    // Output: none
    // Purpose: Initialize the Trie with a root node
    Trie() {
        root = new TrieNode();
    }

    // adding destructor to avoid memory leak 

    ~Trie() {
        clear(root);
    }

    // Insert a word into the Trie
    // Input: word to insert (string)
    // Output: none
    // Purpose: Add a word to the Trie by creating nodes for each character
    void insert(string word) {     //rahma
        // TODO: Implement this function
        TrieNode* node = root;
        for (int i = 0; i < word.length(); i++) {
            char c = word[i];

            //mina : handle uppercase
            if (c >= 'A' && c <= 'Z'){
                c = c - 'A' + 'a';
            }
            int index =c-'a'; // to get the index of the character ex:a-'a'=0

            // mina: skip non-alphabit characters
            if (index < 0 || index >= 26) continue;

            if (!node->children[index]) {
                node->children[index] = new TrieNode(); // new node to represent the  new char & new Sequence
            }
            node = node->children[index];
        }
        node->isEndOfWord = true; //marke the end 
    }

    // Search for a word in the Trie
    // Input: word to search for (string)
    // Output: boolean indicating if the word exists
    // Purpose: Check if the complete word exists in the Trie
    bool search(string word) {   //George
        TrieNode* node = root;
        for (char& c : word) {
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';  // normalize to lowercase
            }
            int index = c - 'a';

            if (index < 0 || index >= 26) return false;

            if (!node->children[index]) return false;

            node = node->children[index];
        }
        return node && node->isEndOfWord;
    }


    // Check if any word starts with the given prefix
    // Input: prefix to check (string)
    // Output: boolean indicating if any word has this prefix
    // Purpose: Verify if the prefix exists in the Trie (doesn't need to be a complete word)
    bool startsWith(string prefix) {  //Ahmed
        TrieNode* node = root;
        for(char& c : prefix){
            if(c >= 'A' && c <= 'Z'){
                c = c - 'A' + 'a';
            }
            int index = c - 'a';
            if( index < 0 || index >= 26) return false ;

            if (!node->children[index]) return false;
            
            node = node->children[index];
        }
        return true;
    }

    // Get all words that start with the given prefix
    // Input: prefix to complete (string)
    // Output: vector of strings that start with the prefix
    // Purpose: Find all complete words that begin with the given prefix
    vector<string> autocomplete(string prefix) {  //Abdo
        vector<string> suggestions;
        TrieNode* node = root;

        string lowerPrefix = prefix;
        for(char& c : lowerPrefix){
            if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 'a';
            }
            int index = c - 'a';
            if (index < 0 || index >= 26) return suggestions;

            if (!node->children[index]) return suggestions; // prefix not found
            node = node->children[index];
        }

        // find all words from this node
        findAllWords(node, lowerPrefix, suggestions);
        return suggestions;
    }
    //bouns fuction
   
    int count_word() {
            return help_count_word(root);// call it to print the final value in the main function
    }

    bool remove_word(const string& word) {
        return helper_remove_word(root, word, 0);
    }
    void checkSpelling(string word) {
        if (search(word)) {
            cout << word << endl;
        }
        else {
            cout << word << " is NOT correct " << endl;
            cout << "Did you mean: "  << endl;
            
            vector<string> suggestions = autocomplete(word.substr(0, 2));
            if (suggestions.empty()) {
                cout << " - No suggestions" << endl;
            }
            else for (string s : suggestions){
                cout << " - " << s << endl;
            }
        }
    }
    
    string find_longest_word() {
        string longest = "";
        return longest_word(root, "", longest);
    }


};

// Main function
// Input: none
// Output: integer return code
// Purpose: Program entry point, run tests and interactive demo
int main() {
    cout << "=== TRIE DATA STRUCTURE IMPLEMENTATION ===" << endl;
    cout << "Testing all Trie functionalities..." << endl;

    Trie trie;

    // Test 1: Basic insertion and search
    cout << "\n1. Testing basic insertion and search:" << endl;
    cout << "======================================" << endl;

    vector<string> words = {"apple", "banana", "orange", "grape", "kiwi"};
    for (const string& word : words) {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }

    // Test search for existing words
    for (const string& word : words) {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    // Test search for non-existing words
    vector<string> nonExisting = {"app", "ban", "ora", "graph", "kiwis"};
    for (const string& word : nonExisting) {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;
    }

    // Test 2: Prefix checking
    cout << "\n2. Testing prefix checking:" << endl;
    cout << "==========================" << endl;

    vector<string> prefixes = {"app", "ban", "ora", "grap", "k"};
    for (const string& prefix : prefixes) {
        bool hasPrefix = trie.startsWith(prefix);
        cout << "Prefix '" << prefix << "': " << (hasPrefix ? "EXISTS" : "DOESN'T EXIST") << endl;
    }

    vector<string> nonPrefixes = {"x", "yield", "zed", "micro", "nano"};
    for (const string& prefix : nonPrefixes) {
        bool hasPrefix = trie.startsWith(prefix);
        cout << "Prefix '" << prefix << "': " << (hasPrefix ? "EXISTS" : "DOESN'T EXIST") << " (expected: DOESN'T EXIST)" << endl;
    }

    // Test 3: Autocomplete functionality
    cout << "\n3. Testing autocomplete functionality:" << endl;
    cout << "======================================" << endl;

    vector<string> testPrefixes = {"a", "b", "o", "g", "k", "ap", "ban", "ora", "gr", "ki"};
    for (const string& prefix : testPrefixes) {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty()) {
            cout << "No suggestions found";
        } else {
            for (size_t i = 0; i < suggestions.size(); i++) {
                if (i > 0) cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // Test 4: Edge cases
    cout << "\n4. Testing edge cases:" << endl;
    cout << "======================" << endl;

    // Empty string tests
    bool emptySearch = trie.search("");
    cout << "Search empty string: " << (emptySearch ? "FOUND" : "NOT FOUND") << " (expected: NOT FOUND)" << endl;

    bool emptyPrefix = trie.startsWith("");
    cout << "Starts with empty prefix: " << (emptyPrefix ? "EXISTS" : "DOESN'T EXIST") << " (expected: EXISTS)" << endl;

    vector<string> emptySuggestions = trie.autocomplete("");
    cout << "Autocomplete for empty string: ";
    if (emptySuggestions.empty()) {
        cout << "No suggestions found";
    } else {
        for (size_t i = 0; i < emptySuggestions.size(); i++) {
            if (i > 0) cout << ", ";
            cout << emptySuggestions[i];
        }
    }
    cout << " (expected: all words)" << endl;

    // Test 5: Adding more words and retesting
    cout << "\n5. Testing with additional words:" << endl;
    cout << "================================" << endl;

    vector<string> additionalWords = {"application", "appetizer", "banister", "banana", "oracle", "grapefruit"};
    for (const string& word : additionalWords) {
        trie.insert(word);
        cout << "Inserted: " << word << endl;
    }

    // Test search for new words
    for (const string& word : additionalWords) {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    // Test autocomplete with new words
    vector<string> newPrefixes = {"app", "ban", "ora", "gra"};
    for (const string& prefix : newPrefixes) {
        vector<string> suggestions = trie.autocomplete(prefix);
        cout << "Autocomplete for '" << prefix << "': ";
        if (suggestions.empty()) {
            cout << "No suggestions found";
        } else {
            for (size_t i = 0; i < suggestions.size(); i++) {
                if (i > 0) cout << ", ";
                cout << suggestions[i];
            }
        }
        cout << endl;
    }

    // Test 6: Case sensitivity (all lowercase expected)
    cout << "\n6. Testing case sensitivity:" << endl;
    cout << "============================" << endl;

    trie.insert("Hello");
    trie.insert("WORLD");

    vector<string> caseWords = {"hello", "Hello", "WORLD", "world"};
    for (const string& word : caseWords) {
        bool found = trie.search(word);
        cout << "Search '" << word << "': " << (found ? "FOUND" : "NOT FOUND") << endl;
    }

    cout << "\n=== ALL TESTS COMPLETED ===" << endl;

    // Test 7: Bonus functions 

    cout << "\n7. Testing bonus functions:" << endl;
    cout << "============================" << endl;

    cout << "Word count:expected 13 | actual : " << trie.count_word() << endl;

    cout << "Removing 'banana'" << endl;
    trie.remove_word("banana");
    cout << "Search 'banana': (Expected: NOT FOUND) | Actual: " << (trie.search("banana") ? "FOUND" : "NOT FOUND") << endl;

    cout << "Spell check for 'appl'(Expected: suggestions like 'apple', 'apply'):" << endl;
    trie.checkSpelling("appl");

    cout << "Longest word in Trie:( Expected: 'application') | Actual: " << trie.find_longest_word() << endl;


    return 0;
}