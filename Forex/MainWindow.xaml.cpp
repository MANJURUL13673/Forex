// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#include <iostream>
#include <string>
#include <fstream>
#include <Windows.h>
#include <regex>
#include "ForexGlobal.h"

#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::Forex::implementation
{
    MainWindow::MainWindow()
    {
        InitializeComponent();
        
        // Initialize Log
        if (pFGlobal.lgr == nullptr)
        {
            pFGlobal.lgr = std::make_unique<CLogger>("E:/LOG/log");
        }
        
        // Initialize server and database
        if (pFGlobal.db == nullptr)
        {
            pFGlobal.db = std::make_unique<CDatabase>("localhost", "admin", "root");
        }

        // Create required tables
        std::string userTableName = "UserInfo";
        if (!pFGlobal.db->checkTableExist(userTableName))
        {
            std::string createUserTable =
                "CREATE TABLE " + userTableName + " ("
                "user_id INT PRIMARY KEY AUTO_INCREMENT, "
                "email VARCHAR(255) UNIQUE NOT NULL, "
                "mobile_no VARCHAR(255) NOT NULL, "
                "password_1 VARCHAR(255) NOT NULL, "
                "password_2 VARCHAR(255) NOT NULL, "
                "password_3 VARCHAR(255) NOT NULL, "
                "password_4 VARCHAR(255) NOT NULL, "
                "reset_key VARCHAR(255) NOT NULL"
                ")";
            pFGlobal.db->createTable(createUserTable);
        }

        std::string otpTableName = "OTP";
        if (!pFGlobal.db->checkTableExist(otpTableName))
        {
            std::string createOtpTable =
                "CREATE TABLE " + otpTableName + " ("
                "otp_id INT PRIMARY KEY AUTO_INCREMENT, "
                "user_id INT NOT NULL, "
                "otp_code VARCHAR(6) NOT NULL, "
                "created_at DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP, "
                "expires_at DATETIME NOT NULL, "
                "is_used BOOLEAN NOT NULL DEFAULT FALSE, "
                "attempts INT NOT NULL DEFAULT 0, "
                "FOREIGN KEY (user_id) REFERENCES " + userTableName + "(user_id) ON DELETE CASCADE"
                ")";
        }

        isPasswordValid = FALSE; 
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
    void MainWindow::OnBtnPasswordValidation(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        // extract email and password data
        winrt::hstring temp;
        temp = EmailBox().Text();
        std::string email = winrt::to_string(temp);
        std::string password[4];
        temp = Password1Box().Password();
        password[0] = winrt::to_string(temp);   
        temp = Password2Box().Password();
        password[1] = winrt::to_string(temp);
        temp = Password3Box().Password();
        password[2] = winrt::to_string(temp);
        temp = Password4Box().Password();
        password[3] = winrt::to_string(temp);

        // Check if email or password is empty or not
        if (email.empty())
        {
            WrongPWWarning().Text(L"*Email should not\n be empty");
            return;
        }
        for (int i = 0; i < 4; i++) // four password
        {
            if (password[i].empty())
            {
                WrongPWWarning().Text(L"*Password should\n not be empty");
                return;
            }
        }
        if (!isValidMail(email))
        {
            WrongPWWarning().Text(L"Email is not correct");
            return;
        }

        // check if the mail is already registered
        std::string cmd =
            "SELECT COUNT(*) AS count "
            "FROM UserInfo "
            "WHERE email = '" + email + "'";
        if (pFGlobal.db->sendTableQuery(cmd))
        {
            sql::ResultSet* res = pFGlobal.db->GetCurrentResult();
            if (res->next())
            {
                int count = res->getInt(1);
                if (count == 0)
                {
                    WrongPWWarning().Text(L"*User not found!");
                    return;
                }
            }
            else
            {
                exit(0);
            }
        }
        else
        {
            exit(0);
        }

        std::string query =
            "SELECT * FROM UserInfo "
            "WHERE email = '" + email + "'";
        if (!pFGlobal.db->sendTableQuery(query))
        {
            return;
        }

        std::string dEmail;
        std::string dPassword[4];
        std::string resetKey;
        sql::ResultSet* res = pFGlobal.db->GetCurrentResult();
        if (res->next())
        {
            dEmail = res->getString("email");
            dPassword[0] = res->getString("password_1");
            dPassword[1] = res->getString("password_2");
            dPassword[2] = res->getString("password_3");
            dPassword[3] = res->getString("password_4");
            resetKey = res->getString("reset_key");
        }

        for (int i = 0; i < 4; i++)
        {
            if (dPassword[i] != password[i])
            {
                WrongPWWarning().Text(L"*Wrong Password");
                return;
            }
        }

        Dlg1().Visibility(Visibility::Collapsed);
        Dlg2().Visibility(Visibility::Visible);
    }

    void MainWindow::OnBtnOTPValidation(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Dlg2().Visibility(Visibility::Collapsed);
        Dlg7().Visibility(Visibility::Visible);
        system("E:/PERSONAL/Freelance/Forex/live-binance-charts/src/finplot/dist/main.exe");
    }
    void MainWindow::OnBtnNewSignUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Dlg1().Visibility(Visibility::Collapsed);
        Dlg4().Visibility(Visibility::Visible);
    }
    void MainWindow::OnBtnResetKeyTwo(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Dlg2().Visibility(Visibility::Collapsed);
        Dlg3().Visibility(Visibility::Visible);
    }
    void MainWindow::OnBtnValidateAndReset(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::hstring temp = ResetKeyBox().Password();
        std::string resetkey, line;
        resetkey = winrt::to_string(temp);
        
        std::ifstream myfile;
        myfile.open("../resetkey.par");
        if (myfile.eof())
        {
            return;
        }
        getline(myfile, line);
        if (line != resetkey)
        {
            WrongRKWarning().Text(L"*Wrong ResetKey");
            return;
        }

        Dlg3().Visibility(Visibility::Collapsed);
        Dlg4().Visibility(Visibility::Visible);
    }

    void MainWindow::OnBtnValidateAndSetup1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::hstring temp;
        std::string mobileNumber, mail, resetKey;

        // Extract and validate mobile number
        temp = MobileBox().Text();
        if (temp.empty())
        {
            WrongMGWarning().Text(L"*Number Empty");
            return;
        }
        mobileNumber = winrt::to_string(temp);
        if (!isValidMobileNumber(mobileNumber))
        {
            WrongMGWarning().Text(L"*Wrong Number");
            return;
        }

        // Extract and validate mail
        temp = GmailBox().Text();
        if (temp.empty())
        {
            WrongMGWarning().Text(L"*Gmail is Empty");
            return;
        }

        mail = winrt::to_string(temp);
        if (!isValidMail(mail))
        {
            WrongMGWarning().Text(L"*Wrong  Gmail");
            return;
        }

        // Extract and validate reset key
        temp = ResetKeyBox2().Text();
        if (temp.empty())
        {
            WrongMGWarning().Text(L"Reset Key is empty");
            return;
        }
        resetKey = winrt::to_string(temp);

        if (pFGlobal.usr == nullptr)
        {
            pFGlobal.usr = std::make_unique<SUserProperty>();
        }
        else
        {
            pFGlobal.usr.reset(new SUserProperty());
        }
        pFGlobal.usr->mobileNo = mobileNumber;
        pFGlobal.usr->email = mail;
        pFGlobal.usr->resetKey = resetKey;
        //pFGlobal.otphandler.init(mail);
        //pFGlobal.otphandler.sendEmailWithOTP();

        Dlg4().Visibility(Visibility::Collapsed);
        Dlg5().Visibility(Visibility::Visible);
    }

    void MainWindow::OnBtnClickedPasswordSetup(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        winrt::hstring temp;
        std::string password[4];
        temp = Password1Box2().Password();
        password[0] = winrt::to_string(temp);
        temp = Password2Box2().Password();
        password[1] = winrt::to_string(temp);
        temp = Password3Box2().Password();
        password[2] = winrt::to_string(temp);
        temp = Password4Box2().Password();
        password[3] = winrt::to_string(temp);

        for (int i = 0; i < 4; i++)
        {
            if (password[i].empty())
            {
                WrongPW1Warning().Text(L"*Password should\n not be empty");
                return;
            }
        }
        
        if (pFGlobal.usr == nullptr)
        {
            pFGlobal.lgr->log(LogLevel::EXCEPTION, "user not created, not password copy!");
            exit(0);
        }

        for (int i = 0; i < 4; i++)
        {
            pFGlobal.usr->passwords[i] = password[i];
        }

        pFGlobal.otphandler.init(pFGlobal.usr->email);
        pFGlobal.usr->otp = pFGlobal.otphandler.sendEmailWithOTP();

        Dlg5().Visibility(Visibility::Collapsed);
        Dlg6().Visibility(Visibility::Visible);
    }

    void MainWindow::OnBtnOTPValidationAndSetup(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        std::string OTP = "";
        winrt::hstring temp;
        temp = OTP2KEY1().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY2().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);
        
        temp = OTP2KEY3().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY4().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY5().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY6().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY7().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        temp = OTP2KEY8().Text();
        if (temp.empty())
        {
            return;
        }
        OTP += winrt::to_string(temp);

        if (pFGlobal.usr->otp != OTP)
        {
            return;
        }

        std::string cmd =
            "INSERT INTO UserInfo "
            "(email, mobile_no, password_1, password_2, password_3, password_4, reset_key) "
            "VALUES ('" + pFGlobal.usr->email + "', '" + pFGlobal.usr->mobileNo + "', '" + pFGlobal.usr->passwords[0] + "', '" + pFGlobal.usr->passwords[1] + "', '" + pFGlobal.usr->passwords[2] + "', '" + pFGlobal.usr->passwords[3] + "', '" + pFGlobal.usr->resetKey + "')";
        
        BOOL insertStatus = pFGlobal.db->sendCmd(cmd);
        if (!insertStatus)
        {
            return;
        }

        Dlg6().Visibility(Visibility::Collapsed);
        Dlg1().Visibility(Visibility::Visible);
    }

    BOOL MainWindow::isValidMobileNumber(std::string mobileNumber)
    {
        std::regex regex("^\\+[0-9]{13}$");
        if (std::regex_match(mobileNumber, regex))
        {
            // Valid mobile number
            return TRUE;
        }
        else
        {
            // Invalid mobile number
            return FALSE;
        }
    }

    BOOL MainWindow::isValidMail(std::string gmailAddress)
    {
        std::regex regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        if (std::regex_match(gmailAddress, regex))
        {
            // Valid email address
            return TRUE;
        }
        else
        {
            // Invalid email address
            return FALSE;
        }
        return TRUE;
    }
}


void winrt::Forex::implementation::MainWindow::OnBtnResetKeyOne(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{

}






