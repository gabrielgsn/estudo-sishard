# Documentation for `atv/09-tad` - Abstract Data Types (ADT)

This directory contains an implementation of a dynamic integer vector, which is a common example of an Abstract Data Type (ADT).

## Concepts

### Abstract Data Type (ADT)
An Abstract Data Type is a mathematical model for data types. It is defined by its behavior (semantics) from the point of view of a user, specifically in terms of possible values, possible operations on data of this type, and the behavior of these operations. ADTs hide the internal structure and implementation details from the user.

### Dynamic Array (Vector)
A dynamic array (often called a vector) is a list or sequence of elements that can grow or shrink in size at runtime. Unlike static arrays, you don't need to specify their size at compile time. They achieve this by allocating a certain amount of memory (capacity) and then reallocating more memory when the current capacity is exceeded.

## File Structure

*   `vec_int.h`: The header file defining the interface for the dynamic integer vector (`vec_int`).
*   `vec_int.c`: The C source file containing the implementation of the `vec_int` functions.
*   `test_vec_int.c`: A C source file containing tests to verify the functionality of `vec_int`.
*   `README.md`: The original readme for this assignment.

## `vec_int` Struct

The `vec_int` structure is defined in `vec_int.h` as follows:

```c
typedef struct {
    int *data;     // Pointer to the actual array of integers
    int size;      // Current number of elements in the vector
    int capacity;  // Current allocated memory capacity
} vec_int;
```

*   `data`: A pointer to an integer array stored on the heap. This is where the elements of the vector are kept.
*   `size`: An integer representing the number of elements currently present in the vector.
*   `capacity`: An integer representing the total number of elements the vector can hold with its currently allocated memory.

## Function Explanations and Examples

Below are explanations for each function implemented in `vec_int.c`, along with C code examples.

### `vec_int *vec_int_create()`

*   **Purpose**: Creates and initializes a new `vec_int` instance.
*   **Behavior**:
    *   Allocates memory for the `vec_int` struct itself.
    *   Allocates initial memory for the `data` array (typically with a small starting capacity, e.g., 2 as seen in the provided code).
    *   Sets `size` to 0.
    *   Sets `capacity` to the initial allocated capacity.
*   **Returns**: A pointer to the newly created `vec_int` on success, or `NULL` if memory allocation fails.

*   **Example**:
    ```c
    #include "vec_int.h"
    #include <stdio.h>

    int main() {
        vec_int *my_vector = vec_int_create();
        if (my_vector == NULL) {
            printf("Failed to create vector!\n");
            return 1;
        }
        printf("Vector created with size: %d, capacity: %d\n", my_vector->size, my_vector->capacity);
        // ... use the vector ...
        vec_int_destroy(&my_vector); // Don't forget to free memory
        return 0;
    }
    ```

### `void vec_int_destroy(vec_int **v)`

*   **Purpose**: Frees all memory associated with a `vec_int` instance.
*   **Behavior**:
    *   Frees the memory allocated for the `data` array.
    *   Frees the memory allocated for the `vec_int` struct itself.
    *   Sets the pointer to the vector (passed as `vec_int **v`) to `NULL` to prevent dangling pointers.
*   **Parameters**:
    *   `_v`: A pointer to a pointer to the `vec_int` to be destroyed.

*   **Example**:
    ```c
    #include "vec_int.h"

    int main() {
        vec_int *my_vector = vec_int_create();
        // ... use the vector ...
        vec_int_destroy(&my_vector);
        // my_vector is now NULL
        return 0;
    }
    ```

### `int vec_int_size(vec_int *v)`

*   **Purpose**: Returns the current number of elements in the vector.
*   **Parameters**:
    *   `v`: A pointer to the `vec_int`.
*   **Returns**: The number of elements (`size`) in the vector, or -1 if `v` is `NULL`.

*   **Example**:
    ```c
    #include "vec_int.h"
    #include <stdio.h>

    int main() {
        vec_int *my_vector = vec_int_create();
        vec_int_insert(my_vector, 0, 10);
        vec_int_insert(my_vector, 1, 20);
        printf("Current size of vector: %d\n", vec_int_size(my_vector)); // Output: 2
        vec_int_destroy(&my_vector);
        return 0;
    }
    ```

### `int vec_int_at(vec_int *v, int pos, int *val)`

*   **Purpose**: Retrieves the element at a specific position in the vector.
*   **Behavior**:
    *   Checks if the vector `v` is valid and if `pos` is within the valid range (`0 <= pos < v->size`).
    *   If valid, copies the element at `v->data[pos]` into the memory location pointed to by `val`.
*   **Parameters**:
    *   `v`: A pointer to the `vec_int`.
    *   `pos`: The index of the element to retrieve.
    *   `val`: A pointer to an integer where the retrieved value will be stored.
*   **Returns**: `1` if the operation is successful and `pos` is valid. `0` otherwise (e.g., `v` is `NULL`, `pos` is out of bounds).

*   **Example**:
    ```c
    #include "vec_int.h"
    #include <stdio.h>

    int main() {
        vec_int *my_vector = vec_int_create();
        vec_int_insert(my_vector, 0, 100);
        vec_int_insert(my_vector, 1, 200);

        int value;
        if (vec_int_at(my_vector, 1, &value)) {
            printf("Element at position 1: %d\n", value); // Output: 200
        } else {
            printf("Failed to get element at position 1.\n");
        }

        if (!vec_int_at(my_vector, 5, &value)) {
            printf("Position 5 is out of bounds, as expected.\n");
        }
        vec_int_destroy(&my_vector);
        return 0;
    }
    ```

### `int vec_int_insert(vec_int *v, int pos, int val)`

*   **Purpose**: Inserts an element at a specific position in the vector.
*   **Behavior**:
    *   Checks if `v` is valid and `pos` is a valid insertion position (`0 <= pos <= v->size`).
    *   **Capacity Handling**: If `v->size == v->capacity` (vector is full), it doubles the `capacity`. This involves:
        *   Allocating a new, larger memory block for `data` using `realloc`.
        *   If `realloc` fails, the insertion fails.
        *   Otherwise, updates `v->data` to point to the new memory and updates `v->capacity`.
    *   **Shifting Elements**: Shifts all elements from `pos` to `v->size - 1` one position to the right to make space for the new element.
    *   Inserts `val` at `v->data[pos]`.
    *   Increments `v->size`.
*   **Parameters**:
    *   `v`: A pointer to the `vec_int`.
    *   `pos`: The index at which to insert the new element.
    *   `val`: The integer value to insert.
*   **Returns**: `1` if the insertion is successful. `0` otherwise (e.g., `v` is `NULL`, `pos` is out of bounds, memory reallocation fails).

*   **Example**:
    ```c
    #include "vec_int.h"
    #include <stdio.h>

    void print_vector(vec_int *v) {
        printf("Vector (size %d, capacity %d): [", vec_int_size(v), v->capacity);
        for (int i = 0; i < vec_int_size(v); ++i) {
            int val;
            vec_int_at(v, i, &val);
            printf("%d%s", val, (i == vec_int_size(v) - 1) ? "" : ", ");
        }
        printf("]\n");
    }

    int main() {
        vec_int *my_vector = vec_int_create(); // Initial capacity likely 2
        print_vector(my_vector); // Vector (size 0, capacity 2): []

        vec_int_insert(my_vector, 0, 10);
        print_vector(my_vector); // Vector (size 1, capacity 2): [10]

        vec_int_insert(my_vector, 0, 5);  // Insert at beginning
        print_vector(my_vector); // Vector (size 2, capacity 2): [5, 10]

        vec_int_insert(my_vector, 1, 7);  // Insert in middle (triggers reallocation)
        print_vector(my_vector); // Vector (size 3, capacity 4): [5, 7, 10]

        vec_int_insert(my_vector, 3, 20); // Insert at end
        print_vector(my_vector); // Vector (size 4, capacity 4): [5, 7, 10, 20]

        vec_int_destroy(&my_vector);
        return 0;
    }
    ```

### `int vec_int_remove(vec_int *v, int pos)`

*   **Purpose**: Removes an element from a specific position in the vector.
*   **Behavior**:
    *   Checks if `v` is valid and `pos` is a valid position to remove from (`0 <= pos < v->size`).
    *   **Shifting Elements**: Shifts all elements from `pos + 1` to `v->size - 1` one position to the left to fill the gap.
    *   Decrements `v->size`.
    *   **Capacity Handling (Shrinking)**: If `v->size` becomes significantly smaller than `v->capacity` (e.g., `v->size == v->capacity / 4` in the provided code), it halves the `capacity`. This involves:
        *   Allocating a new, smaller memory block for `data` using `realloc`.
        *   If `realloc` is successful, updates `v->data` and `v->capacity`. (If it fails, the capacity might remain larger, which is generally acceptable).
*   **Parameters**:
    *   `v`: A pointer to the `vec_int`.
    *   `pos`: The index of the element to remove.
*   **Returns**: `1` if the removal is successful. `0` otherwise (e.g., `v` is `NULL`, `pos` is out of bounds).

*   **Example**:
    ```c
    #include "vec_int.h"
    #include <stdio.h>

    // (print_vector function from previous example)
    void print_vector(vec_int *v) {
        printf("Vector (size %d, capacity %d): [", vec_int_size(v), v->capacity);
        for (int i = 0; i < vec_int_size(v); ++i) {
            int val;
            vec_int_at(v, i, &val);
            printf("%d%s", val, (i == vec_int_size(v) - 1) ? "" : ", ");
        }
        printf("]\n");
    }

    int main() {
        vec_int *my_vector = vec_int_create();
        vec_int_insert(my_vector, 0, 10);
        vec_int_insert(my_vector, 1, 20);
        vec_int_insert(my_vector, 2, 30);
        vec_int_insert(my_vector, 3, 40); // Size 4, Capacity likely 4
        print_vector(my_vector); // Vector (size 4, capacity 4): [10, 20, 30, 40]

        vec_int_remove(my_vector, 1); // Remove 20
        print_vector(my_vector); // Vector (size 3, capacity 4): [10, 30, 40]

        vec_int_remove(my_vector, 0); // Remove 10
        print_vector(my_vector); // Vector (size 2, capacity 4): [30, 40]

        vec_int_remove(my_vector, 1); // Remove 40. Size becomes 1. Capacity might shrink.
                                      // If initial capacity was 2, then cap/4 = 0.5, size = 1.
                                      // If it grew to 4, then cap/4 = 1, size = 1. Shrink to capacity 2.
        print_vector(my_vector); // Vector (size 1, capacity 2): [30] (Example, actual capacity may vary based on exact shrink condition)

        vec_int_destroy(&my_vector);
        return 0;
    }
    ```

## Compilation and Testing

As mentioned in the `README.md` for this assignment:

1.  **Compile**:
    You can compile the vector implementation along with the test program using GCC:
    ```bash
    gcc -Og -Wall -g vec_int.c test_vec_int.c -o test
    ```
    *   `-Og`: Optimize for debugging.
    *   `-Wall`: Enable all warnings.
    *   `-g`: Include debugging information.

2.  **Run Tests**:
    Execute the compiled test program:
    ```bash
    ./test
    ```
    The `test_vec_int.c` file contains assertions that check the correct behavior of the vector functions. If all tests pass, it usually means the implementation is correct according to the test cases.

This documentation should provide a good understanding of the dynamic integer vector implemented in this directory.
