#include <stdlib.h>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <fstream>

#include <QApplication>

#include <LoanCalcQtMainWindow.h>
#include <CmdLineParser.h>
#include <LoanCalculator.h>

using namespace std;

enum CALC_TYPE
{
    CALC_UNKNOWN=0,
    CALC_BALANCE=100,
    CALC_PAYMENT,
    CALC_NUMPAYMENTS,
    CALC_AMOUNT,
    CALC_INTEREST
};

const string ARG_CALC_BALANCE      = "-cb";
const string ARG_CALC_PAYMENT      = "-cp";
const string ARG_CALC_NUMPAYMENTS  = "-cn";
const string ARG_CALC_AMOUNT       = "-ca";
const string ARG_CALC_INTEREST     = "-ci";

const string ARG_PAYMENT           = "-p";
const string ARG_PERIOD_TOTAL      = "-N";
const string ARG_PERIOD_ELAPSED    = "-n";
const string ARG_AMOUNT            = "-a";
const string ARG_INITIAL_PAYMENT   = "-ai";
const string ARG_INTEREST          = "-i";
const string ARG_OPENFEE           = "-of";
const string ARG_OPENPERCENT       = "-op";

// Simple config reader
map<string,string> readConfig(const string &filename) {
    map<string,string> cfg;
    ifstream file(filename);
    if(!file.is_open()) return cfg;
    string line;
    while(getline(file,line)) {
        size_t eq = line.find('=');
        if(eq != string::npos) {
            string key = line.substr(0,eq);
            string value = line.substr(eq+1);
            cfg[key] = value;
        }
    }
    return cfg;
}

void loadCmdLine(CmdLineParser &clp)
{
    clp.setMainHelpText("A simple loan calculator");
    clp.setMainHelpTextEnd("With no options set, a GUI will be launched");

    // Calculation types
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_BALANCE,
         "Calculate the loan balance after several payments",
         false, CALC_BALANCE));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_PAYMENT,
         "Calculate the monthly loan payment",
         false, CALC_PAYMENT));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_NUMPAYMENTS,
         "Calculate the number of payments needed to pay a loan",
         false, CALC_NUMPAYMENTS));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_AMOUNT,
         "Calculate the initial loan amount",
         false, CALC_AMOUNT));
    clp.addMutExclCmdLineOption(new CmdLineOptionFlag(ARG_CALC_INTEREST,
         "Calculate the loan interest",
         false, CALC_INTEREST));
    clp.setMutExclUsageText("Calculations");

    // Different values
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_PAYMENT, "Monthly loan payment. Ej: 325.67"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_PERIOD_TOTAL, "Total loan period in months. Ej: 60"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_PERIOD_ELAPSED, "Elapsed period in months. Ej: 32"));
    clp.addCmdLineOption(new CmdLineOptionInt(ARG_AMOUNT, "Initial amount. Ej: 19300"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_INITIAL_PAYMENT,
         "Initial payment. Ej: 1000, default 0.0"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_INTEREST, "Yearly interest rate. Ej: 6.75"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_OPENFEE, "Opening fees. Ej: 100, default 0.0"));
    clp.addCmdLineOption(new CmdLineOptionFloat(ARG_OPENPERCENT, "Opening fees percent. Ej: 2.75, default 0.0"));

    clp.setMinNumberArgs(3);
}

CALC_TYPE parseCommandLine(int argc, char **argv, CmdLineParser &clp, LoanCalculator &calculator)
{
    CALC_TYPE ct(CALC_UNKNOWN);

    if(!clp.parseCmdLine(argc, argv)) {
        clp.printUsage();
        return ct;
    }

    // Set calculator values from command-line options
    calculator.setAmount(((CmdLineOptionInt*)clp.getCmdLineOption(ARG_AMOUNT))->getValue());
    calculator.setInitialPayment(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_INITIAL_PAYMENT))->getValue());
    calculator.setInterest(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_INTEREST))->getValue());
    calculator.setPayment(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_PAYMENT))->getValue());
    calculator.setPeriodTotal((unsigned long)((CmdLineOptionInt*)clp.getCmdLineOption(ARG_PERIOD_TOTAL))->getValue());
    calculator.setPeriodElapsed((unsigned long)((CmdLineOptionInt*)clp.getCmdLineOption(ARG_PERIOD_ELAPSED))->getValue());
    calculator.setOpeningFee(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_OPENFEE))->getValue());
    calculator.setOpeningPercent(((CmdLineOptionFloat*)clp.getCmdLineOption(ARG_OPENPERCENT))->getValue());

    CmdLineOption *option(clp.getMutExclOption());
    if(option != nullptr) {
        ct = (CALC_TYPE)((CmdLineOptionFlag*)option)->getValueKey();
    }

    return ct;
}

int main(int argc, char **argv)
{
    LoanCalculator calculator;

    // Load default config
    auto cfg = readConfig("config.txt");
    try {
        if(cfg.count("default_amount")) calculator.setAmount(stold(cfg["default_amount"]));
        if(cfg.count("default_interest")) calculator.setInterest(stold(cfg["default_interest"]));
        if(cfg.count("default_period_months")) calculator.setPeriodTotal(stoul(cfg["default_period_months"]));
        if(cfg.count("default_initial_payment")) calculator.setInitialPayment(stold(cfg["default_initial_payment"]));
        if(cfg.count("default_opening_fee")) calculator.setOpeningFee(stold(cfg["default_opening_fee"]));
        if(cfg.count("default_opening_percent")) calculator.setOpeningPercent(stold(cfg["default_opening_percent"]));
    } catch(const exception &e) {
        cerr << "Error loading config: " << e.what() << "\n";
    }

    // Launch GUI if no arguments
    if(argc == 1) {
        QApplication app(argc, argv);
        LoanCalcQtMainWindow mainWindow(&calculator);
        mainWindow.show();
        return app.exec();
    }

    CmdLineParser clp;
    loadCmdLine(clp);
    CALC_TYPE ct = parseCommandLine(argc, argv, clp, calculator);

    try {
        cout << endl;
        switch(ct) {
            case CALC_BALANCE:
                cout << "Loan Balance = " << (float)calculator.calculateLoanBalance() << endl;
                break;
            case CALC_PAYMENT: {
                float payment = calculator.calculatePayment();
                cout << "Monthly Payment = " << payment
                     << "\nTotal paid = " << (float)(payment*calculator.getPeriodTotal()) << endl;
                if(calculator.getOpeningPercent() != 0.0 || calculator.getOpeningFee() != 0.0)
                    cout << "Effective interest with fees = " << (float)calculator.calculateEffectiveInterestRate() << "%" << endl;
                break;
            }
            case CALC_NUMPAYMENTS:
                cout << "Number of payments = " << (float)calculator.calculateNumberPayments() << endl;
                break;
            case CALC_AMOUNT:
                cout << "Initial Loan amount = " << (float)calculator.calculateLoanAmount() << endl;
                break;
            case CALC_INTEREST:
                cout << "Yearly Interest Rate = " << (float)calculator.calculateInterestRate() << "%" << endl;
                break;
            default:
                return 1;
        }

        cout << calculator.toString() << endl;
    } catch(const exception &e) {
        cerr << "Error executing loan calculator: " << e.what() << endl;
    }

    return 0;
}
