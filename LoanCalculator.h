#ifndef LOANCALCULATOR_H_INCLUDED
#define LOANCALCULATOR_H_INCLUDED

#include <string>
#include <stdexcept>

/**
 * @class LoanCalculator
 * @brief A simple loan calculator for computing EMIs, interest rates, loan balances, and related calculations.
 * 
 * Supports calculation of:
 * - Loan balance after N payments
 * - Monthly payment
 * - Number of payments
 * - Original loan amount
 * - Interest rate (approximate)
 * - Effective interest rate (including fees)
 */
class LoanCalculator
{
public:
    /** 
     * @brief Default constructor 
     */
    LoanCalculator() {}

    /**
     * @brief Destructor
     */
    ~LoanCalculator() {}

    // --- Setters / Getters with validation ---

    /**
     * @brief Set the initial loan amount
     * @param A Loan amount (must be positive)
     * @throws std::invalid_argument if A <= 0
     */
    inline void setAmount(long double A) {
        if (A <= 0.0L) throw std::invalid_argument("Loan amount must be positive");
        amount_ = A; amountSet_ = true;
    }

    /**
     * @brief Get the loan amount
     * @return Current loan amount
     */
    inline long double getAmount() const { return amount_; }

    /**
     * @brief Set the initial down payment
     * @param initialA Initial payment (cannot be negative)
     * @throws std::invalid_argument if initialA < 0
     */
    inline void setInitialPayment(long double initialA) {
        if (initialA < 0.0L) throw std::invalid_argument("Initial payment cannot be negative");
        initialPayment_ = initialA;
    }

    /**
     * @brief Get the initial payment
     * @return Initial payment
     */
    inline long double getInitialPayment() const { return initialPayment_; }

    /**
     * @brief Set the yearly interest rate
     * @param i Interest rate in percent (must be positive)
     * @throws std::invalid_argument if i <= 0
     */
    inline void setInterest(long double i) {
        if (i <= 0.0L) throw std::invalid_argument("Interest rate must be positive");
        interest_ = i;
        interestPeriodic_ = (i / 100.0L) / 12.0L; ///< monthly periodic interest
        interestSet_ = true;
    }

    /**
     * @brief Get the yearly interest rate
     * @return Interest rate in percent
     */
    inline long double getInterest() const { return interest_; }

    /**
     * @brief Get the monthly interest rate
     * @return Monthly periodic interest
     */
    inline long double getPeriodicInterest() const { return interestPeriodic_; }

    /**
     * @brief Set the fixed monthly payment
     * @param P Payment amount (must be positive)
     * @throws std::invalid_argument if P <= 0
     */
    inline void setPayment(long double P) {
        if (P <= 0.0L) throw std::invalid_argument("Payment must be positive");
        payment_ = P; paymentSet_ = true;
    }

    /**
     * @brief Get the monthly payment
     * @return Payment amount
     */
    inline long double getPayment() const { return payment_; }

    /**
     * @brief Set the total loan period in months
     * @param N Total number of payments (must be > 0)
     * @throws std::invalid_argument if N == 0
     */
    inline void setPeriodTotal(unsigned long N) {
        if (N == 0) throw std::invalid_argument("Total period must be > 0");
        periodTotal_ = N; periodTotalSet_ = true;
    }

    /**
     * @brief Get the total loan period
     * @return Total number of payments
     */
    inline unsigned long getPeriodTotal() const { return periodTotal_; }

    /**
     * @brief Set the number of payments already made
     * @param n Number of elapsed payments
     */
    inline void setPeriodElapsed(unsigned long n) {
        periodElapsed_ = n; periodElapsedSet_ = true;
    }

    /**
     * @brief Get the number of payments already made
     * @return Number of elapsed payments
     */
    inline unsigned long getPeriodElapsed() const { return periodElapsed_; }

    /**
     * @brief Set the opening fee for the loan
     * @param fee Opening fee (cannot be negative)
     * @throws std::invalid_argument if fee < 0
     */
    inline void setOpeningFee(long double fee) {
        if (fee < 0.0L) throw std::invalid_argument("Opening fee cannot be negative");
        openingFee_ = fee;
    }

    /**
     * @brief Get the opening fee
     * @return Opening fee
     */
    inline long double getOpeningFee() const { return openingFee_; }

    /**
     * @brief Set the opening fee as a percentage of loan
     * @param percent Opening fee percent (cannot be negative)
     * @throws std::invalid_argument if percent < 0
     */
    inline void setOpeningPercent(long double percent) {
        if (percent < 0.0L) throw std::invalid_argument("Opening percent cannot be negative");
        openingPercent_ = percent;
    }

    /**
     * @brief Get the opening fee percent
     * @return Opening fee percentage
     */
    inline long double getOpeningPercent() const { return openingPercent_; }

    /**
     * @brief Reset all internal values to zero/defaults
     */
    inline void reset() {
        amount_ = initialPayment_ = interest_ = interestPeriodic_ = payment_ = openingFee_ = openingPercent_ = 0.0L;
        periodTotal_ = periodElapsed_ = 0;
        amountSet_ = interestSet_ = paymentSet_ = periodTotalSet_ = periodElapsedSet_ = false;
    }

    // --- Calculation functions ---

    /**
     * @brief Calculate loan balance after N payments
     * @return Remaining loan balance
     */
    long double calculateLoanBalance();

    /**
     * @brief Calculate the monthly payment
     * @return Monthly EMI payment
     */
    long double calculatePayment();

    /**
     * @brief Calculate the number of payments required to repay the loan
     * @return Number of payments
     */
    long double calculateNumberPayments();

    /**
     * @brief Calculate the original loan amount based on payment, interest, and period
     * @return Loan amount
     */
    long double calculateLoanAmount();

    /**
     * @brief Estimate the yearly interest rate
     * @return Yearly interest rate in percent
     */
    long double calculateInterestRate();

    /**
     * @brief Calculate effective interest rate including fees
     * @return Effective yearly interest rate in percent
     */
    long double calculateEffectiveInterestRate();

    /**
     * @brief Get a human-readable summary of the loan
     * @return Loan details as string
     */
    std::string toString();

private:
    long double amount_{0.0L};          ///< Loan amount
    bool amountSet_{false};             ///< Flag if amount is set

    long double initialPayment_{0.0L};  ///< Initial down payment

    long double interest_{0.0L};        ///< Yearly interest rate
    long double interestPeriodic_{0.0L};///< Monthly interest rate
    bool interestSet_{false};           ///< Flag if interest is set

    long double payment_{0.0L};         ///< Monthly payment
    bool paymentSet_{false};            ///< Flag if payment is set

    unsigned long periodTotal_{0};      ///< Total number of payments
    bool periodTotalSet_{false};        ///< Flag if period total is set

    unsigned long periodElapsed_{0};    ///< Payments already made
    bool periodElapsedSet_{false};      ///< Flag if period elapsed is set

    long double openingFee_{0.0L};      ///< Flat opening fee
    long double openingPercent_{0.0L};  ///< Opening fee percent
};

#endif
