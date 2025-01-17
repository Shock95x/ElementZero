#pragma once

#include <string>
#include "../Core/mce.h"
#include "../dll.h"

// class Certificate;

class ExtendedCertificate {
public:
    static MCAPI std::string getXuid(class Certificate const &);
    static MCAPI mce::UUID getIdentity(class Certificate const &);
    static MCAPI std::string getIdentityName(class Certificate const &);
};