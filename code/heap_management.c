#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define heap_size 1000000 //  heap size to 1 mb
#define max_allocations 100 // maximum number of allocations

char main_memory[heap_size];

typedef struct metadata
{
    size_t block_size; // size of the block
    char alloc_status; // 'f' for free || 'a' for allocated
    struct metadata *next_block;
} mem_block;

mem_block *heap_start=(void *)main_memory;

void initialize_heap()
{
    heap_start->block_size=heap_size;
    printf("size of memory block data: %lu bytes\n", (unsigned long)sizeof(mem_block));
    heap_start->alloc_status='f';
    heap_start->next_block=NULL;
    printf("memory pool created\n");
    printf("heap start address: %p\n", heap_start);
    printf("heap end address: %p\n", main_memory + heap_size - 1);
}

void *allocate_memory(size_t requested_size)
{
    mem_block *current_block;
    void *return_ptr=NULL;
    current_block=heap_start;

    if (requested_size <= 0 || requested_size > heap_size)
    {
        printf("invalid size request or not enough memory\n");
        return NULL;
    }

    while (current_block != NULL && (current_block->block_size < requested_size || current_block->alloc_status=='a'))
    {
        current_block=current_block->next_block;
    }

    if (current_block == NULL)
    {
        printf("out of memory: not enough space for %lu bytes\n", (unsigned long)requested_size);
        return NULL;
    }

    if (current_block->block_size==requested_size)
    {
        current_block->alloc_status='a';
        return_ptr=(void *)((char *)current_block + sizeof(mem_block));
        printf("allocated exact-fit block of %lu bytes\n", (unsigned long)requested_size);
    }
    else
    {
        mem_block *new_block=(void *)((char *)current_block + sizeof(mem_block) + requested_size);
        new_block->block_size=current_block->block_size - requested_size;
        new_block->next_block=current_block->next_block;
        new_block->alloc_status='f';

        current_block->block_size=requested_size;
        current_block->next_block=new_block;
        current_block->alloc_status='a';
        return_ptr=(void *)((char *)current_block + sizeof(mem_block));
    }
    return return_ptr;
}

void merge_free_blocks()
{
    mem_block *current_block=heap_start;
    while (current_block->next_block != NULL)
    {
        if (current_block->alloc_status=='f' && current_block->next_block->alloc_status=='f')
        {
            current_block->block_size+=current_block->next_block->block_size + sizeof(mem_block);
            current_block->next_block=current_block->next_block->next_block;
        }
        else
        {
            current_block=current_block->next_block;
        }
    }
}

void free_memory(void *ptr_to_free)
{
    if (ptr_to_free < (void *)main_memory || ptr_to_free >= (void *)(main_memory + heap_size))
    {
        printf("invalid pointer: address is outside the heap\n");
        return;
    }

    mem_block *current_block=heap_start;
    while (current_block != NULL && (void *)((char *)current_block + sizeof(mem_block)) != ptr_to_free)
    {
        current_block=current_block->next_block;
    }

    if (current_block == NULL)
    {
        printf("invalid pointer: memory block not found\n");
        return;
    }

    current_block->alloc_status='f';
    merge_free_blocks();
    printf("memory freed successfully\n");
}

void display_heap_status()
{
    mem_block *current_block=heap_start;
    size_t block_index=0;
    printf("heap status:\n");
    while (current_block != NULL)
    {
        printf("%lu - %lu \t", (unsigned long)block_index, (unsigned long)(block_index + current_block->block_size - 1));
        printf("%c\n", current_block->alloc_status);
        block_index+=current_block->block_size;
        current_block=current_block->next_block;
    }
}

int main()
{
    initialize_heap();
    int user_choice;
    void *allocated_blocks[max_allocations]; // array to track allocated blocks
    size_t block_sizes[max_allocations]; // array to track sizes of allocated blocks
    int num_allocated = 0; // number of currently allocated blocks
    size_t requested_size;

    while (1)
    {
        printf("\n1. allocate memory\n");
        printf("2. free memory\n");
        printf("3. display heap\n");
        printf("4. exit\n");
        printf("enter your choice: ");
        scanf("%d", &user_choice);

        if (user_choice == 1) {
            if (num_allocated >= max_allocations)
            {
                printf("maximum allocations reached. cannot allocate more memory.\n");
                continue;
            }
            printf("enter size to allocate (in bytes): ");
            scanf("%lu", (unsigned long *)&requested_size);
            allocated_blocks[num_allocated] = allocate_memory(requested_size);
            if (allocated_blocks[num_allocated] != NULL)
            {
                block_sizes[num_allocated] = requested_size;
                num_allocated++;
                printf("memory allocated at address: %p\n", allocated_blocks[num_allocated - 1]);
            }
        } else if (user_choice == 2) {
            if (num_allocated == 0)
            {
                printf("no memory allocated to free\n");
                continue;
            }
            printf("enter the size of the block to free: ");
            scanf("%lu", (unsigned long *)&requested_size);
            int block_found = 0;
            for (int i = 0; i < num_allocated; i++)
            {
                if (block_sizes[i] == requested_size)
                {
                    free_memory(allocated_blocks[i]);
                    for (int j = i; j < num_allocated - 1; j++)
                    {
                        allocated_blocks[j] = allocated_blocks[j + 1];
                        block_sizes[j] = block_sizes[j + 1];
                    }
                    num_allocated--;
                    block_found = 1;
                    break;
                }
            }
            if (!block_found)
            {
                printf("no block of size %lu bytes found to free\n", (unsigned long)requested_size);
            }
        } else if (user_choice == 3) {
            display_heap_status();
        } else if (user_choice == 4) {
            printf("exiting...\n");
            exit(0);
        } else {
            printf("invalid choice. please try again.\n");
        }
    }
    return 0;
}
