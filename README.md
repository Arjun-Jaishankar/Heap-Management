# Heap Memory Management Implementation

A custom heap memory management system implemented in C using First-Fit allocation strategy.

## Features
- Simulated heap memory management using a 1MB memory pool
- First-Fit allocation algorithm
- Memory block metadata tracking
- Free block merging (coalescing)
- Interactive command-line interface
- Heap status visualization

## Implementation Details
- Uses a 1MB static array as simulated heap memory
- Implements block metadata structure to track allocations
- Supports allocation, deallocation, and heap status display
- Includes safety checks for invalid operations
