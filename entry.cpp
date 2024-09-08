#include "mm.h"

#include <cstdio>

int main()
{
    PBYTE ManagerBlock = ( PBYTE )malloc( 4096 );

    memset( ManagerBlock, 'm', 4096 );

    printf( "Allocated MmBlock as: 0x%p\n", ManagerBlock );

    MmInitialize( ManagerBlock, 4096 );

    PBYTE TestAlloc  = MmAllocate( 25 );
    PBYTE TestAlloc2 = MmAllocate( 7 );

    printf( "TestAlloc: 0x%p\n", TestAlloc );
    printf( "TestAlloc2: 0x%p\n", TestAlloc2 );
    printf( "Difference: 0x%lX\n", TestAlloc2 - TestAlloc );

    memset( TestAlloc + 00, 1, 10 );
    memset( TestAlloc + 10, 2, 10 );
    memset( TestAlloc + 20, 3, 5  );

    MmFree( TestAlloc2 );
    MmFree( TestAlloc );

    system( "pause" );

    free( ManagerBlock );
}