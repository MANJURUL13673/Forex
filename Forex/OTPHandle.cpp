#include "pch.h"
#include "OTPHandle.h"
#include <iostream>
#include <sstream>

COTPHandle::COTPHandle()
{
	smtpServer = "smtp://smtp.gmail.com:587";
    username = "easiteksolutions@gmail.com";
	from = "easiteksolutions@gmail.com";
	password = "dxonslzvshjwsbau";
}

void COTPHandle::init(const std::string& userMail)
{
    to = userMail;
    otp = generateOTP(6);
}

std::string COTPHandle::generateOTP(int length = 6) {
    srand(static_cast<unsigned>(time(0)));
    std::stringstream ss;
    for (int i = 0; i < length; ++i) {
        ss << rand() % 10;  // Generate a random digit between 0-9
    }
    return ss.str();
}

size_t read_callback(void* ptr, size_t size, size_t nmemb, void* userp) {
    const char** upload_data = static_cast<const char**>(userp);
    size_t buffer_size = size * nmemb;
    //OutputDebugString(L"Hello from callback!\n");
    if (*upload_data && **upload_data != '\0') {
        std::wstring temp;
        size_t len = strlen(*upload_data);
        temp = std::to_wstring(len) + L"\n";
        //OutputDebugString(temp.c_str());
        if (len > buffer_size) len = buffer_size;

        memcpy(ptr, *upload_data, len);
        *upload_data += len; // Advance pointer
        return len;
    }
    return 0; // No more data to send
}

void COTPHandle::sendMail(std::string emailBody)
{
    CURL* curl;
    CURLcode res;
    struct curl_slist* recipients = NULL;

    const char* upload_data = emailBody.c_str();

    curl = curl_easy_init();
    int lines_read = 0;
    if (curl) {
        // Set SMTP URL
        curl_easy_setopt(curl, CURLOPT_URL, smtpServer.c_str());
        curl_easy_setopt(curl, CURLOPT_USE_SSL, CURLUSESSL_ALL);
        // Set username and password
        curl_easy_setopt(curl, CURLOPT_USERNAME, username.c_str());
        curl_easy_setopt(curl, CURLOPT_PASSWORD, password.c_str());

        // Set the sender
        std::string sender = "<" + from + ">";
        curl_easy_setopt(curl, CURLOPT_MAIL_FROM, sender.c_str());

        // Set the recipients
        std::string receiver = "<" + to + ">";
        recipients = curl_slist_append(recipients, receiver.c_str());
        curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

        curl_easy_setopt(curl, CURLOPT_READFUNCTION, read_callback);
        // Set the email body
        curl_easy_setopt(curl, CURLOPT_READDATA, &upload_data);
        // Enable verbose output
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if (res != CURLE_OK)
            OutputDebugString(L"curl_easy_perform() failed: ");

        // Cleanup
        curl_slist_free_all(recipients);
        curl_easy_cleanup(curl);
    }
}

void COTPHandle::sendEmailWithOTP()
{
    std::string email_body = "";
    email_body += "To: " + to + "\r\n";
    email_body += "From: " + from + "\r\n";
    email_body += "Subject: OTP from Forex\r\n";
    email_body += "\r\n";
    email_body += "Hi, \r\n";
    email_body += "User tried login in your Forex account. If you are not the user, please do not share OTP with anyone.\r\n";
    email_body += "OTP: " + otp + "\r\n";
    email_body += "\r\n";
    email_body += "Regards,\r\n";
    email_body += "Forex Team";

    sendMail(email_body);
}
