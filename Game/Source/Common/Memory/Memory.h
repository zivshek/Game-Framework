#ifndef __MyMemory_H__
#define __MyMemory_H__

#if _DEBUG

void MyMemory_ForgetAllPreviousAllocations();
void MyMemory_ValidateAllocations(bool breakOnAnyAllocation);

void* operator new(size_t size, char* file, unsigned long line);
void* operator new[](size_t size, char* file, unsigned long line);
void operator delete(void* m, char* file, unsigned long line);
void operator delete[](void* m, char* file, unsigned long line);

void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* m);
void operator delete[](void* m);

//#define new new(__FILE__, __LINE__)

#endif //_DEBUG

#endif //__MyMemory_H__
