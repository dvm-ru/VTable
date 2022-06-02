#pragma once

#include "driving_licence.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

// предварительная декларация
class IdentityDocument;
class InternationalDrivingLicence;
class DrivingLicence;

struct VTable4 {								                // перечислить все виртуальные функции Foo
    const void* parentVtable;					                // (одиночное) наследство

    void (InternationalDrivingLicence::* PrintID)() const;
    void (InternationalDrivingLicence::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();      //тут вызов произвольной функции: static
    void (InternationalDrivingLicence::* Delete)();
};

class InternationalDrivingLicence {
public:
    InternationalDrivingLicence()
        : driving_licence_(DrivingLicence())
        , vtable_ptr_(&InternationalDrivingLicence::VTABLE)
    {
        driving_licence_.vtable_ptr_ = reinterpret_cast<VTable3*>(vtable_ptr_);

        std::cout << "InternationalDrivingLicence::Ctor()"sv << std::endl;
    }

    InternationalDrivingLicence(const InternationalDrivingLicence& other)
        : driving_licence_(DrivingLicence(other))
        , vtable_ptr_(&InternationalDrivingLicence::VTABLE)
    {
        driving_licence_.vtable_ptr_ = reinterpret_cast<VTable3*>(vtable_ptr_);

        std::cout << "InternationalDrivingLicence::CCtor()"sv << std::endl;
    }

    ~InternationalDrivingLicence() {
        if (driving_licence_.vtable_ptr_ == reinterpret_cast<VTable3*>(&InternationalDrivingLicence::VTABLE)) {
            driving_licence_.vtable_ptr_ = &DrivingLicence::VTABLE;
        }
        std::cout << "InternationalDrivingLicence::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "InternationalDrivingLicence::PrintID() : "sv << (&(&driving_licence_)->identity_)->GetID() << std::endl;
    }

    template <typename T>
    operator T() {
        if (driving_licence_.vtable_ptr_ == reinterpret_cast<VTable3*>(&InternationalDrivingLicence::VTABLE)) {
            driving_licence_.vtable_ptr_ = &DrivingLicence::VTABLE;
        }
        auto* base_ptr = reinterpret_cast<T*>(this);
        return *base_ptr;
    }

public:
    static VTable4 VTABLE;

    DrivingLicence driving_licence_;

    VTable4* vtable_ptr_ = nullptr;

private:
    
};


VTable4 InternationalDrivingLicence::VTABLE = {                     // только одна постоянная таблица
    &IdentityDocument::VTABLE,                                      // родительский класс

    &InternationalDrivingLicence::PrintID,
    0,
    &PrintUniqueIDCount_IdentityDocument,       //тут вызов произвольной функции: для static    
    0
};