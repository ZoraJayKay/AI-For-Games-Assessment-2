#include <assert.h>
#include <iostream>
#include <string>

namespace MemoryDebugger {

    struct Memblk
    {
        void* data = nullptr;
        const char* file = nullptr;
        const char* func = nullptr;
        int line = 0;

        Memblk& operator=(const Memblk& o) {
            data = o.data;
            file = o.file;
            func = o.func;
            line = o.line;
            return *this;
        }
    };

    std::ostream& operator << (std::ostream& of, const Memblk& b) {
        of << "<--> 0x" << std::hex << reinterpret_cast<unsigned int>(b.data) << std::dec << " " << b.file << "," << b.line << " : " << b.func << "();";
        return of;
    }

    struct MemoryModule {
        //int memory_module = 0;
        int reserved;
        Memblk* memory;
        int count;
        unsigned int allocations;
        unsigned int deallocations;

        MemoryModule()
            : reserved{ 10 }
            , memory{ (Memblk*)malloc(sizeof(Memblk) * reserved) }
            , count{ 0 }
            , allocations{ 0 }
            , deallocations{ 0 }
        {
        }

        ~MemoryModule() {
            std::cout << "----------------------------------------------"<<std::endl;
            std::cout << "<--> Alloc\t" << allocations << std::endl;
            std::cout << "<--> Dealloc\t" << deallocations << std::endl;
            std::cout << "<--> Unfreed\t" << count << std::endl;
            std::cout << std::endl;

            for (int i = 0; i < count; ++i) {
                std::cout << memory[i] << std::endl;
                free(memory[i].data);
            }
            std::cout << "----------------------------------------------"<<std::endl;
            free(memory);
        }

        void* alloc(size_t p, Memblk b) {
            if (reserved == count) {
                reserved *= 2;
                Memblk* new_mem = (Memblk*)malloc(sizeof(Memblk) * reserved);
                memcpy(new_mem, memory, sizeof(Memblk) * count);
                free(memory);
                memory = new_mem;
            }
            b.data = malloc(p);
            auto l = strlen(b.file);
            b.file = b.file + l;
            while (*(b.file - 1) != '\\')
                b.file--;
            memory[count++] = b;
            allocations++;
            return b.data;
        }

        void dealloc(void* ptr) {
            for (int i = 0; i < count; ++i) {
                if (memory[i].data == ptr) {
                    free(memory[i].data);
                    count--;
                    memory[i] = memory[count];
                    deallocations++;
                    memset(&memory[count], 0xfe, sizeof(Memblk));
                    return;
                }
            }
        }

    } module;
}

class UnrecoverablePointer
{
    void* p;
    bool copied;
public:
    UnrecoverablePointer(void* ptr) : p{ ptr }, copied{ false } {

    }
    ~UnrecoverablePointer() {
        if (!copied)
            std::cout << "<-->leak of " + std::to_string(reinterpret_cast<unsigned int>(p));
    }
    template<typename O>
    operator O* () const {
        copied = true;
        return static_cast<O*>(p);
    }
};

void* operator new (size_t size) {
    const auto ptr = MemoryDebugger::module.alloc(size, MemoryDebugger::Memblk{ 0,"new", "new", 79 });
    //std::cout <<      "<--> unknown new " << MemoryDebugger::module.count << " 0x" << ptr << "new" << std::endl;
    return ptr;
}

void* operator new (size_t size, MemoryDebugger::Memblk b) {
    const auto ptr = MemoryDebugger::module.alloc(size, b);
    std::cout <<          "<--> new\t" << MemoryDebugger::module.count << " 0x" << ptr << std::endl;
    return ptr;
}

void operator delete(void* ptr) {
    char output[1024];
    int l = 0;
    if (ptr == nullptr) {
        sprintf_s(&output[0], 1023,"<--> delete\t\t-NULLPTR-\n");//output = "---" "dealloc null" "\n";
    }
    else {
        MemoryDebugger::module.dealloc(ptr);
        sprintf_s(output, 1023, "<--> delete\t%u\t0x%u\n", MemoryDebugger::module.count, reinterpret_cast<unsigned int>(ptr));
    }
    std::cout << output;
}

#define new new(MemoryDebugger::Memblk{(void*)0,__FILE__,__func__,__LINE__} )