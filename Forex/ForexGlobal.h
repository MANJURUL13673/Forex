#pragma once
#include "Logger.h"
#include "Database.h"
#include "OTPHandle.h"
#include <memory>

class CForexGlobal
{
public:
	CForexGlobal();
	std::unique_ptr<CLogger>lgr;
	std::unique_ptr<CDatabase>db;
	COTPHandle otphandler;
};

extern CForexGlobal pFGlobal;

