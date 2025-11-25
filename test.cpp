#include <gtest/gtest.h>
#include "LoanCalculator.h"
#include <cmath>


// ---------------------------
// 1. NORMAL EMI CALCULATION
// ---------------------------
TEST(LoanCalculatorTest, NormalEmiCalculation)
{
    LoanCalculator calc;
    calc.setAmount(100000);          // Loan amount
    calc.setInterest(12);            // 12% yearly → 1% monthly
    calc.setPeriodTotal(12);         // 12 months

    float payment = calc.calculatePayment();

    // Manually computed EMI (verified using independent EMI calculator)
    float expectedEmi = 8884.0f;

    EXPECT_NEAR(payment, expectedEmi, 1.0f);  
}



// ---------------------------
// 2. INVALID INPUT HANDLING
// ---------------------------
TEST(LoanCalculatorTest, InvalidInputThrowsException)
{
    LoanCalculator calc;

    // Invalid negative amount -> should throw
    EXPECT_THROW({
        calc.setAmount(-5000);
    }, std::invalid_argument);

    // Missing interest -> should throw when calculating payment
    calc.reset();
    calc.setAmount(20000);
    calc.setPeriodTotal(24);

    EXPECT_THROW({
        calc.calculatePayment();
    }, std::invalid_argument);
}



// --------------------------------------
// 3. LARGE TENURE WITHOUT OVERFLOW
// --------------------------------------
TEST(LoanCalculatorTest, LargeTenureDoesNotOverflow)
{
    LoanCalculator calc;

    calc.setAmount(500000);         // Loan amount
    calc.setInterest(7.5f);         // 7.5% yearly
    calc.setPeriodTotal(1200);      // 100 YEARS → tests overflow

    // If overflow occurs, calculation crashes or returns NaN/Inf.
    float emi = calc.calculatePayment();

    EXPECT_TRUE(std::isfinite(emi));       // NOT inf / NaN
    EXPECT_GT(emi, 0.0);                   // EMI must be positive
}
