#include "GamePCH.h"

#include "CPPList.h"

#if _DEBUG

#define newtype_reg         1
#define newtype_array       2

// Assert when 'new' has been called this many times.
#define CountToAssertOn     UINT_MAX

// Undefine the 'new' defined by '#define new new(__FILE__, __LINE__)' in Memory.h.
// So 'new' simply means 'new' in this file.
#undef new

class MemObject : public CPPListNode
{
public:
    char* m_file;
    unsigned long m_line;
    unsigned int m_type;
    unsigned int m_size;
    unsigned int m_count;

    MemObject() :
        m_file(0),
        m_line(0),
        m_type(0),
        m_size(0),
        m_count(0)
    {
    }
};

CPPListHead g_Allocations;
unsigned int g_AllocationCount = 0;

void MyMemory_ForgetAllPreviousAllocations()
{
    while( g_Allocations.GetHead() )
    {
        g_Allocations.RemHead();
    }
}

void MyMemory_ValidateAllocations(bool breakOnAnyAllocation)
{
    CPPListNode* pNode;
    for( pNode = g_Allocations.HeadNode.Next; pNode->Next; pNode = pNode->Next )
    {
        MemObject* obj = (MemObject*)pNode;
        assert( obj->m_type < 3 );
        assert( obj->Next != NULL );
        assert( obj->Prev != NULL );

        OutputMessage( "%s(%d): Memory unreleased.\n", obj->m_file, obj->m_line );
        if( breakOnAnyAllocation )
            __debugbreak();
    }
}

//===========================================================================================
// Overrides for new/delete with file/line numbers
//===========================================================================================

void* operator new(size_t size, char* file, unsigned long line)
{
    assert( CountToAssertOn == UINT_MAX || g_AllocationCount != CountToAssertOn );
    assert( size > 0 );

    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = file;
    mo->m_line = line;
    mo->m_type = newtype_reg;
    mo->m_size = size;
    mo->m_count = g_AllocationCount++;

    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof(MemObject);
}

void* operator new[](size_t size, char* file, unsigned long line)
{
    assert( CountToAssertOn == UINT_MAX || g_AllocationCount != CountToAssertOn );
    assert( size > 0 );

    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = file;
    mo->m_line = line;
    mo->m_type = newtype_array;
    mo->m_size = size;
    mo->m_count = g_AllocationCount++;

    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void operator delete(void* m, char* file, unsigned long line)
{
    delete (char*)m;
}

void operator delete[](void* m, char* file, unsigned long line)
{
    delete (char*)m;
}

//===========================================================================================
// Overrides for global new/delete
//===========================================================================================

void* operator new(size_t size)
{
    assert( CountToAssertOn == UINT_MAX || g_AllocationCount != CountToAssertOn );
    assert( size > 0 );

    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = 0;
    mo->m_line = 0;
    mo->m_type = newtype_reg;
    mo->m_size = size;
    mo->m_count = g_AllocationCount++;

    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void* operator new[](size_t size)
{
    assert( CountToAssertOn == UINT_MAX || g_AllocationCount != CountToAssertOn );
    assert( size > 0 );

    MemObject* mo = (MemObject*)malloc( size + sizeof(MemObject) );
    mo->m_file = 0;
    mo->m_line = 0;
    mo->m_type = newtype_array;
    mo->m_size = size;
    mo->m_count = g_AllocationCount++;

    g_Allocations.AddTail( mo );

    return ((char*)mo) + sizeof( MemObject );
}

void operator delete(void* m)
{
    if( m == 0 )
        return;

    MemObject* mo = (MemObject*)(((char*)m) - sizeof(MemObject));
    assert( mo->m_type == newtype_reg );
    mo->Remove();

    free( mo );
}

void operator delete[](void* m)
{
    if( m == 0 )
        return;

    MemObject* mo = (MemObject*)( ((char*)m) - sizeof(MemObject) );
    assert( mo->m_type == newtype_array );
    mo->Remove();

    free( mo );
}

#endif //_DEBUG
