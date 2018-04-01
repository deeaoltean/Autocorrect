/*
 * Project: Homework 1 - Data Structures
 * Name: OLTEAN ANDREEA-DIANA
 * Group: 314CD
 */
#ifndef TABLE_H
#define TABLE_H
#define MAX 100
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// List element for Dictionary lists.
typedef struct ListNode {
	struct ListNode *next;
	struct ListNode *prev;
	char *key;
	char *value;
	int frequency;
} ListNode;

// Dictionary structure that includes the lists of elements and their number.
typedef struct Dictionary {
	ListNode **lists;		// lists of elements.
	int N;					// number of lists.
} Dictionary;


// Initializes an empty Dictionary structure.
Dictionary* createDictionary(int N);

// Adds an element to the Dictionary structure.
void addElement(Dictionary *dictionary, char *key, char *value, int frequency);

// Removes an element from the Dictionary structure.
void removeElement(Dictionary *dictionary, char *key, char *value);

// Prints all the elements from all the lists of the Dictionary structure.
void printDictionary(FILE *f, Dictionary *dictionary);

// Gets all the elements with the specified key and increments the frequencies.
ListNode* get(Dictionary *dictionary, char *key);

// Prints all the elements with the specified value.
void printValue(FILE *f, Dictionary *dictionary , char *value);

// Prints all the elements with the specified frequency.
void printFrequency(FILE *f, Dictionary *dictionary , int frequency);

// Returns a list containing the elements with the specified value.
ListNode* unionValues(Dictionary *dictionary, char *value);

// Returns a list containing the elements with maximum frequency in each list.
ListNode* unionMaxFrequencies(Dictionary *dictionary);

// Returns a new Dictionary with reversed lists of the input structure.
Dictionary* reverseLists(Dictionary *dictionary);

// Prints a double-linked non-circular list.
void printList(FILE *f, ListNode *list);

// Frees all the memory allocated for the Dictionary.
void freeDictionary(Dictionary *dictionary);

// Frees all the memory allocated for a double-linked non-circular list.
void freeList(ListNode *list);

//------------------------------------------------------------------------------

/*
 * Name function: createDictionary
 * Return: the memory address of the dictionary
 * Arguments: the number of lists of the dictionary
 * Purpose: allocate memory for the dictionary
 */
Dictionary* createDictionary(int N) {
	Dictionary *dictionary;
	dictionary = (Dictionary*)malloc(sizeof(Dictionary));

	if(dictionary == NULL) {
		printf("Memory was not allocated\n");
		return NULL;
	} else {
		int i;
		dictionary->N = N;
		dictionary->lists = (ListNode**)malloc(N * sizeof(ListNode*));
		if(dictionary->lists == NULL) {
			printf("Memory was not allocated\n");
			return NULL;
		}
		for(i = 0; i < N; i++) {
			dictionary->lists[i] = NULL;
		}
		return dictionary;
	}
}

/*
 * Name function: Delete
 * Return: void (it does not return a value)
 * Arguments: the dictionary and the index of a specific list
 * Purpose: delete the last element of the list, if the number of elements
 * exceeds the maximum value N
 */
void Delete(Dictionary *dictionary, int r) {
	ListNode *t;
	t = dictionary->lists[r];
	int s = 1;
	//determining the number of elements in a specific list
	while(t->next != dictionary->lists[r]) {
		s = s + 1;
		t = t->next;
	}
	//if the maximum value is excedeed
	// the last element of the list is removed
	if(s  == dictionary->N) {
		removeElement(dictionary, dictionary->lists[r]->prev->key,
				dictionary->lists[r]->prev->value); 
	}
}

/*
 * Name function: Remove
 * Return: void (it does not return a value)
 * Arguments: the dictionary
 * Purpose: erasing the last element of each list
 * if the total number of cells from the dictionary
 * exceedes the maximum value 2 * N
 */
void Remove(Dictionary *dictionary) {
	ListNode *t;
	int s = 0;
	int i;
	//determining the total number of elements in the dictionary
	for(i = 0; i < dictionary->N; i++) {
		t = dictionary->lists[i];
		if(t != NULL) {
			s = s + 1;
			while(t->next != dictionary->lists[i]) {
				s = s + 1;
				t = t->next;
			}
		} 
	}
	//removing the last element of each list in case
	// the total exceeds the maximum value 
	if(s > 2 * dictionary->N) {
		for(i = 0; i < dictionary->N; i++) {
			t = dictionary->lists[i];
			if(t != NULL) {
				removeElement(dictionary, t->prev->key,
						t->prev->value);
			}
		}
	}
}

/*
 * Name function: newElement
 * Return: void (it does not return a value)
 * Arguments: the element, the key, the value and the frequency
 * Purpose: store the key, value and frequency in the new cell
 */
void newElement(ListNode *element, char *key, char *value, int frequency) {

	element->next = NULL;
	element->prev = NULL;
	element->key = (char*)malloc(MAX * sizeof(char));
	if(element->key == NULL) {
		printf("Memory was not allocated\n");
		return;
	}
	strcpy(element->key, key);
	element->value = (char*)malloc(MAX * sizeof(char));
	if(element->value == NULL) {
		printf("Memory was not allocated\n");
		return;
	}
	strcpy(element->value, value);
	element->frequency = frequency;
}

/*
 * Name function: addBefore
 * Return: void (it does not return a value)
 * Arguments: the dictionary, an element, the element before who I will add
 * an element and the index of the list
 * Purpose: add an element before a specific cell of a list
 */
void addBefore(ListNode *element, ListNode *t, Dictionary *dictionary, int r) {
	element->next = t;
	element->prev = t->prev;
	t->prev->next = element;
	t->prev = element;

	//changing the first element of the list in case the element is added
	// before the first one
	if(t == dictionary->lists[r]) {
		dictionary->lists[r] = element;
	} 
	Remove(dictionary);
}

/*
 * Name function: addAfter
 * Return: void (it does not return a value)
 * Arguments: the dictionary, an element, the element after who I will add
 * an element and the index of the list
 * Purpose: add an element after a certain cell of a list
 */
void addAfter(ListNode *element, ListNode *t, Dictionary *dictionary, int r) {
	element->prev = t;
	element->next = dictionary->lists[r];
	dictionary->lists[r]->prev = element;
	t->next = element;
	Remove(dictionary);
}

/*
 * Name function: sameElement
 * Return: void (it does not return a value)
 * Arguments: the dictionary, an element, the current cell,
 * key, value, frequency
 * Purpose: increases frequency if the element has the same key and value
 * as the current cell, the cell is removed and added in the correct order
 */
void sameElement(ListNode *t, ListNode *element, Dictionary *dictionary,
		char *key, char *value, int frequency) {
	t->frequency = t->frequency + frequency;
	int new_frequency = t->frequency;
	removeElement(dictionary, key, value);
	addElement(dictionary, key, value, new_frequency);  
	free(element->key);
	free(element->value);
	free(element); 
}

/*
 * Name function: checkLast
 * Return: void (it does not return a value)
 * Arguments: the dictionary, an element and key, value, frequency,
 * the cell element  and the index of the list
 * Purpose: checks where the element should be added in comparison with
 * the last cell of the list, if it was not added before into the list
 */
void checkLast(ListNode *t,ListNode *element, Dictionary *dictionary,
		char *key, char *value, int frequency, int r) {
	if(t->frequency < frequency) {
		addBefore(element, t, dictionary, r);
		return;
	}
	if(t->frequency > frequency) {
		addAfter(element, t, dictionary, r);
		return;
	}
	if(t->frequency == frequency){
		if(strcmp(t->value, value) == 0) {
			if(strcmp(t->key, key) > 0) {
				addBefore(element, t, dictionary, r);
				return;
			} else {
				if(strcmp(t->key, key) < 0) {
					addAfter(element, t, dictionary, r);
					return;
				}
			}
		}
		if(strcmp(t->value, value) > 0) {
			addBefore(element, t, dictionary, r);
			return;
		} else {
			addAfter(element, t, dictionary, r);
			return;
		}
	}

	ListNode *aux = t->next;
	if(strcmp(t->key, key) == 0 && strcmp(t->value, value) == 0) {
		sameElement(t, element, dictionary, key, value, frequency);
		return;                 
	}
}

/*
 * Name function: ListIndex
 * Return: the index of the list
 * Arguments: the dictionary and the key of the element
 * Purpose: determines the list in which an element should be added
 */
int listIndex(Dictionary *dictionary, char *key) {
	int r = 0, i;
	for(i = 0; i < strlen(key); i++) {
		r = r + key[i];
	}
	r = r % dictionary->N;
	return r;
}

/*
 * Name function: addElement
 * Return: void (it does not return a value)
 * Arguments: the dictionary, the key and the value of the element to be added
 * Purpose: add a new element into the dictionary or increase
 * the frequency value if the key and value of the element to be added already
 * exists
 */
void addElement(Dictionary *dictionary, char *key, char *value, int frequency) {
	ListNode *element;
	element = (ListNode*)malloc(sizeof(ListNode));
	if(element == NULL) {
		printf("Memory was not allocated\n");
		return;
	} else {
		newElement(element, key, value, frequency);
		//determining the index of the list
		// in which the element will be added
		int r;
		r = listIndex(dictionary, key);
		//adding the element if the list is empty
		if(dictionary->lists[r] == NULL) {
			dictionary->lists[r] = element;
			dictionary->lists[r]->next = element;
			dictionary->lists[r]->prev = element;
		} else {
			ListNode *t;
			//searching for the place in the specific list,
			// where the element will be added 
			//if the first element has the same key and value,
			// the frequency increases
			t = dictionary->lists[r]; 
			if(strcmp(t->value, value) == 0 &&
					strcmp(t->key, key) == 0) {
				t->frequency = t->frequency + frequency;
				free(element->key);
				free(element->value);
				free(element);
				return;
			}                     

			//deleting elements from the dictionary if needed
			Delete(dictionary, r);

			//adding the element if the last one was removed
			if(dictionary->lists[r] == NULL) {
				dictionary->lists[r] = element;
				dictionary->lists[r]->next = element;
				dictionary->lists[r]->prev = element;
				Remove(dictionary);
				return;
			}

			//searching the list for elements that have 
			//the same key and value
			t = dictionary->lists[r];
			while(t->next != dictionary->lists[r]) {
				ListNode *aux = t->next;
				if(strcmp(t->key, key) == 0 &&
						strcmp(t->value, value) == 0) {
					sameElement(t, element, dictionary, key, value, frequency);
					return;                 
				}
				t = aux;
			}

			//checking for the last element of the list
			if(strcmp(t->key, key) == 0 &&
					strcmp(t->value, value) == 0) {
				sameElement(t, element, dictionary, key, value, frequency);
				return;                 
			}

			//searching for the right place to insert the element
			t = dictionary->lists[r];
			while(t->next != dictionary->lists[r]) {
				if(t->frequency < frequency) {
					addBefore(element, t, dictionary, r);
					return;
				}
				if(t->frequency == frequency){
					if(strcmp(t->value, value) == 0) {
						if(strcmp(t->key, key) > 0) {
							addBefore(element, t,dictionary, r);
							return;
						}
					}
					if(strcmp(t->value, value) > 0) {
						addBefore(element, t, dictionary, r);
						return;
					} 
				}
				t = t->next;
			}
			//checking the conditions for the last element
			checkLast(t, element, dictionary, key, value, frequency, r);
		}
	}
}

/*
 * Name function: removeElement
 * Return: void (it does not return a value)
 * Arguments: the dictionary, the key and the value of the 
 * element to be removed
 * Purpose: remove a specific element from a list
 */
void removeElement(Dictionary *dictionary, char *key, char *value) {
	//determining the list in which is located the element to be removed
	int  r;
	r = listIndex(dictionary, key);
	//searching the list in case there are elements
	if(dictionary->lists[r] != NULL) {
		//if the first element needs to be removed
		if(strcmp(dictionary->lists[r]->value, value) == 0 &&
				strcmp(dictionary->lists[r]->key, key) == 0) {
			ListNode *t = dictionary->lists[r];
			//if there was only one element
			if(t->next == t) {
				dictionary->lists[r] = NULL;
			} else {
				dictionary->lists[r] = t->next;
				t->next->prev = t->prev;
				t->prev->next = t->next;
			}
			free(t->key);
			free(t->value);
			free(t);
		} else {
			//searching for the element that needs to be removed
			ListNode *t;
			t = dictionary->lists[r];
			while(t->next != dictionary->lists[r]) {
				ListNode *aux = t;
				t = t->next;
				//deleting the element if it has 
				//the same key and value
				if(strcmp(aux->value, value) == 0 &&
						strcmp(aux->key, key) == 0) {
					aux->prev->next = aux->next;
					aux->next->prev = aux->prev;
					free(aux->key);
					free(aux->value);
					free(aux);   
					break;    
				}
			}

			//checking for the last element
			if(t->next == dictionary->lists[r] &&
					strcmp(t->value, value) == 0
					&& strcmp(t->key, key) == 0) {
				t->prev->next = t->next;
				t->next->prev = t->prev;
				free(t->key);
				free(t->value);
				free(t);
			}	  
		}
	}
	return;
}

/*
 * Name function: printDictionary
 * Return: void (it does not return a value)
 * Arguments: the file I am writing into and the dictionary
 * Purpose: print the cells of every list of the dictionary
 */
void printDictionary(FILE *f, Dictionary *dictionary) {
	int i;
	ListNode *t;
	for(i = 0; i < dictionary->N; i++) {
		fprintf(f, "List %d: ", i);
		t = dictionary->lists[i];
		if(t != NULL) {
			while(t->next != dictionary->lists[i]) {
				fprintf(f, "(%s, %s, %d) ", t->key, t->value, t->frequency);
				t = t->next;
			}
			fprintf(f, "(%s, %s, %d) ", t->key, t->value, t->frequency);
		}
		fprintf(f, "\n");
	}		
}

/*
 * Name function: sort
 * Return: void (it does not return a value)
 * Arguments: the element to pe placed in the correct order, the dictionary, 
 * the key and the index of the list
 * Purpose: place the cell in the right order after the frequency was increased
 */
void sort(ListNode *t, Dictionary *dictionary, char *key, int r) {
	if(t != dictionary->lists[r] &&	t->prev->frequency <= t->frequency) {
		int new_frequency = t->frequency;
		char value[MAX];
		strcpy(value, t->value);
		//removing the element and adding it back in the corect order
		removeElement(dictionary,	key, value);
		addElement(dictionary, key,	value, new_frequency);  
	}
}

/*
 * Name function: get
 * Return: the memory address of a list
 * Arguments: the dictionary and the specific key
 * Purpose: create a list that contains the elements with a specific key
 */
ListNode* get(Dictionary *dictionary, char *key) {
	ListNode *t, *p;
	p = NULL;
	int r;
	r = listIndex(dictionary, key);
	t = dictionary->lists[r];
	if(t == NULL) {
		return NULL;
	} else {
		ListNode *new_list = NULL;
		while(t->next != dictionary->lists[r]) {
			if(strcmp(t->key, key) == 0) {
				//creating a new cell with the same key,
				// value and frequency
				ListNode *element;
				element = (ListNode*)malloc(sizeof(ListNode));
				if(element == NULL) {
					printf("Memory was not allocated\n");
					return NULL;
				}
				newElement(element, t->key, t->value, 
						t->frequency);
				t->frequency = t->frequency + 1;

				//adding the element if the list is empty
				if(new_list == NULL) {
					new_list = element;
					p = element;
					ListNode *aux = t->next;
					sort(t, dictionary, key, r);
					t = aux;
				} else {
					//adding the element into a list that
					// contains elements
					p->next = element;
					element->prev = p;
					p = p->next;
					ListNode *aux = t->next;
					sort(t, dictionary, key, r);
					t = aux;
				}
			} else {
				t = t->next;
			}
		}
		//adding the last element
		if(strcmp(t->key, key) == 0) {
			ListNode *element;
			element = (ListNode*)malloc(sizeof(ListNode));
			if(element == NULL) {
				printf("Memory was not allocated\n");
				return NULL;
			}
			newElement(element, t->key, t->value, t->frequency);
			t->frequency = t->frequency + 1;

			if(new_list == NULL) {
				new_list = element;
				element->prev = new_list;
				sort(t, dictionary, key, r);
			} else {
				p->next = element;
				element->prev = p;
				sort(t, dictionary, key, r);
			}
		}
		return new_list;
	}    
	return NULL;
}

/*
 * Name function: printValue
 * Return: void (it does not return a value)
 * Arguments: the file I am writing in, the dictionary and a specific value
 * Purpose: print the elements with a specific value
 */
void printValue(FILE *f, Dictionary *dictionary , char *value) {
	int i;
	ListNode *t;
	for(i = 0; i < dictionary->N; i++) {			
		t = dictionary->lists[i];
		//searching for elements if the dictionary is not empty
		if(t != NULL) {
			while(t->next != dictionary->lists[i]) {
				if(strcmp(t->value, value) == 0) {
					fprintf(f, "(%s, %s, %d) ", t->key,
							t->value, t->frequency);
				}
				t = t->next;
			}
			if(strcmp(t->value, value) == 0) {
				fprintf(f, "(%s, %s, %d) ", t->key,
						t->value, t->frequency);
				t = t->next;
			}
		}
	}
	fprintf(f, "\n");
}

/*
 * Name function: printFrequency
 * Return: void (it does not return a value)
 * Arguments: the file I am writing in, the dictionary and a specific frequency
 * Purpose: print the elements with a specific frequency
 */
void printFrequency(FILE *f, Dictionary *dictionary , int frequency) {
	int i;
	ListNode *t;
	for(i = 0; i < dictionary->N; i++) {			
		t = dictionary->lists[i];
		//searching for elements if the dictionary is not empty
		if(t != NULL) {
			while(t->next != dictionary->lists[i]) {
				if(t->frequency == frequency) {
					fprintf(f, "(%s, %s, %d) ", t->key, t->value, t->frequency);
				}
				t = t->next;
			}
			if(t->frequency == frequency) {
				fprintf(f, "(%s, %s, %d) ", t->key, t->value, t->frequency);
				t = t->next;
			}
		}
	}
	fprintf(f, "\n");
}

/*
 * Name function: unionValues
 * Return: the memory address of a list
 * Arguments: the dictionary and the specific value
 * Purpose: create a list that contains the elements with a specific vlaue
 */
ListNode* unionValues(Dictionary *dictionary, char *value) {
	ListNode *new_list = NULL;
	ListNode *t, *p = NULL;
	int i;
	//searching the dictionary
	for(i = 0; i < dictionary->N; i++) {
		t = dictionary->lists[i];
		//searching the list if it is not empty
		if(t != NULL) {
			while(t->next != dictionary->lists[i]) {
				if(strcmp(t->value, value) == 0) {
					ListNode *element;
					element = (ListNode*)malloc(sizeof(ListNode));
					if(element == NULL) {
						printf("Memory was not allocated\n");
						return NULL;
					}

					newElement(element, t->key, t->value, t->frequency);

					if(p == NULL) {
						new_list = element;
						new_list->next = NULL;
						new_list->prev = NULL;
						p = new_list;
					} else {
						p->next = element;
						element->prev = p;
						element->next = NULL;
						p = p->next;              
					}
				} 
				t = t->next;   
			}

			//checking if the last element needs to be added
			if(strcmp(t->value, value) == 0) {
				ListNode *element;
				element = (ListNode*)malloc(sizeof(ListNode));
				if(element == NULL) {
					printf("Memory was not allocated\n");
					return NULL;
				}

				newElement(element, t->key,
						t->value, t->frequency);
				if(p == NULL) {
					new_list = element;
					new_list->next = NULL;
					new_list->prev = NULL;
					p = new_list;
				} else {
					p->next = element;
					element->prev = p;
					element->next = NULL;
					p = p->next;              
				}
			} 

		}
	}
	if(p == NULL) {
		return NULL;
	}
	return new_list;
}

/*
 * Name function: unionMaxFrequencies
 * Return: the memory address of a list
 * Arguments: the dictionary 
 * Purpose: create a list that contains the elements 
 * with the maximum frequency of each list
 */
ListNode* unionMaxFrequencies(Dictionary *dictionary) {
	ListNode *new_list = NULL;
	ListNode *p = NULL;
	int i;
	for(i = 0; i < dictionary->N; i++) {
		if(dictionary->lists[i] != NULL) {
			int max_value;
			ListNode *t;
			t = dictionary->lists[i];
			//the maximum frequency is found in the first element
			max_value = t->frequency;
			while(t->next != dictionary->lists[i]) {
				if(t->frequency == max_value) {
					ListNode *element;
					element = (ListNode*)malloc(sizeof(ListNode));
					if(element == NULL) {
						printf("Memory was not allocated\n");
						return NULL;
					}

					newElement(element, t->key, t->value, t->frequency);

					if(p == NULL) {
						new_list = element;
						new_list->next = NULL;
						new_list->prev = NULL;
						p = new_list; 
					} else {
						p->next = element;
						element->prev = p;
						element->next = NULL;
						p = p->next;
					}
				}
				t = t->next;
			}

			//checking for the last element
			if(t->frequency == max_value) {
				ListNode *element;
				element = (ListNode*)malloc(sizeof(ListNode));
				if(element == NULL) {
					printf("Memory was not allocated\n");
					return NULL;
				}

				newElement(element, t->key, t->value, t->frequency);

				if(p == NULL) {
					new_list = element;
					new_list->next = NULL;
					new_list->prev = NULL;
					p = new_list; 
				} else {
					p->next = element;
					element->prev = p;
					element->next = NULL;
					p = p->next;
				}
			}
		}
	}
	return new_list;
}

/*
 * Name function: reverseLists
 * Return: the memory address of a list
 * Arguments: the dictionary 
 * Purpose: create a list that has the cells reversed
 */
Dictionary* reverseLists(Dictionary *dictionary) {
	Dictionary *reverse_dictionary;
	reverse_dictionary = createDictionary(dictionary->N);
	if(reverse_dictionary == NULL) {
		printf("Memory was not allocated\n");
		return NULL;
	} else {
		int i;
		for(i = 0; i < dictionary->N; i++) {
			if(dictionary->lists[i] == NULL) {
				reverse_dictionary->lists[i] = NULL;
			} else {
				//the case if there is only one element in the list
				if(dictionary->lists[i] == dictionary->lists[i]->prev) {
					ListNode *element;
					element = (ListNode*)malloc(sizeof(ListNode));
					if(element == NULL) {
						printf("Memory was not allocated\n");
						return NULL;
					}
					newElement(element, dictionary->lists[i]->key, 
							dictionary->lists[i]->value, dictionary->lists[i]->frequency);
					element->next = element->prev = element;

					reverse_dictionary->lists[i] = element;
				} else {
					//the case where there are more than one element in a list
					ListNode *t, *p = NULL;
					t = dictionary->lists[i]->prev;

					while(t->prev != dictionary->lists[i]->prev) {
						ListNode *element;
						element = (ListNode*)malloc(sizeof(ListNode));
						if(element == NULL) {
							printf("Memory was not allocated\n");
							return NULL;
						}
						newElement(element, t->key, t->value, t->frequency);
						element->next = element->prev = element;

						if(p == NULL) {
							reverse_dictionary->lists[i] = element;
							p = element;
						} else {

							p->next = element;
							element->prev = p;
							element->next = reverse_dictionary->lists[i];
							p->next->next->prev = element;
							p = p->next;
						}
						t = t->prev;
					}

					ListNode *element;
					element = (ListNode*)malloc(sizeof(ListNode));
					if(element == NULL) {
						printf("Memory was not allocated\n");
						return NULL;
					}
					newElement(element, t->key, t->value, t->frequency);
					element->next = element->prev = element;
					p->next = element;
					element->prev = p;
					element->next = reverse_dictionary->lists[i];
					p->next->next->prev = element;
					p = p->next;
				}
			}
		}
	}
	return reverse_dictionary;
}

/*
 * Name function: printList
 * Return: void (it does not return a value)
 * Arguments: the file I am writing in and a list
 * Purpose: print the elements of a list
 */
void printList(FILE *f, ListNode *list) {
	ListNode *t;
	t = list;
	if(t == NULL) {
		fprintf(f, "\n");
	} else {
		while(t != NULL) {
			fprintf(f, "(%s, %s, %d) ", t->key, t->value, t->frequency);
			t = t->next;
		}
		fprintf(f,"\n");
	}
	return;
}


/*
 * Name function: freeList
 * Return: void (it does not return a value)
 * Arguments: a list
 * Purpose: free the memory of a list
 */
void freeList(ListNode *list) {
	ListNode *t, *p;
	t = list;
	if(t == NULL) {
		return;
	} else {
		if(t->next == NULL) {
			free(t->key);
			free(t->value);
			free(t);
		} else {
			while(t != NULL) {
				p = t;
				t = t->next;
				free(p->key);
				free(p->value);
				free(p);
			}
		}
	}
}

/*
 * Name function: freeDictionary
 * Return: void (it does not return a value)
 * Arguments: the dictionary
 * Purpose: free the memory of a dictionary
 */
void freeDictionary(Dictionary *dictionary) {
	int i;
	for(i = 0; i < dictionary->N; i++) {
		ListNode *t, *p;
		t = dictionary->lists[i];
		if(t != NULL) {
			while(t->next != dictionary->lists[i]) {
				p = t;
				t = t->next;
				free(p->key);
				free(p->value);
				free(p);
			}
			free(t->key);
			free(t->value);
			free(t);		      
		}
	}	
	free(dictionary->lists);	
	free(dictionary);
}
#endif
