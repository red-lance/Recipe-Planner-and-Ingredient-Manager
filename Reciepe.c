#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Ingredient {
    char name[30];
    int quantity;
    struct Ingredient *next;
} Ingredient; // Struct for ingredients

typedef struct Recipe {
    char name[30];
    Ingredient *ingredients;
    struct Recipe *next;
} Recipe; // Struct for recipe

Recipe *recipeHead = NULL;

typedef struct StackNode {
    char action[50];
    struct StackNode *next;
} StackNode; // Struct for Stack as Linked List

StackNode *undoStack = NULL;
StackNode *redoStack = NULL;

typedef struct QueueNode {
    char ingredientName[30];
    struct QueueNode *next;
} QueueNode; // Struct for Queue as Linked List

QueueNode *front = NULL, *rear = NULL; 

typedef struct CategoryNode {
    char category[30];
    Recipe *recipes;
    struct CategoryNode *left, *right;
} CategoryNode; //Struct for BST

CategoryNode *categoryRoot = NULL; 

typedef struct GraphNode {
    char recipeName[30];
    struct GraphNode *next;
} GraphNode; // Struct for Graph as Linked List

typedef struct RecipeGraph {
    int numRecipes;
    GraphNode **adjLists;
} RecipeGraph;

RecipeGraph *recipeGraph = NULL;

void addIngredient(Ingredient **head, char *name, int quantity) {
    Ingredient *newIngredient = (Ingredient *)malloc(sizeof(Ingredient));
    strcpy(newIngredient->name, name);
    newIngredient->quantity = quantity;
    newIngredient->next = *head;
    *head = newIngredient;
} //Function for Adding Ingredient

void addRecipe(char *name) {
    Recipe *newRecipe = (Recipe *)malloc(sizeof(Recipe));
    strcpy(newRecipe->name, name);
    newRecipe->ingredients = NULL;
    newRecipe->next = recipeHead;
    recipeHead = newRecipe;
    printf("Recipe %s added successfully.\n", name);
} //Function for Adding Recipe

void displayRecipes() {
    Recipe *current = recipeHead;
    while (current != NULL) {
        printf("Recipe: %s\n", current->name);
        Ingredient *ingredient = current->ingredients;
        while (ingredient != NULL) {
            printf("  Ingredient: %s, Quantity: %d\n", ingredient->name, ingredient->quantity);
            ingredient = ingredient->next;
        }
        current = current->next;
    }
} //Function for Displaying Recipe

void push(StackNode **stack, char *action) {
    StackNode *newNode = (StackNode *)malloc(sizeof(StackNode));
    strcpy(newNode->action, action);
    newNode->next = *stack;
    *stack = newNode;
} //Function to push to Stack

void pop(StackNode **stack) {
    if (*stack == NULL) {
        printf("Stack is empty.\n");
        return;
    }
    StackNode *temp = *stack;
    *stack = (*stack)->next;
    free(temp);
} //Function to pop from Stack

void displayStack(StackNode *stack) {
    while (stack != NULL) {
        printf("%s\n", stack->action);
        stack = stack->next;
    }
} //Function to Display Stack

void enqueue(char *ingredientName) {
    QueueNode *newNode = (QueueNode *)malloc(sizeof(QueueNode));
    strcpy(newNode->ingredientName, ingredientName);
    newNode->next = NULL;
    if (rear == NULL) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
} //Function to add to queue

void dequeue() {
    if (front == NULL) {
        printf("Shopping list is empty.\n");
        return;
    }
    QueueNode *temp = front;
    front = front->next;
    if (front == NULL) {
        rear = NULL;
    }
    free(temp);
} //Function to remove from queue

void displayQueue() {
    QueueNode *current = front;
    while (current != NULL) {
        printf("%s\n", current->ingredientName);
        current = current->next;
    }
} //Function to display queue

CategoryNode* insertCategory(CategoryNode *node, char *category) {
    if (node == NULL) {
        CategoryNode *newNode = (CategoryNode *)malloc(sizeof(CategoryNode));
        strcpy(newNode->category, category);
        newNode->recipes = NULL;
        newNode->left = newNode->right = NULL;
        return newNode;
    }
    if (strcmp(category, node->category) < 0) {
        node->left = insertCategory(node->left, category);
    } else if (strcmp(category, node->category) > 0) {
        node->right = insertCategory(node->right, category);
    }
    return node;
} //Function to insert Category

void inorderCategory(CategoryNode *root) {
    if (root != NULL) {
        inorderCategory(root->left);
        printf("Category: %s\n", root->category);
        inorderCategory(root->right);
    }
} //Function to traverse Category in inorder traversal method

RecipeGraph* createGraph(int numRecipes) {
    RecipeGraph *graph = (RecipeGraph *)malloc(sizeof(RecipeGraph));
    graph->numRecipes = numRecipes;
    graph->adjLists = (GraphNode **)malloc(numRecipes * sizeof(GraphNode *));
    for (int i = 0; i < numRecipes; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
} //Function to create Graph

void addEdge(RecipeGraph *graph, int src, int dest, char *recipeName) {
    GraphNode *newNode = (GraphNode *)malloc(sizeof(GraphNode));
    strcpy(newNode->recipeName, recipeName);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;
} //Function to add Edge

void printGraph(RecipeGraph *graph) {
    for (int i = 0; i < graph->numRecipes; i++) {
        GraphNode *current = graph->adjLists[i];
        printf("\nRecipe %d: ", i);
        while (current) {
            printf("%s -> ", current->recipeName);
            current = current->next;
        }
        printf("NULL\n");
    }
} //Function to display Graph

int main() {
    int choice;
    char recipeName[30], ingredientName[30], category[30], action[50];
    int quantity, numRecipes, src, dest;

    while (1) {
        printf("\n--- Recipe Planner & Ingredient Manager ---\n");
        printf("1. Add Recipe\n");
        printf("2. Add Ingredient to Last Added Recipe\n");
        printf("3. Display Recipes\n");
        printf("4. Undo Last Action\n");
        printf("5. Add to Shopping List\n");
        printf("6. View and Remove First Item in Shopping List\n");
        printf("7. Add Recipe Category\n");
        printf("8. Display Recipe Categories\n");
        printf("9. Create Recipe Dependency Graph\n");
        printf("10. Add Dependency Between Recipes\n");
        printf("11. Display Recipe Dependency Graph\n");
        printf("12. Exit\n");
        printf("Enter your choice: ");

        // Validate input for choice
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
            case 1:  // Add Recipe
                printf("Enter recipe name: ");
                scanf(" %[^\n]", recipeName);
                addRecipe(recipeName);
                snprintf(action, sizeof(action), "Added recipe %s", recipeName);
                push(&undoStack, action);
                break;

            case 2:  // Add Ingredient
                if (recipeHead == NULL) {
                    printf("No recipe available. Please add a recipe first.\n");
                    break;
                }
                printf("Enter ingredient name: ");
                scanf(" %[^\n]", ingredientName);
                printf("Enter quantity: ");
                scanf("%d", &quantity);
                addIngredient(&recipeHead->ingredients, ingredientName, quantity);
                snprintf(action, sizeof(action), "Added ingredient %s to recipe %s", ingredientName, recipeHead->name);
                push(&undoStack, action);
                break;

            case 3:  // Display Recipes
                printf("\n--- Recipes ---\n");
                displayRecipes();
                break;

            case 4:  // Undo Last Action
                if (undoStack == NULL) {
                    printf("Nothing to undo.\n");
                } else {
                    printf("Undoing action: %s\n", undoStack->action);
                    pop(&undoStack);
                }
                break;

            case 5:  // Add to Shopping List
                printf("Enter ingredient name to add to shopping list: ");
                scanf(" %[^\n]", ingredientName);
                enqueue(ingredientName);
                break;

            case 6:  // View and Remove from Shopping List
                printf("\n--- Shopping List ---\n");
                displayQueue();
                dequeue();
                printf("\n--- Shopping List after removing first item ---\n");
                displayQueue();
                break;

            case 7:  // Add Recipe Category
                printf("Enter category name (e.g., Breakfast, Lunch): ");
                scanf(" %[^\n]", category);
                categoryRoot = insertCategory(categoryRoot, category);
                break;

            case 8:  // Display Recipe Categories
                printf("\n--- Recipe Categories (Inorder) ---\n");
                inorderCategory(categoryRoot);
                break;

            case 9:  // Create Recipe Dependency Graph
                printf("Enter the number of recipes: ");
                scanf("%d", &numRecipes);
                recipeGraph = createGraph(numRecipes);
                printf("Recipe dependency graph created with %d recipes.\n", numRecipes);
                break;

            case 10:  // Add Dependency Between Recipes
                printf("Enter source recipe index: ");
                scanf("%d", &src);
                printf("Enter destination recipe index: ");
                scanf("%d", &dest);
                printf("Enter recipe dependency description: ");
                scanf(" %[^\n]", recipeName);
                addEdge(recipeGraph, src, dest, recipeName);
                break;

            case 11:  // Display Recipe Dependency Graph
                printf("\n--- Recipe Dependency Graph ---\n");
                printGraph(recipeGraph);
                break;

            case 12:  // Exit
                printf("Exiting program.\n");
                return 0;

            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}