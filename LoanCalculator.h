<<<<<<< HEAD
#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

/*
Refactored LoanCalculator:
 - switched numeric types to long double for safer math / larger range
 - added input validation in setters
 - kept public API names but improved types internally
*/

#include <string>
#include <stdexcept>
=======
#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

/*
Formulas from: http://oakroadsystems.com/math/loan.htm

Loan balance after n payments have been made:
  B_n = A*(1+i)^n - (P/i)*((1+i)^n - 1)


Payment amount on a loan:
  P = i*A / (1 - (1+i)^-N)


Number of payments on a loan: (You can use any logarithm base, as long as both logs use the same base.)
                              Aunt Sally offers to lend you $3500 at 6% for that new home theater system you want.
                              If you pay her back $100 a month, how long will it take?
                              Solution:  6% per year is 0.5% per month, or 0.005. P = 100 and A = 3500. N = 38.57
  N = -log(1-i*A/P) / log(1+i)


Original loan amount:
  A = (P/i)*(1 - (1+i)^-N)


Interest Rate:
  i = (((1 + P/A)^(1/q) - 1 )^q - 1)  NOTICE: This is an approximate not an exact solution
  where q = log(1+1/N) / log(2)


Variables:
A   	the loan amount (the principal sum) or initial investment
B_n or Bn   	(pronounced B sub n) the balance after n payments have been made. After the last payment has been made, B_N is zero.)
i   	the interest rate per period, not per year
      (For instance, if the loan payments are made monthly and the interest rate is 9%, then i = 9%/12 = 0.75% = 0.0075.)
n   	the number of time periods elapsed at any given point
N   	the total number of payments for the entire loan or investment
P   	the amount of each equal payment
*/

#include <string>
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2

class LoanCalculator
{
public:
  LoanCalculator();
  ~LoanCalculator() {}

  //
<<<<<<< HEAD
  // Setters and Getters (now validate inputs)
  //

  /** Total loan amount A */
  inline void setAmount(long double A) {
    if (A <= 0.0L) throw std::invalid_argument("Loan amount must be positive");
    amount_ = A; amountSet_ = true;
  }
  inline long double getAmount() const { return amount_; }
  inline bool isAmountSet() const { return amountSet_; }

  /** Initial down payment */
  inline void setInitialPayment(long double initialA)  {
    if (initialA < 0.0L) throw std::invalid_argument("Initial payment cannot be negative");
    initialPayment_ = initialA;
  }
  inline long double getInitialPayment() const { return initialPayment_; }

  /**
   * Yearly interest rate i as e.g. 6.75 (percent)
   * Internally interestPeriodic_ = (i / 100) / 12
   */
  inline void setInterest(long double i) {
    if (i <= 0.0L) throw std::invalid_argument("Interest rate must be positive");
    interest_ = i;
    interestPeriodic_ = (i / 100.0L) / 12.0L;
    interestSet_ = true;
  }
  inline long double getInterest() const         { return interest_; }
  inline long double getPeriodicInterest() const { return interestPeriodic_; }
  inline bool isInterestSet() const { return interestSet_; }

  inline void setPayment(long double P) {
    if (P <= 0.0L) throw std::invalid_argument("Payment must be positive");
    payment_ = P; paymentSet_ = true;
  }
  inline long double getPayment() const { return payment_; }
  inline bool isPaymentSet() const { return paymentSet_; }

  inline void setPeriodTotal(unsigned long N) {
    if (N == 0) throw std::invalid_argument("Total period (months) must be > 0");
    periodTotal_ = N; periodTotalSet_ = true;
  }
  inline unsigned long getPeriodTotal() const { return periodTotal_; }
  inline bool isPeriodTotalSet() const { return periodTotalSet_; }

  inline void setPeriodElapsed(unsigned long n) {
    // elapsed 0 is allowed (just before any payment)
    periodElapsed_ = n; periodElapsedSet_ = true;
  }
  inline unsigned long getPeriodElapsed() const  { return periodElapsed_; }
  inline bool isPeriodElapsedSet() const { return periodElapsedSet_; }

  inline void setOpeningFee(long double fee) {
    if (fee < 0.0L) throw std::invalid_argument("Opening fee cannot be negative");
    openingFee_ = fee;
  }
  inline long double getOpeningFee() const   { return openingFee_; }

  inline void setOpeningPercent(long double percent) {
    if (percent < 0.0L) throw std::invalid_argument("Opening percent cannot be negative");
    openingPercent_ = percent;
  }
  inline long double getOpeningPercent() const       { return openingPercent_; }

  inline void reset() {
    amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0L;
=======
  // Setters and Getters
  //

  /**
   * Total loan amount A
   */
  inline void setAmount(float A) { amount_ = A; amountSet_ = true; }
  inline float getAmount() const { return amount_; }

  /**
   * Initial down payment
   */
  inline void setInitialPayment(float initialA)  { initialPayment_ = initialA; }
  inline float getInitialPayment() const         { return initialPayment_; }

  /**
   * Yearly interest rate i as in 6.75
   * Internally .0675/12 will be used
   * If 6.75 is passed to setInterest()
   *    getInterest() will return 6.75
   *    getPeriodicInterest() will return .0675/12.0
   */
  void setInterest(float i) { interest_ = i; interestPeriodic_ = i/100.0/12.0; interestSet_ = true; }
  inline float getInterest() const         { return interest_; }
  inline float getPeriodicInterest() const { return interestPeriodic_; }

  void setPayment(float P)        { payment_ = P; paymentSet_ = true; }
  inline float getPayment() const { return payment_; }

  void setPeriodTotal(int N)        { periodTotal_ = N; periodTotalSet_ = true; }
  inline int getPeriodTotal() const { return periodTotal_; }

  void setPeriodElapsed(int n)         { periodElapsed_ = n; periodElapsedSet_ = true; }
  inline int getPeriodElapsed() const  { return periodElapsed_; }

  inline void setOpeningFee(float fee) { openingFee_ = fee; }
  inline float getOpeningFee() const   { return openingFee_; }

  inline void setOpeningPercent(float percent) { openingPercent_ = percent; }
  inline float getOpeningPercent() const       { return openingPercent_; }

  inline void reset() {
    amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0;
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
    periodTotal_ = periodElapsed_ = 0;
    amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
  }

  //
<<<<<<< HEAD
  // Calculation methods (now return long double)
  //
  long double calculateLoanBalance();
  long double calculatePayment();
  long double calculateNumberPayments();
  long double calculateLoanAmount();
  long double calculateInterestRate();
  long double calculateEffectiveInterestRate();
=======
  // The actual calculation methods
  //

  float calculateLoanBalance();
  float calculatePayment();
  float calculateNumberPayments();
  float calculateLoanAmount();
  float calculateInterestRate();
  // The effective interest rate, once fees have been applied
  float calculateEffectiveInterestRate();
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2

  std::string toString();

private:
<<<<<<< HEAD
  long double amount_{0.0L};        // loan amount
  bool amountSet_{false};

  long double initialPayment_{0.0L};     // initial down payment

  long double interest_{0.0L};          // yearly percent (e.g., 6.75)
  long double interestPeriodic_{0.0L};  // monthly rate decimal (.0675/12)
  bool interestSet_{false};

  long double payment_{0.0L};       // payment amount
  bool paymentSet_{false};

  unsigned long periodTotal_{0};     // total payment periods (months)
  bool periodTotalSet_{false};

  unsigned long periodElapsed_{0};   // number of elapsed payment periods
  bool periodElapsedSet_{false};

  // opening fee and percentage
  long double openingFee_{0.0L};
  long double openingPercent_{0.0L};
};

#endif // LOANCALCULATOR_H_INCLUDED
=======
  float amount_;        // loan amount
  bool amountSet_;

  float initialPayment_;     // initial down payment

  float interest_;          // interest rate, something like 6.75
  float interestPeriodic_;  // this will be .0675/12
  bool interestSet_;

  float payment_;       // payment amount
  bool paymentSet_;

  int periodTotal_;     // total payment periods
  bool periodTotalSet_;

  int periodElapsed_;   // number of elapsed payment periods
  bool periodElapsedSet_;

  // These two are used if loans charge a fee opening fee or percentage
  float openingFee_;
  float openingPercent_;

};

#endif // LOANCALCULATOR_H_INCLUDED
>>>>>>> d7f836415e8c4284798991218502bf6c7a2c9ec2
