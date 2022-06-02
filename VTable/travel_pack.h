#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>

using namespace std::string_view_literals;

// предварительная декларация
class IdentityDocument;
class Passport;
class DrivingLicence;
class TravelPack;

struct VTable5 {								                // перечислить все виртуальные функции Foo
    const void* parentVtable;					                // (одиночное) наследство

    void (TravelPack::* PrintID)() const;
    void (TravelPack::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();      //тут вызов произвольной функции: static
    void (TravelPack::* Delete)();
};

class TravelPack {
public:
    static VTable5 VTABLE;
    IdentityDocument identity_;

public:
    TravelPack()
        : identity_doc1_(reinterpret_cast<IdentityDocument*>(new Passport()))
        , identity_doc2_(reinterpret_cast<IdentityDocument*>(new DrivingLicence()))

        , vtable_ptr_(&TravelPack::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);

        std::cout << "TravelPack::Ctor()"sv << std::endl;
    }

    TravelPack(const TravelPack& other)
        : identity_(other.identity_)

        , identity_doc1_(reinterpret_cast<IdentityDocument*>(new Passport(*reinterpret_cast<Passport*>(other.identity_doc1_))))
        , identity_doc2_(reinterpret_cast<IdentityDocument*>(new DrivingLicence(*reinterpret_cast<DrivingLicence*>(other.identity_doc2_))))

        , additional_pass_(other.additional_pass_)
        , additional_dr_licence_(other.additional_dr_licence_)

        , vtable_ptr_(&TravelPack::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);

        std::cout << "TravelPack::CCtor()"sv << std::endl;
    }

    ~TravelPack() {
        if (identity_.vtable_ptr_ == reinterpret_cast<VTable*>(&TravelPack::VTABLE)) {
            identity_.vtable_ptr_ = &IdentityDocument::VTABLE;
        }
        //delete identity_doc1_;
        //delete identity_doc2_;
        identity_doc1_->Delete();
        identity_doc2_->Delete();
        std::cout << "TravelPack::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        identity_doc1_->PrintID();
        identity_doc2_->PrintID();
        additional_pass_.PrintID();
        additional_dr_licence_.PrintID();
    }

    template <typename T>
    operator T() {
        auto* base_ptr = reinterpret_cast<T*>(this);
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(&IdentityDocument::VTABLE);
        return *base_ptr;
    }

    void Delete() {
        this->~TravelPack();
    }

    void PrintUniqueIDCount() {
        IdentityDocument::VTABLE.PrintUniqueIDCount_IdentityDocument();
    }

private:
    IdentityDocument* identity_doc1_;
    IdentityDocument* identity_doc2_;
    Passport additional_pass_;
    DrivingLicence additional_dr_licence_;

private:
    VTable5* vtable_ptr_ = nullptr;

//public:
//    DrivingLicence driving_licence_;

};

void PrintUniqueIDCount_IdentityDocument2() {
    IdentityDocument::PrintUniqueIDCount();
}

VTable5 TravelPack::VTABLE = {                      // только одна постоянная таблица
    &IdentityDocument::VTABLE,                      // родительский класс

    &TravelPack::PrintID,
    0,
    &PrintUniqueIDCount_IdentityDocument2,       //тут вызов произвольной функции: для static    
    &TravelPack::Delete
};