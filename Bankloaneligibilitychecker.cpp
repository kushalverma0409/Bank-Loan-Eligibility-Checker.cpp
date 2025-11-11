#include <iostream>
using namespace std;

int main() {
    string name;
    int age, creditScore, employmentYears;
    double income, existingEMI, loanAmount;
    
    cout << "===== BANK LOAN ELIGIBILITY CHECKER =====\n";

    cout << "Enter Applicant Name: ";
    getline(cin, name);
    
    cout << "Enter Age: ";
    cin >> age;

    cout << "Enter Monthly Income: ";
    cin >> income;

    cout << "Enter Existing EMI (if any, else 0): ";
    cin >> existingEMI;

    cout << "Enter Years of Employment: ";
    cin >> employmentYears;

    cout << "Enter Credit Score: ";
    cin >> creditScore;

    cout << "Enter Requested Loan Amount: ";
    cin >> loanAmount;

    cout << "\n--- Checking Eligibility ---\n";

    // Simple eligibility conditions
    if (age < 21 || age > 60) {
        cout << "❌ Not Eligible: Age must be between 21 and 60.\n";
    } 
    else if (income < 15000) {
        cout << "❌ Not Eligible: Minimum monthly income should be ₹15000.\n";
    } 
    else if (employmentYears < 1) {
        cout << "❌ Not Eligible: Minimum 1 year of employment required.\n";
    } 
    else if (creditScore < 600) {
        cout << "❌ Not Eligible: Credit score must be at least 600.\n";
    } 
    else if (existingEMI > (0.4 * income)) {
        cout << "❌ Not Eligible: EMI burden exceeds 40% of income.\n";
    } 
    else {
        cout << "✅ Congratulations " << name << "! You are Eligible for the Loan.\n";
        cout << "Eligible Loan Amount: ₹" << loanAmount << "\n";
    }

    cout << "\n===== Thank You for Using Bank Loan Checker =====\n";
    return 0;
}
