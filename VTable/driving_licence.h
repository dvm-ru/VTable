#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

// предварительная декларация
class IdentityDocument;
class DrivingLicence;

struct VTable3 {								                // перечислить все виртуальные функции Foo
    const void* parentVtable;					                // (одиночное) наследство

    void (DrivingLicence::* PrintID)() const;
    void (DrivingLicence::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();
    void (DrivingLicence::* Delete)();
};

class DrivingLicence {
public:
    DrivingLicence() 
        : identity_(IdentityDocument())
        , vtable_ptr_(&DrivingLicence::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);

        std::cout << "DrivingLicence::Ctor()"sv << std::endl;
    }

    DrivingLicence(const DrivingLicence& other)
        : identity_(other.identity_)
        , vtable_ptr_(&DrivingLicence::VTABLE)
    {
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    template <typename T>
    DrivingLicence(const T& other)
        : identity_((&other.driving_licence_)->identity_)
        , vtable_ptr_(&DrivingLicence::VTABLE)
    {
        std::cout << "DrivingLicence::CCtor()"sv << std::endl;
    }

    ~DrivingLicence() {
        if (identity_.vtable_ptr_ == reinterpret_cast<VTable*>(&DrivingLicence::VTABLE)) {
            identity_.vtable_ptr_ = &IdentityDocument::VTABLE;
        }
        std::cout << "DrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "DrivingLicence::PrintID() : "sv << (&identity_)->GetID() << std::endl;
    }

    template <typename T>
    operator T() {
        auto* base_ptr = reinterpret_cast<T*>(this);
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(&IdentityDocument::VTABLE);
        return *base_ptr;
    }

    void Delete() {
        this->~DrivingLicence();
    }

public:
    static VTable3 VTABLE; //static, так как таблица одна на весь класс

    IdentityDocument identity_;

    VTable3* vtable_ptr_ = nullptr;

private:
    
};

VTable3 DrivingLicence::VTABLE = {                      // только одна постоянная таблица
    &IdentityDocument::VTABLE,                          // родительский класс

    &DrivingLicence::PrintID,
    0,
    0,
    &DrivingLicence::Delete,
};