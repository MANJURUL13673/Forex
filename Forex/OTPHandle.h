#pragma once
#pragma comment(lib, "wldap32.lib" )
#pragma comment(lib, "crypt32.lib" )
#pragma comment(lib, "Ws2_32.lib")

#define CURL_STATICLIB 
#include "curl.h"

class COTPHandle
{
private:
	std::string to;
	std::string smtpServer;
	std::string username;
	std::string from;
	std::string password;
	std::string otp;
	std::string generateOTP(int length);
	void sendMail(std::string emailBody);

public:
	COTPHandle();
	void init(const std::string& userMail);
	void sendEmailWithOTP();
};
