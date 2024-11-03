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
        winrt::hstring temp = Password1Box().Password();
        std::string password[4];
        password[0] = winrt::to_string(temp);   
        temp = Password2Box().Password();
        password[1] = winrt::to_string(temp);
        temp = Password3Box().Password();
        password[2] = winrt::to_string(temp);
        temp = Password4Box().Password();
        password[3] = winrt::to_string(temp);

        std::ifstream myfile;
        myfile.open("E:/PERSONAL/Freelance/Forex_Final/Forex/usr.par");
        std::string line;
        isPasswordValid = TRUE;
        for (int i = 0; i < 4; i++)
        {
            if (myfile.eof())
            {
                isPasswordValid = FALSE;
                break;
            }
            getline(myfile, line);
            if (line != password[i])
            {
                isPasswordValid = FALSE;
                break;
            }
        }

        if (!isPasswordValid)
        {
            WrongPWWarning().Text(L"*Wrong Password");
            return;
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
    void MainWindow::OnBtnResetKeyOne(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
    {
        Dlg1().Visibility(Visibility::Collapsed);
        Dlg3().Visibility(Visibility::Visible);
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
        winrt::hstring temp = MobileBox().Text();
        std::string mobileNumber, gmail;

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

        temp = GmailBox().Text();
        if (temp.empty())
        {
            WrongMGWarning().Text(L"*Gmail is Empty");
            return;
        }

        gmail = winrt::to_string(temp);
        if (!isValidGmail(gmail))
        {
            WrongMGWarning().Text(L"*Wrong  Gmail");
            return;
        }

        Dlg4().Visibility(Visibility::Collapsed);
        Dlg5().Visibility(Visibility::Visible);
    }

    BOOL MainWindow::isValidMobileNumber(std::string mobileNumber)
    {
        std::regex regex("^[0-9]{13}$");
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

    BOOL MainWindow::isValidGmail(std::string gmailAddress)
    {
        //std::regex regex("^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$");
        //if (std::regex_match(gmailAddress, regex))
        //{
        //    // Valid email address
        //    return TRUE;
        //}
        //else
        //{
        //    // Invalid email address
        //    return FALSE;
        //}
        return TRUE;
    }
}












