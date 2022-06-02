#pragma once

#include <iostream>
#include <string>


using namespace std::string_view_literals;

// ��������������� ����������
class IdentityDocument;

struct VTable {								            // ����������� ��� ����������� �������
    const void* parentVtable;					        // (���������) ����������

    void (IdentityDocument::* PrintID)() const;
    void (IdentityDocument::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();      //��� ����� ������������ �������: static
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
    static VTable VTABLE; //static, ��� ��� ������� ���� �� ���� �����

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

VTable IdentityDocument::VTABLE = {             // ������ ���� ���������� �������
    0,                                          // ��� ������������� ������

    &IdentityDocument::PrintID,
    0,
    &PrintUniqueIDCount_IdentityDocument,       //��� ����� ������������ �������: ��� static    
    &IdentityDocument::Delete
};
