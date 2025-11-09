#include <bits/stdc++.h>
using namespace std;

struct Applicant {
    string name;
    int age;
    string employmentStatus; // "Salaried" / "Self-Employed" / "Unemployed"
    double monthlyIncome;
    int creditScore; // 300 - 900 typical scale
    double existingMonthlyEMI; // total current EMI obligations per month
    double requestedLoanAmount;
    int tenureMonths;
};

struct Decision {
    bool eligible = false;
    double monthlyEMI = 0.0;
    vector<string> reasons;
    double interestRateAnnual = 0.0;
};

bool isPositiveDouble(const string &s) {
    try {
        size_t idx;
        double v = stod(s, &idx);
        return idx == s.size() && v >= 0.0;
    } catch (...) { return false; }
}

double calculateEMI(double principal, double annualRatePercent, int months) {
    if (months <= 0) return 0.0;
    double monthlyRate = annualRatePercent / 12.0 / 100.0;
    if (monthlyRate <= 0.0) return principal / months;
    double factor = pow(1.0 + monthlyRate, months);
    double emi = principal * monthlyRate * factor / (factor - 1.0);
    return emi;
}

double determineInterestRate(int creditScore, const string &employmentStatus) {
    // Simplified interest rate bands (annual %)
    if (creditScore >= 750) return (employmentStatus == "Salaried") ? 9.0 : 10.0;
    if (creditScore >= 700) return (employmentStatus == "Salaried") ? 10.5 : 11.5;
    if (creditScore >= 650) return (employmentStatus == "Salaried") ? 12.0 : 13.0;
    if (creditScore >= 600) return (employmentStatus == "Salaried") ? 14.0 : 15.0;
    return 18.0; // high risk
}

Decision evaluateApplicant(const Applicant &a) {
    Decision d;

    // Bank policy parameters (configurable)
    const int minAge = 21;
    const int maxAge = 62;
    const double minMonthlyIncome = 10000.0; // minimal threshold
    const int minCreditScore = 600;
    const double maxDTI = 0.50; // Debt-to-Income ratio after proposed EMI
    const double loanToAnnualIncomeMultiplier = 5.0; // loan <= 5 * annual income

    // Basic checks
    if (a.age < minAge) d.reasons.push_back("Applicant below minimum age (" + to_string(minAge) + ").");
    if (a.age > maxAge) d.reasons.push_back("Applicant above maximum age (" + to_string(maxAge) + ").");
    if (a.employmentStatus == "Unemployed") d.reasons.push_back("Applicant is unemployed.");
    if (a.monthlyIncome < minMonthlyIncome) d.reasons.push_back("Monthly income below minimum required ("
                                                              + to_string((int)minMonthlyIncome) + ").");
    if (a.creditScore < minCreditScore) d.reasons.push_back("Credit score (" + to_string(a.creditScore) +
                                                            ") below minimum (" + to_string(minCreditScore) + ").");

    // Interest rate decision
    d.interestRateAnnual = determineInterestRate(a.creditScore, a.employmentStatus);

    // EMI calculation
    d.monthlyEMI = calculateEMI(a.requestedLoanAmount, d.interestRateAnnual, a.tenureMonths);

    // DTI check
    double newTotalEMI = a.existingMonthlyEMI + d.monthlyEMI;
    double dti = (a.monthlyIncome > 0.0) ? (newTotalEMI / a.monthlyIncome) : 1.0;

    if (dti > maxDTI) {
        ostringstream oss;
        oss << fixed << setprecision(2);
        oss << "Debt-to-Income ratio would be " << dti << " (> " << maxDTI << ").";
        d.reasons.push_back(oss.str());
    }

    // Loan amount vs annual income check
    double maxAllowedLoan = loanToAnnualIncomeMultiplier * (a.monthlyIncome * 12.0);
    if (a.requestedLoanAmount > maxAllowedLoan) {
        ostringstream oss;
        oss << fixed << setprecision(0);
        oss << "Requested loan exceeds allowed limit based on income (max allowed: " << maxAllowedLoan << ").";
        d.reasons.push_back(oss.str());
    }

    // Final decision
    d.eligible = d.reasons.empty();
    return d;
}

void printDecisionReport(const Applicant &a, const Decision &d) {
    cout << "\n========== Loan Eligibility Report ==========\n";
    cout << "Applicant Name  : " << a.name << "\n";
    cout << "Age             : " << a.age << "\n";
    cout << "Employment      : " << a.employmentStatus << "\n";
    cout << "Monthly Income  : " << fixed << setprecision(2) << a.monthlyIncome << "\n";
    cout << "Credit Score    : " << a.creditScore << "\n";
    cout << "Existing EMIs   : " << a.existingMonthlyEMI << "\n";
    cout << "Requested Loan  : " << a.requestedLoanAmount << "\n";
    cout << "Tenure (months) : " << a.tenureMonths << "\n";
    cout << "Estimated Rate  : " << d.interestRateAnnual << "% p.a.\n";
    cout << "Estimated EMI    : " << fixed << setprecision(2) << d.monthlyEMI << "\n";
    if (d.eligible) {
        cout << "\nDecision: APPROVED ✅\n";
        cout << "Congratulations! Your loan request meets the bank's simplified criteria.\n";
    } else {
        cout << "\nDecision: REJECTED ❌\n";
        cout << "Reasons:\n";
        for (const auto &r : d.reasons) {
            cout << " - " << r << "\n";
        }
    }
    cout << "==============================================\n\n";
}

int main() {
    cout << "===== Bank Loan Eligibility Checker =====\n";
    Applicant a;
    cout << "Enter applicant name: ";
    getline(cin, a.name);
    cout << "Enter age: ";
    while (!(cin >> a.age) || a.age < 0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter a valid positive integer for age: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Employment Status (Salaried/Self-Employed/Unemployed): ";
    getline(cin, a.employmentStatus);
    // Normalize
    for (auto &c : a.employmentStatus) c = (char)toupper(c);
    if (a.employmentStatus.rfind("SALARIED", 0) == 0) a.employmentStatus = "Salaried";
    else if (a.employmentStatus.rfind("SELF", 0) == 0) a.employmentStatus = "Self-Employed";
    else a.employmentStatus = "Unemployed";

    cout << "Monthly Income: ";
    while (!(cin >> a.monthlyIncome) || a.monthlyIncome < 0.0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter monthly income (numeric): ";
    }

    cout << "Credit Score (300-900): ";
    while (!(cin >> a.creditScore) || a.creditScore < 0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter credit score: ";
    }

    cout << "Existing Monthly EMI obligations: ";
    while (!(cin >> a.existingMonthlyEMI) || a.existingMonthlyEMI < 0.0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter existing total monthly EMI: ";
    }

    cout << "Requested Loan Amount: ";
    while (!(cin >> a.requestedLoanAmount) || a.requestedLoanAmount < 0.0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter requested loan amount: ";
    }

    cout << "Tenure (months): ";
    while (!(cin >> a.tenureMonths) || a.tenureMonths <= 0) {
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid. Enter tenure in months: ";
    }

    Decision d = evaluateApplicant(a);
    printDecisionReport(a, d);

    cout << "Thank you for using the Loan Eligibility Checker.\n";
    return 0;
}