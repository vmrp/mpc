#include "interpreter.h"

struct Table StringTable;
struct TableEntry *StringHashTable[STRING_TABLE_SIZE];
char *StrEmpty;

/* 初始化共享串系统 */
void TableInit(void)
{
    TableInitTable(&StringTable, &StringHashTable[0], STRING_TABLE_SIZE, TRUE);
    StrEmpty = TableStrRegister("");
}

/* 字符串哈希函数 */
static unsigned int TableHash(const char *Key, int Len)
{
    unsigned int Hash = Len;
    int Offset;
    int Count;
    
    for (Count = 0, Offset = 8; Count < Len; Count++, Offset+=7)
    {
        if (Offset > sizeof(unsigned int) * 8 - 7)
            Offset -= sizeof(unsigned int) * 8 - 6;
            
        Hash ^= *Key++ << Offset;
    }
    
    return Hash;
}

/* 初始化一个表 */
void TableInitTable(struct Table *Tbl, struct TableEntry **HashTable, int Size, int OnHeap)
{
    Tbl->Size = Size;
    Tbl->OnHeap = OnHeap;
    Tbl->HashTable = HashTable;
    memset((void *)HashTable, '\0', sizeof(struct TableEntry *) * Size);
}

/* 用key检查一个哈希表入口 */
static struct TableEntry *TableSearch(struct Table *Tbl, const char *Key, int *AddAt)
{
    struct TableEntry *Entry;
    int HashValue = ((unsigned long)Key) % Tbl->Size;   /* 共享字符串具有唯一的地址，所以我们不需要散列他们 */
    
    for (Entry = Tbl->HashTable[HashValue]; Entry != NULL; Entry = Entry->Next)
    {
        if (Entry->p.v.Key == Key)
            return Entry;   /* 发现 */
    }
    
    *AddAt = HashValue;    /* 没有在链表中找到它 */
    return NULL;
}

/* 设置一个符号值。如果它已经存在，则返回FALSE. 
 * key 必须是一个共享的字符串 TableStrRegister() */
int TableSet(struct Table *Tbl, char *Key, struct Value *Val, const char *DeclFileName, int DeclLine, int DeclColumn)
{
    int AddAt = 0;
    struct TableEntry *FoundEntry = TableSearch(Tbl, Key, &AddAt);
    
    if (FoundEntry == NULL)
    {   /* 将它添加到表 */
        struct TableEntry *NewEntry = VariableAlloc(NULL, sizeof(struct TableEntry), Tbl->OnHeap);
        NewEntry->DeclFileName = DeclFileName;
        NewEntry->DeclLine = DeclLine;
        NewEntry->DeclColumn = DeclColumn;
        NewEntry->p.v.Key = Key;
        NewEntry->p.v.Val = Val;
        NewEntry->Next = Tbl->HashTable[AddAt];
        Tbl->HashTable[AddAt] = NewEntry;
        return TRUE;
    }

    return FALSE;
}

/* 找到一个表中的的值。如果没有找到返回FALSE. 
 * key 必须是一个共享的字符串 TableStrRegister() */
int TableGet(struct Table *Tbl, const char *Key, struct Value **Val, const char **DeclFileName, int *DeclLine, int *DeclColumn)
{
    int AddAt;
    struct TableEntry *FoundEntry = TableSearch(Tbl, Key, &AddAt);
    if (FoundEntry == NULL)
        return FALSE;
    
    *Val = FoundEntry->p.v.Val;
    
    if (DeclFileName != NULL)
    {
        *DeclFileName = FoundEntry->DeclFileName;
        *DeclLine = FoundEntry->DeclLine;
        *DeclColumn = FoundEntry->DeclColumn;
    }
    
    return TRUE;
}

/* remove an entry from the table */
struct Value *TableDelete(struct Table *Tbl, const char *Key)
{
    struct TableEntry **EntryPtr;
    int HashValue = ((unsigned long)Key) % Tbl->Size;   /* 共享字符串具有唯一的地址，所以我们不需要散列他们 */
    
    for (EntryPtr = &Tbl->HashTable[HashValue]; *EntryPtr != NULL; EntryPtr = &(*EntryPtr)->Next)
    {
        if ((*EntryPtr)->p.v.Key == Key)
        {
            struct TableEntry *DeleteEntry = *EntryPtr;
            struct Value *Val = DeleteEntry->p.v.Val;
            *EntryPtr = DeleteEntry->Next;
            HeapFreeMem(DeleteEntry);

            return Val;
        }
    }

    return NULL;
}

/* 检查哈希表项的符号入口 */
static struct TableEntry *TableSearchIdentifier(struct Table *Tbl, const char *Key, int Len, int *AddAt)
{
    struct TableEntry *Entry;
    int HashValue = TableHash(Key, Len) % Tbl->Size;
    
    for (Entry = Tbl->HashTable[HashValue]; Entry != NULL; Entry = Entry->Next)
    {
        if (memcmp(&Entry->p.Key[0], (char *)Key, Len) == 0 && Entry->p.Key[Len] == '\0')
            return Entry;   /* 发现 */
    }
    
    *AddAt = HashValue;    /* 没有在链表中找到它 */
    return NULL;
}

/* 设置一个符号和返回的符号。份额如果可能的话 */
char *TableSetIdentifier(struct Table *Tbl, const char *Ident, int IdentLen)
{
    int AddAt = 0;
    struct TableEntry *FoundEntry = TableSearchIdentifier(Tbl, Ident, IdentLen, &AddAt);
    
    if (FoundEntry != NULL)
        return &FoundEntry->p.Key[0];
    else
    {   /* 将它添加到表 - 我们不分配的整体结构，节约 */
        struct TableEntry *NewEntry = HeapAllocMem(sizeof(struct TableEntry) - sizeof(union TableEntryPayload) + IdentLen + 1);
        if (NewEntry == NULL)
            ProgramFail(NULL, "内存不足");
            
        memcpy((char *)&NewEntry->p.Key[0], (char *)Ident, IdentLen);
        NewEntry->p.Key[IdentLen] = '\0';
        NewEntry->Next = Tbl->HashTable[AddAt];
        Tbl->HashTable[AddAt] = NewEntry;
        return &NewEntry->p.Key[0];
    }
}

/* 注册共享字符串 */
char *TableStrRegister2(const char *Str, int Len)
{
    return TableSetIdentifier(&StringTable, Str, Len);
}

char *TableStrRegister(const char *Str)
{
    return TableStrRegister2(Str, strlen((char *)Str));
}

/* 释放所有的字符串 */
void TableStrFree(void)
{
    struct TableEntry *Entry;
    struct TableEntry *NextEntry;
    int Count;
    
    for (Count = 0; Count < StringTable.Size; Count++)
    {
        for (Entry = StringTable.HashTable[Count]; Entry != NULL; Entry = NextEntry)
        {
            NextEntry = Entry->Next;
            HeapFreeMem(Entry);
        }
    }
}
