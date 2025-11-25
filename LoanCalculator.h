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

class LoanCalculator
{
public:
  LoanCalculator();
  ~LoanCalculator() {}

  //
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
    periodTotal_ = periodElapsed_ = 0;
    amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
  }

  //
  // Calculation methods (now return long double)
  //
  long double calculateLoanBalance();
  long double calculatePayment();
  long double calculateNumberPayments();
  long double calculateLoanAmount();
  long double calculateInterestRate();
  long double calculateEffectiveInterestRate();

  std::string toString();

private:
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
