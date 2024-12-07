#pragma once
#include "Logger.h"
#include "Database.h"
#include "OTPHandle.h"
#include <memory>

struct SUserProperty
{
    std::string mobileNo;
    std::string email;
    std::string resetKey;
    std::string passwords[4];
    std::string otp;
};

class CForexGlobal
{
public:
	CForexGlobal();
	std::unique_ptr<CLogger>lgr;
	std::unique_ptr<CDatabase>db;
    std::unique_ptr<SUserProperty>usr;
    void resetUsr(std::string email, std::string mobileNo, std::string resetKey);
	COTPHandle otphandler;
};

extern CForexGlobal pFGlobal;

