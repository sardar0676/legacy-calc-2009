#include "LoanCalculator.h"
#include <cmath>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <string>

using namespace std;

// LoanCalculator::LoanCalculator()
//   : amountSet_(false), initialPayment_(0.0L),
//     interestSet_(false), paymentSet_(false),
//     periodTotalSet_(false), periodElapsedSet_(false),
//     openingFee_(0.0L), openingPercent_(0.0L)
// {
   
// }

/**
 * Loan balance after n payments have been made:
 *   B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)
 */
long double LoanCalculator::calculateLoanBalance()
{
    if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
        throw invalid_argument("Must set loan amount, interest, elapsed period, and payment for this calculation");

    long double A = amount_ - initialPayment_;
    long double i = interestPeriodic_;
    unsigned long n = periodElapsed_;

    if (fabsl(i) < 1e-20L)
        return A - (payment_ * (long double)n);

    long double pow_term = powl(1.0L + i, (long double)n);
    long double balance = (A * pow_term) - (payment_ / i) * (pow_term - 1.0L);
    return balance;
}

/**
 * Payment amount on a loan:
 *   P = i*A / (1 - (1+i)^-N)
 */
long double LoanCalculator::calculatePayment()
{
    if(!amountSet_ || !interestSet_ || !periodTotalSet_)
        throw invalid_argument("Must set loan amount, interest, and total period for this calculation");

    long double totalAmount = amount_ - initialPayment_;
    totalAmount += openingFee_ + (totalAmount * (openingPercent_/100.0L));

    long double i = interestPeriodic_;
    unsigned long N = periodTotal_;

    if (fabsl(i) < 1e-20L)
        return totalAmount / (long double)N;

    long double denom = 1.0L - powl(1.0L + i, -(long double)N);
    if (fabsl(denom) < 1e-20L)
        throw runtime_error("Denominator near zero in payment calculation");

    return (i * totalAmount) / denom;
}

/**
 * Number of payments on a loan:
 *   N = -log(1 - i*A/P) / log(1+i)
 */
long double LoanCalculator::calculateNumberPayments()
{
    if(!amountSet_ || !interestSet_ || !paymentSet_)
        throw invalid_argument("Must set loan amount, interest, and payment for this calculation");

    long double A = amount_ - initialPayment_;
    long double P = payment_;
    long double i = interestPeriodic_;

    if (P <= i * A)
        throw invalid_argument("Payment too small — will never amortize the loan (P <= i*A).");

    long double numerator = -logl(1.0L - (i * A / P));
    long double denominator = logl(1.0L + i);
    return numerator / denominator;
}

/**
 * Original loan amount:
 *   A = (P/i)*(1 - (1+i)^-N)
 */
long double LoanCalculator::calculateLoanAmount()
{
    if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
        throw invalid_argument("Must set payment, interest, and total period for this calculation");

    long double P = payment_;
    long double i = interestPeriodic_;
    unsigned long N = periodTotal_;

    if (fabsl(i) < 1e-20L)
        return P * (long double)N;

    return (P / i) * (1.0L - powl(1.0L + i, -(long double)N));
}

/**
 * Interest Rate (approximate):
 *   i = (((1 + P/A)^(1/q) - 1 )^q - 1)
 * Returns yearly percentage (e.g., 6.75)
 */
long double LoanCalculator::calculateInterestRate()
{
    if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
        throw invalid_argument("Must set amount, payment, and total period for this calculation");

    long double A = amount_ - initialPayment_;
    long double P = payment_;
    unsigned long N = periodTotal_;

    if (A <= 0.0L) throw invalid_argument("Actual loan amount must be positive");

    long double q = logl(1.0L + 1.0L/(long double)N) / logl(2.0L);
    long double base = 1.0L + P / A;
    if (base <= 0.0L) throw invalid_argument("Invalid base in interest estimation");

    long double monthlyInterest = powl(powl(base, 1.0L/q) - 1.0L, q) - 1.0L;
    return monthlyInterest * 12.0L * 100.0L;
}

/**
 * Effective Interest Rate including fees
 */
long double LoanCalculator::calculateEffectiveInterestRate()
{
    if(!amountSet_ || !periodTotalSet_)
        throw invalid_argument("Must set amount and total period for this calculation");

    long double payment = calculatePayment();
    long double totalAmount = amount_ - initialPayment_;
    unsigned long N = periodTotal_;

    long double q = logl(1.0L + 1.0L/(long double)N) / logl(2.0L);
    long double monthlyInterest = powl(powl(1.0L + payment/totalAmount, 1.0L/q) - 1.0L, q) - 1.0L;

    return monthlyInterest * 12.0L * 100.0L;
}

/**
 * Returns human-readable string
 */
std::string LoanCalculator::toString()
{
    stringstream ss;
    ss << fixed << setprecision(2);

    if(amountSet_)
        ss << "Initial Amount:      " << (double)amount_ << "\n";

    if(initialPayment_ != 0.0L)
        ss << "Initial Payment:     " << (double)initialPayment_ << "\n"
           << "Actual Loan Amount:  " << (double)(amount_ - initialPayment_) << "\n";

    if(interestSet_)
        ss << "Yearly Interest:     " << (double)interest_ << "%\n";

    if(paymentSet_)
        ss << "Monthly payment:     " << (double)payment_ << "\n";

    if(periodTotalSet_)
        ss << "Loan Period:         " << periodTotal_ << " months\n";

    if(periodElapsedSet_)
        ss << "Elapsed Period:      " << periodElapsed_ << " months\n";

    if(openingFee_ != 0.0L)
        ss << "Opening Fee:         " << (double)openingFee_ << "\n";

    if(openingPercent_ != 0.0L)
        ss << "Opening Fee %:       " << (double)openingPercent_ << "% = "
           << (double)((openingPercent_/100.0L)*(amount_ - initialPayment_)) << "\n";

    return ss.str();
}
