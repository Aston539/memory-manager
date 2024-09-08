#pragma once

#include <wtypes.h>

#ifndef EXTERN
#define EXTERN extern
#endif

#ifndef MmZeroMemory
#define MmZeroMemory( Base, Size ) memset( Base, NULL, Size )
#endif

#define MM_MAX_BLOCKS 256
#define MM_BLOCK_SIZE 16

typedef struct _MM_BLOCK
{
    BOOL   IsAllocated;

    PBYTE  BlockBase;
    SIZE_T BlockSize;

    SIZE_T AllocationSize;

} MM_BLOCK, * PMM_BLOCK;

typedef struct _MM_ALLOCATION
{
    PBYTE    AllocationBase;
    SIZE_T   AllocationSize;

    SIZE_T   TotalAllocatedSize;

    ULONG    AllocatedBlocksCount;
    MM_BLOCK Blocks[ MM_MAX_BLOCKS ];

} MM_ALLOCATION, *PMM_ALLOCATION;

EXTERN MM_ALLOCATION MmAllocation;

//
// Initialize memory manager with a static
// allocation
//
VOID
MmInitialize(
    _In_ PBYTE  Allocation,
    _In_ SIZE_T Size
);

BOOL
MmAllocateBlock(
    _In_ SIZE_T Size,
    _Inout_ PMM_BLOCK* Block
);

BOOL
MmFindBlockByAllocation(
    _In_ PBYTE Allocation,
    _Inout_ PMM_BLOCK* Block
);

PBYTE
MmAllocate(
    _In_ SIZE_T Size
);

VOID
MmFree(
    _In_ PBYTE Allocation
);