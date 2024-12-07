#include "pch.h"
#include "ForexGlobal.h"

CForexGlobal pFGlobal;

CForexGlobal::CForexGlobal()
{
	lgr = nullptr;
	db = nullptr;
	usr = nullptr;
}

void CForexGlobal::resetUsr(std::string email, std::string mobileNo, std::string resetKey)
{
    if (pFGlobal.usr == nullptr)
    {
        pFGlobal.usr = std::make_unique<SUserProperty>();
    }
    else
    {
        pFGlobal.usr.reset(new SUserProperty());
    }
    pFGlobal.usr->mobileNo = mobileNo;
    pFGlobal.usr->email = email;
    pFGlobal.usr->resetKey = resetKey;
}