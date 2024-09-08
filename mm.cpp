#include "mm.h"

MM_ALLOCATION MmAllocation;

VOID
MmInitialize(
    _In_ PBYTE  Allocation,
    _In_ SIZE_T Size
)
{
    MmAllocation.AllocationBase = Allocation;
    MmAllocation.AllocationSize = Size;

    //
    // Initialize blocks with static size
    //
    for ( ULONG I = NULL; I < MM_MAX_BLOCKS; I++ )
    {
        PMM_BLOCK Block = &MmAllocation.Blocks[ I ];

        Block->BlockBase = Allocation + ( I * MM_BLOCK_SIZE );
        Block->BlockSize = MM_BLOCK_SIZE;
    }
}

BOOL
MmAllocateBlock(
    _In_ SIZE_T Size,
    _Inout_ PMM_BLOCK* Block
)
{
    if ( Block == NULL )
    {
        return FALSE;
    }

    //
    // Calculate how many blocks we are going to need
    // to fulfil the allocation
    //
    ULONG NeededBlocks = Size / MM_BLOCK_SIZE;

    for ( ULONG I = NULL; I < MM_MAX_BLOCKS; I++ )
    {
        if ( I + NeededBlocks > MM_MAX_BLOCKS )
        {
            //
            // Failed to find a good block
            //
            return FALSE;
        }

        PMM_BLOCK CurrentBlock = &MmAllocation.Blocks[ I ];

        if ( CurrentBlock->IsAllocated )
        {
            continue;
        }

        BOOL IsGoodBlock = TRUE;
        for ( ULONG J = NULL; J < NeededBlocks; J++ )
        {
            //
            // Pointer arithmetic = MmAllocation.Blocks[ I + J ]
            //
            PMM_BLOCK SubsequentBlock = CurrentBlock + J;

            //
            // If the next block in our allocation chain is used
            // then look for another block
            //
            if ( SubsequentBlock->IsAllocated )
            {
                //
                // Given we cant allocate within this range of blocks
                // advance our iterator by the start of the bad block
                //
                I += J;

                IsGoodBlock = FALSE;

                break;
            }
        }

        if ( IsGoodBlock )
        {
            MmAllocation.AllocatedBlocksCount += NeededBlocks;

            CurrentBlock->IsAllocated = TRUE;
            CurrentBlock->AllocationSize = Size;

            *Block = CurrentBlock;

            return TRUE;
        }
    }
}

BOOL
MmFindBlockByAllocation(
    _In_ PBYTE Allocation,
    _Inout_ PMM_BLOCK* Block
)
{
    if ( Allocation == NULL || Block == NULL )
    {
        return FALSE;
    }

    for ( ULONG I = NULL; I < MmAllocation.AllocatedBlocksCount; I++ )
    {
        PMM_BLOCK CurrentBlock = &MmAllocation.Blocks[ I ];

        if ( CurrentBlock->BlockBase == Allocation )
        {
            *Block = CurrentBlock;

            return TRUE;
        }
    }

    return FALSE;
}

PBYTE
MmAllocate(
    _In_ SIZE_T Size
)
{
    if ( Size == NULL || Size > MmAllocation.AllocationSize )
    {
        return NULL;
    }

    PMM_BLOCK Allocation = NULL;
    if ( MmAllocateBlock( Size, &Allocation ) == FALSE )
    {
        return NULL;
    }

    return Allocation->BlockBase;
}

VOID
MmFree(
    _In_ PBYTE Allocation
)
{
    PMM_BLOCK AllocatedBlock = NULL;
    if ( MmFindBlockByAllocation( Allocation, &AllocatedBlock ) == FALSE )
    {
        return;
    }

    //
    // Zero the block for later use.
    //
    MmZeroMemory( AllocatedBlock->BlockBase, AllocatedBlock->AllocationSize );

    AllocatedBlock->IsAllocated = FALSE;
    AllocatedBlock->AllocationSize = NULL;
}