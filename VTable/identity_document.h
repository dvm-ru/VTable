#pragma once

#include <iostream>
#include <string>


using namespace std::string_view_literals;

// предварительная декларация
class IdentityDocument;

struct VTable {								            // перечислить все виртуальные функции
    const void* parentVtable;					        // (одиночное) наследство

    void (IdentityDocument::* PrintID)() const;
    void (IdentityDocument::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();      //тут вызов произвольной функции: static
    void (IdentityDocument::* Delete)();
};

class IdentityDocument {
private:
    static int unique_id_count_;
    int unique_id_;
public:
    IdentityDocument()
        : unique_id_(++unique_id_count_)
        , vtable_ptr_(&IdentityDocument::VTABLE)
    {
        std::cout << "IdentityDocument::Ctor() : "sv << unique_id_ << std::endl;
    }

    ~IdentityDocument() {
        --unique_id_count_;
        std::cout << "IdentityDocument::Dtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument(const IdentityDocument& other)
        : unique_id_(++unique_id_count_)
        , vtable_ptr_(other.vtable_ptr_)
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    template <typename T>
    IdentityDocument(const T& other)
        : unique_id_(++unique_id_count_)
        , vtable_ptr_(other.vtable_ptr_)
    {
        std::cout << "IdentityDocument::CCtor() : "sv << unique_id_ << std::endl;
    }

    IdentityDocument& operator=(const IdentityDocument&) = delete;

    void PrintID() const {
		if (&IdentityDocument::PrintID == vtable_ptr_->PrintID) {
			std::cout << "IdentityDocument::PrintID() : "sv << unique_id_ << std::endl;
		}
		else {
			(this->*(vtable_ptr_->PrintID))();
		}
    }

     static void PrintUniqueIDCount() {
        std::cout << "unique_id_count_ : "sv << unique_id_count_ << std::endl;
    }

    template <typename T>
    operator T() {
        IdentityDocument* base_ptr = reinterpret_cast<IdentityDocument*>(this);
        vtable_ptr_ = &IdentityDocument::VTABLE;
        return *base_ptr;
    }

    void Delete() {
        if (&IdentityDocument::Delete == vtable_ptr_->Delete) {
            this->~IdentityDocument();
        }
        else {
            (this->*(vtable_ptr_->Delete))();
        }
    }

public:
    static VTable VTABLE; //static, так как таблица одна на весь класс

    VTable* vtable_ptr_ = nullptr;

//protected:
    int GetID() const {
        return unique_id_;
    }
};

int IdentityDocument::unique_id_count_ = 0;


void PrintUniqueIDCount_IdentityDocument() {
    IdentityDocument::PrintUniqueIDCount();
}

VTable IdentityDocument::VTABLE = {             // только одна постоянная таблица
    0,                                          // нет родительского класса

    &IdentityDocument::PrintID,
    0,
    &PrintUniqueIDCount_IdentityDocument,       //тут вызов произвольной функции: для static    
    &IdentityDocument::Delete
};
