<<<<<<< HEAD
#include "LoanCalculator.h"
#include <cmath>
#include <sstream>
#include <iomanip>

using namespace std;

LoanCalculator::LoanCalculator()
{
  // members are default initialized in header
}

//
// The actual calculation methods (long double versions)
=======

#include <iostream>
#include <math.h>
#include <stdexcept>
#include <sstream>
#include <string>
#include <math.h>

#include "LoanCalculator.h"

using namespace std;

LoanCalculator::LoanCalculator() :
  amountSet_(false),
  initialPayment_(0.0),
  interestSet_(false),
  paymentSet_(false),
  periodTotalSet_(false),
  periodElapsedSet_(false),
  openingFee_(0.0),
  openingPercent_(0.0)
{
}

//
// The actual calculation methods
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
//

/**
 * Loan balance after n payments have been made:
 *   B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)
 */
<<<<<<< HEAD
long double LoanCalculator::calculateLoanBalance()
{
  if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, elapsed period, and payment for this calculation");
  }

  long double A = amount_ - initialPayment_;
  long double i = interestPeriodic_;
  unsigned long n = periodElapsed_;

  // If interestPeriodic_ is extremely small, dividing by i could be dangerous.
  if (fabsl(i) < 1e-20L) {
    // approximate as simple reduction: balance = A - P*n
    return A - (payment_ * (long double)n);
  }

  long double pow_term = powl(1.0L + i, (long double)n);
  long double balance = (A * pow_term) - (payment_ / i) * (pow_term - 1.0L);
  return balance;
=======
float LoanCalculator::calculateLoanBalance()
{
  if(!amountSet_ || !interestSet_ || !periodElapsedSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and elapsed period for this calculation" );
  }

  return (amount_*pow((1+interestPeriodic_), periodElapsed_)) -
         (payment_/interestPeriodic_)*(pow((1+interestPeriodic_), periodElapsed_)-1);
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
}

/**
 * Payment amount on a loan:
 *   P = i*A / (1 - (1+i)^-N)
 */
<<<<<<< HEAD
long double LoanCalculator::calculatePayment()
{
  if(!amountSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and total period for this calculation");
  }

  long double totalAmount = amount_ - initialPayment_;
  totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0L));

  long double i = interestPeriodic_;
  unsigned long N = periodTotal_;

  if (fabsl(i) < 1e-20L) {
    // zero (or near-zero) interest: payment is simple division
    return totalAmount / (long double)N;
  }

  long double denom = 1.0L - powl((1.0L + i), - (long double)N);
  if (fabsl(denom) < 1e-20L) {
    throw runtime_error("Denominator near zero in payment calculation (possible precision issue).");
  }

  long double payment = (i * totalAmount) / denom;
  return payment;
=======
float LoanCalculator::calculatePayment()
{
  if(!amountSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and total period for this calculation" );
  }

  float totalAmount = amount_ - initialPayment_;
  totalAmount = totalAmount + openingFee_ + (totalAmount * (openingPercent_/100.0));

  return (interestPeriodic_*totalAmount) /
         (1 - pow((1+interestPeriodic_), (-1*periodTotal_)));
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
}

/**
 * Number of payments on a loan:
<<<<<<< HEAD
 *   N = -log(1 - i*A/P) / log(1+i)
 */
long double LoanCalculator::calculateNumberPayments()
{
  if(!amountSet_ || !interestSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and payment for this calculation");
  }

  long double A = amount_ - initialPayment_;
  long double P = payment_;
  long double i = interestPeriodic_;

  if (P <= i * A) {
    throw invalid_argument("Payment too small — will never amortize the loan (P <= i*A).");
  }

  long double numerator = -logl(1.0L - (i * A / P));
  long double denominator = logl(1.0L + i);
  long double N = numerator / denominator;
  return N;
=======
 *   N = -log(1-i*A/P) / log(1+i)
 *      (You can use any logarithm base, as long as both logs use the same base.)
 *      Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
 *      If you pay her back $100 a month, how long will it take?
 *      Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
 */
float LoanCalculator::calculateNumberPayments()
{
  if(!amountSet_ || !interestSet_ || !paymentSet_)
  {
    throw invalid_argument("Must set loan amount, interest, and payment for this calculation" );
  }

  return (-1.0*log10(1.0-(interestPeriodic_*amount_/payment_))) /
         log10(1.0 + interestPeriodic_);
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
}

/**
 * Original loan amount:
 *   A = (P/i)*(1 - (1+i)^-N)
 */
<<<<<<< HEAD
long double LoanCalculator::calculateLoanAmount()
{
  if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set payment, interest, and total period for this calculation");
  }

  long double P = payment_;
  long double i = interestPeriodic_;
  unsigned long N = periodTotal_;

  if (fabsl(i) < 1e-20L) {
    // near-zero interest
    return P * (long double)N;
  }

  long double amount = (P / i) * (1.0L - powl((1.0L + i), - (long double)N));
  return amount;
=======
float LoanCalculator::calculateLoanAmount()
{
  if(!paymentSet_ || !interestSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set payment, interest, and total period for this calculation" );
  }

  return (payment_/interestPeriodic_) *
         (1 - pow((1+interestPeriodic_), (-1*periodTotal_)));
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
}

/**
 * Interest Rate:
 *   i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
 *   where q = log(1+1/N) / log(2)
<<<<<<< HEAD
 *
 * returns yearly percentage (e.g., 6.75)
 */
long double LoanCalculator::calculateInterestRate()
{
  if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount, payment, and total period for this calculation");
  }

  long double A = amount_ - initialPayment_;
  long double P = payment_;
  unsigned long N = periodTotal_;

  if (A <= 0.0L) throw invalid_argument("Actual loan amount (A - initial payment) must be positive");

  long double q = logl(1.0L + 1.0L/(long double)N) / logl(2.0L);
  long double base = 1.0L + P / A;
  if (base <= 0.0L) throw invalid_argument("Invalid base in interest estimation.");
  long double monthlyInterest = powl((powl(base, 1.0L/q) -1.0L), q) -1.0L;

  return monthlyInterest * 12.0L * 100.0L;
}

long double LoanCalculator::calculateEffectiveInterestRate()
{
  if(!amountSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount and total period for this calculation");
  }

  long double payment = calculatePayment();
  long double totalAmount = amount_ - initialPayment_;

  long double N = (long double) periodTotal_;
  long double q = logl(1.0L + 1.0L/N) / logl(2.0L);
  long double monthlyInterest = powl((powl((1.0L + payment/totalAmount), 1.0L/q) -1.0L), q) -1.0L;

  return monthlyInterest * 12.0L * 100.0L;
=======
*/
float LoanCalculator::calculateInterestRate()
{
  if(!amountSet_ || !paymentSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount, payment, and total period for this calculation" );
  }

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = pow((pow((1.0 + payment_/amount_), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
}

float LoanCalculator::calculateEffectiveInterestRate()
{
  if(!amountSet_ || !periodTotalSet_)
  {
    throw invalid_argument("Must set amount and total period for this calculation" );
  }

  float payment = calculatePayment();
  float totalAmount = amount_ - initialPayment_;

  float q = log10(1.0 + 1.0/periodTotal_) / log10(2.0);
  float monthlyInterest = pow((pow((1.0 + payment/totalAmount), 1.0/q) -1.0), q) -1.0;

  return monthlyInterest*12*100;
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
}

std::string LoanCalculator::toString()
{
<<<<<<< HEAD
  std::stringstream ss;
  ss << std::fixed << std::setprecision(2);

  if(amountSet_)
  {
    ss << "Initial Amount:      " << (double)amount_ << "\n";
  }

  if(initialPayment_ != 0.0L)
  {
    ss << "Initial Payment:     " << (double)initialPayment_ << "\n";
    ss << "Actual Loan Amount:  " << (double)(amount_ - initialPayment_) << "\n";
=======
  stringstream ss;

  //ss << "LoanCalculator set values:\n";

  if(amountSet_)
  {
    ss << "Initial Amount:      " << amount_ << "\n";
  }

  if(initialPayment_ != 0.0)
  {
    ss << "Initial Payment:     " << initialPayment_ << "\n";
    ss << "Actual Loan Amount:  " << (amount_ - initialPayment_) << "\n";
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
  }

  if(interestSet_)
  {
<<<<<<< HEAD
    ss << "Yearly Interest:     " << (double)interest_ << "%\n";
=======
    ss << "Yearly Interest:     " << interest_ << "%\n";
    //ss << "Yearly Interest:     " << interest_
    //   << "\nMonthly Interest:    " << interestPeriodic_ << "\n";
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
  }

  if(paymentSet_)
  {
<<<<<<< HEAD
    ss << "Monthly payment:     " << (double)payment_ << "\n";
=======
    ss << "Monthly payment:     " << payment_ << "\n";
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
  }

  if(periodTotalSet_)
  {
    ss << "Loan Period:         " << periodTotal_ << " months\n";
  }

  if(periodElapsedSet_)
  {
    ss << "Elapsed Period:      " << periodElapsed_ << " months\n";
  }

<<<<<<< HEAD
  if(openingFee_ != 0.0L)
  {
    ss << "Opening Fee:         " << (double)openingFee_ << "\n";
  }

  if(openingPercent_ != 0.0L)
  {
    ss << "Opening Fee %:       " << (double)openingPercent_ << "% = "
       << (double)(openingPercent_/100.0L*(amount_ - initialPayment_)) << "\n";
=======
  if(openingFee_ != 0.0)
  {
      ss << "Opening Fee:       " << openingFee_ << "\n";
  }

  if(openingPercent_ != 0.0)
  {
    ss << "Opening Fee %:       " << openingPercent_ << "% = "
       << openingPercent_/100*(amount_ - initialPayment_) << "\n";
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
  }

  return ss.str();
}
