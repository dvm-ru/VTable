#pragma once

#include "identity_document.h"
#include <iostream>
#include <string>
#include <ctime>

using namespace std::string_view_literals;

// предварительная декларация
class IdentityDocument;
class Passport;

struct VTable2 {								                // перечислить все виртуальные функции Foo
    const void* parentVtable;					                // (одиночное) наследство

    void (Passport::* PrintID)() const;
    void (Passport::* PrintVisa)(const std::string& country) const;
    void (*PrintUniqueIDCount_IdentityDocument)();
    void (Passport::* Delete)();
};

class Passport {
public:
    Passport()
        : identity_(IdentityDocument())
        , expiration_date_(GetExpirationDate())
        , vtable_ptr_(&Passport::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);

        std::cout << "Passport::Ctor()"sv << std::endl;        
    }

    Passport(const Passport& other)
        : identity_(other.identity_)
        , expiration_date_(other.expiration_date_)
        , vtable_ptr_(&Passport::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);
    
        std::cout << "Passport::CCtor()"sv << std::endl;        
    }
    template <typename T>
    Passport(const T& other)
        : identity_(other.identity_)
        , expiration_date_(other.expiration_date_)
        , vtable_ptr_(&Passport::VTABLE)
    {
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(vtable_ptr_);

        std::cout << "Passport::CCtor()"sv << std::endl;
    }

    ~Passport() {
        if (identity_.vtable_ptr_ == reinterpret_cast<VTable*>(&Passport::VTABLE)) {
            identity_.vtable_ptr_ = &IdentityDocument::VTABLE;
        }
        std::cout << "Passport::Dtor()"sv << std::endl;
    }

    void PrintID() const {
        std::cout << "Passport::PrintID() : "sv << (&identity_)->GetID();
        std::cout << " expiration date : "sv << expiration_date_.tm_mday << "/"sv << expiration_date_.tm_mon << "/"sv
            << expiration_date_.tm_year + 1900 << std::endl;
    }

    void PrintVisa(const std::string& country) const {
        std::cout << "Passport::PrintVisa("sv << country << ") : "sv << (&identity_)->GetID() << std::endl;
    }

    void PrintUniqueIDCount() {
        IdentityDocument::VTABLE.PrintUniqueIDCount_IdentityDocument();
    }

    template <typename T>
    operator T() {
        auto* base_ptr = reinterpret_cast<T*>(this);        
        identity_.vtable_ptr_ = reinterpret_cast<VTable*>(&IdentityDocument::VTABLE);
        return *base_ptr;
    }

    void Delete() {
        this->~Passport();
    }

    tm GetExpirationDate() {
        time_t t = time(nullptr);
        tm exp_date = *localtime(&t);
        exp_date.tm_year += 10;
        mktime(&exp_date);
        return exp_date;
    }

public:
    static VTable2 VTABLE; //static, так как таблица одна на весь класс

    IdentityDocument identity_;

private:
    const struct tm expiration_date_;
    VTable2* vtable_ptr_ = nullptr;
};

VTable2 Passport::VTABLE = {                     // только одна постоянная таблица
    &IdentityDocument::VTABLE,                  // родительский класс

    &Passport::PrintID,
    &Passport::PrintVisa,
    0,
    &Passport::Delete,
};