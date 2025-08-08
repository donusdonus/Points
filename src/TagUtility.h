#ifndef ___TAG_UTILITY_H____
#define ___TAG_UTILITY_H____

#include <stdio.h>
#include <stdlib.h>
#include <TagStatic.h>

class TagUtility
{
private:
    bool monitor = false;
    Topic **TOPIC_PTR[3] = {nullptr,nullptr,nullptr};

    bool SetName(RawMemory *src,const char *name,isMemory memtype = isMemory::RAM)
    {
        
        monitor = (src->size > 0);
        if(monitor)
        {
            memset(src->value,MARK_FREE_MEMORY,src->size);
            memset(&src->size,MARK_FREE_MEMORY,sizeof(src->value));
            free(src->value);
        }
        
        src->size = strlen(name) + 1;
        
        monitor = (src->value == nullptr);
        if(!monitor)
            return monitor;

        src->value = (uint8_t*)Allocator(memtype,src->size,sizeof(uint8_t));
        monitor = (src->value != nullptr);
        if(!monitor)
            return monitor;

        memcpy(src->value,name,src->size-1);
        src->value[src->size-1] = MARK_CHAR_NULL;

        return monitor;
    }

//    bool SetName(RawMemory 
    /* data */
public:
    TagUtility(/* args */);
    ~TagUtility();

#pragma region "Topic function"

    Topic * TopicAdd(Topic **src,const char *name,isMemory MemType = isMemory::RAM)
    {
        Topic **cur = src;

        /* 1. Find last elements for connect */
        while ((cur != nullptr) && (*cur != nullptr))
        {
            cur = &(*cur)->Next;
        }
        
        /* 2. Allocate and Check Result is Complete ? */
        Topic *cur_alloc = (Topic*)Allocator(MemType,1,sizeof(Topic));
        monitor = (cur_alloc != nullptr);
        if(!monitor)
            return nullptr;

        /* 3. Assign Name of Topic */
        monitor = SetName(&cur_alloc->name,name,MemType);
        if(!monitor)
            return nullptr;

        /* 4. Assign Memory Type usage of Topic */
        (*cur_alloc).memType = MemType;

        /* 5. mark pointer topic to current */
        *cur = cur_alloc;

        /* 6. mark pointer as home */
        (*cur)->Home = *src;
        
        return *cur;   
    }

    size_t TopicCount(Topic **src)
    {
        size_t count = 0 ;
        Topic **cur = src;

        /* 1. Set Topic position at first topic */
        *cur = (*cur)->Home;

        /* 2. Count Topic and next position topic */
        while((cur != nullptr) && ((*cur) !=nullptr))
        {
            cur = &(*cur)->Next;
            count++;
        }

        return count;
    }

    Topic * TopicFindByName(Topic **src,const char * name)
    {
        monitor = false;

        Topic **cur = src;

        /* 1. Set Topic position at first topic */
        *cur = (*cur)->Home;

        /* 2. Compare Name string at current Topic */
        while((cur != nullptr) && ((*cur) !=nullptr))
        {
            monitor = (strcmp((char*)(*cur)->name.value,name) == 0);
            if(monitor)
            {
                return *cur;
            }
            else 
            {
                cur = &(*cur)->Next;
            }
        }

        return nullptr;
    }
    
    Topic * TopicFindByIndex(Topic **src,size_t index)
    {
         monitor = false;

         Topic **cur = src;
         
        /* 1. Set Topic position at first topic */
         *cur = (*cur)->Home;

        /* 2. Count until at index of until false in condition */
        size_t count = 0;
        while (count <= index)
        {
            monitor = (cur != nullptr) && ((*cur) !=nullptr);
            if(!monitor)
            {
                return nullptr;
            }
            else 
            {
                if(count == index)
                    return *cur;
                cur = &(*cur)->Next;
            }
            count++;
        }
        
        return nullptr;
    }
#pragma endregion

#pragma region "GroupItem function"
    Tag * ItemAdd(Topic **src,isType type,const char * name,size_t array_size=1,isMemory memtype = isMemory::RAM)
    {
        TagGroup **cur = nullptr;

        cur = &(*src)->Group;

        /* 1. Find last elements for connect */
        while ((cur != nullptr) && (*cur != nullptr))
        {
            cur = &(*cur)->Next;
        }

        TagGroup *newItem = (TagGroup*)Allocator(memtype,array_size,sizeof(TagGroup));
        
        monitor = (newItem != nullptr);
        if(!monitor)
            return nullptr;
    
        monitor = SetName(&newItem->Item.name,name,memtype);
        if(!monitor)
            return nullptr;

        newItem->Item.data.value = (uint8_t*)Allocator(memtype,array_size,SchematicPoint[type].element_size);

        monitor = (newItem->Item.data.value != nullptr);
        if(!monitor)
            return nullptr;

        newItem->Item.data.size = SchematicPoint[type].element_size * array_size;
        newItem->Item.memType = memtype;
        newItem->Item.type = type;


        *cur = newItem;
        //(*cur)->Home = (*src)->Group;
     

        return nullptr;
    }
    
    Tag * ItemAdd(Topic **src,isType type,const char * name,void *addr,size_t array_size=1,isMemory memtype = isMemory::RAM)
    {
         TagGroup **cur = &(*src)->Home->Group;

        /* 1. Find last elements for connect */
        while ((cur != nullptr) && (*cur != nullptr))
        {
            cur = &(*cur)->Next;
        }

        TagGroup *newItem = (TagGroup*)Allocator(memtype,array_size,sizeof(TagGroup));
        
        monitor = (newItem != nullptr);
        if(!monitor)
            return nullptr;
    
        monitor = SetName(&newItem->Item.name,name,memtype);
        if(!monitor)
            return nullptr;

        newItem->Item.data.value = (uint8_t*)addr;

        monitor = (newItem->Item.data.value != nullptr);
        if(!monitor)
            return nullptr;

        newItem->Item.data.size = SchematicPoint[type].element_size * array_size;
        newItem->Item.memType = memtype;
        newItem->Item.type = type;
        newItem->Item.external_alloc = 1;


        *cur = newItem;
        (*cur)->First = (*src)->Group;
     

        return nullptr;   
    }

    size_t ItemCount(Topic **src)
    {
        size_t count = 0;
        TagGroup **cur = nullptr;

        cur =  &(*src)->Group;

        while((cur != nullptr) && (*cur != nullptr))
        {
            count+=1;
            cur = &(*cur)->Next;
        }
        return count;
    }
    
    Tag * ItemFindByName(Topic **src,const char *name)
    {
        TagGroup **cur = nullptr;

        cur =  &(*src)->Group;

        while((cur != nullptr) && (*cur != nullptr))
        {
            if(strcmp((char*)(*cur)->Item.name.value,name) == 0)
                return &(*cur)->Item;
            cur = &(*cur)->Next;
        }

        return nullptr;
    }
    
    Tag * ItemFindByIndex(Topic **src,size_t index)
    {
        size_t count = 0 ;
        TagGroup **cur = nullptr;

        cur =  &(*src)->Group;

        while((cur != nullptr) && (*cur != nullptr))
        {
            if(count == index)
                return &(*cur)->Item;
            
            cur = &(*cur)->Next;
            count+=1;
        }

        return nullptr;
    }

#pragma endregion

#pragma region "Tag function"
    
 


#pragma endregion

};

TagUtility::TagUtility(/* args */)
{

}

TagUtility::~TagUtility()
{

}



#endif