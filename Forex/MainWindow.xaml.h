// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#pragma once

#include "MainWindow.g.h"
#include "Logger.h"
#include <iostream>

namespace winrt::Forex::implementation
{
    struct MainWindow : MainWindowT<MainWindow>
    {
    private:
        BOOL isPasswordValid;
  
        BOOL isValidMobileNumber(std::string mobileNumber);
        BOOL isValidGmail(std::string gmailAddress);

    public:
        MainWindow();

        int32_t MyProperty();
        void MyProperty(int32_t value);

        void OnBtnPasswordValidation(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnOTPValidation(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnNewSignUp(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnResetKeyTwo(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnValidateAndReset(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnValidateAndSetup1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
        void OnBtnResetKeyOne(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e);
    };
}

namespace winrt::Forex::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
