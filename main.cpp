#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>

using namespace std;

// ---------------------------- STRUCTURE TO HOLD APPLICANT DETAILS ---------------------------- //
struct Applicant {
    string name;
    int credit_score;
    double income;
    double loan_amount;
    double existing_debt;
    int defaults;
    double dti; // Debt-to-Income Ratio
    double risk_score;

    // Constructor to initialize applicant details
    Applicant(string n, int cs, double inc, double la, double ed, int d)
        : name(n), credit_score(cs), income(inc), loan_amount(la), existing_debt(ed), defaults(d) {
        
        dti = (income > 0) ? (existing_debt / income) : 1e9; // Avoid division by zero
        risk_score = (1.0 / credit_score) + dti + (0.5 * defaults); // Formula for risk score
    }
};

// ---------------------------- MIN-HEAP (PRIORITY QUEUE) FOR LOWEST RISK APPLICANTS ---------------------------- //
struct CompareRisk {
    bool operator()(Applicant a, Applicant b) {
        return a.risk_score > b.risk_score;
    }
};

// Global priority queue (min-heap)
priority_queue<Applicant, vector<Applicant>, CompareRisk> loanQueue;

// ---------------------------- DECISION TREE IMPLEMENTATION ---------------------------- //
struct DecisionTreeNode {
    int threshold;
    DecisionTreeNode* left;
    DecisionTreeNode* right;

    DecisionTreeNode(int t) : threshold(t), left(nullptr), right(nullptr) {}
};

// Function to classify loan approval using the Decision Tree
string classifyLoan(DecisionTreeNode* tree, int credit_score) {
    if (!tree) return "Undecided";
    if (credit_score < tree->threshold)
        return (tree->left) ? classifyLoan(tree->left, credit_score) : "Rejected";
    else
        return (tree->right) ? classifyLoan(tree->right, credit_score) : "Approved";
}

// ---------------------------- NAÏVE BAYES CLASSIFIER ---------------------------- //
class NaiveBayes {
private:
    map<int, pair<int, int>> probabilities; // {category: {approved_count, rejected_count}}

public:
    void train(vector<Applicant>& applicants) {
        for (auto& app : applicants) {
            int category = (app.credit_score >= 600) ? 1 : 0;
            probabilities[category].first += (app.defaults == 0);
            probabilities[category].second += (app.defaults == 1);
        }
    }

    string predict(Applicant& applicant) {
        int category = (applicant.credit_score >= 600) ? 1 : 0;
        double approveProb = (double)probabilities[category].first /
                             (probabilities[category].first + probabilities[category].second + 1); // Smoothing
        return (approveProb > 0.5) ? "Approved" : "Rejected";
    }
};

// ---------------------------- FUNCTION TO DISPLAY APPLICANT DETAILS ---------------------------- //
void displayApplicants(vector<Applicant>& applicants) {
    cout << "\n------------------------------------------------------\n";
    cout << "List of Loan Applicants:\n";
    cout << "------------------------------------------------------\n";
    cout << setw(12) << left << "Name"
         << setw(15) << "Credit Score"
         << setw(10) << "Income"
         << setw(10) << "Loan Amt"
         << setw(12) << "Debt"
         << setw(12) << "DTI Ratio"
         << setw(12) << "Risk Score\n";
    cout << "------------------------------------------------------\n";

    for (const auto& app : applicants) {
        cout << setw(12) << app.name
             << setw(15) << app.credit_score
             << setw(10) << app.income
             << setw(10) << app.loan_amount
             << setw(12) << app.existing_debt
             << setw(12) << fixed << setprecision(2) << app.dti
             << setw(12) << fixed << setprecision(2) << app.risk_score << "\n";
    }
    cout << "------------------------------------------------------\n\n";
}

// ---------------------------- MAIN FUNCTION ---------------------------- //
int main() {
    // Sample Applicants
    vector<Applicant> applicants = {
        {"Amar", 750, 5000, 10000, 1000, 0},
        {"Snehal", 550, 2000, 7000, 4000, 1},
        {"Nikita", 620, 4500, 12000, 2000, 0},
        {"sudhanshu", 400, 1800, 5000, 3000, 1},
        {"kajal", 680, 6000, 15000, 2500, 0}
    };

    // Populate Priority Queue (Min-Heap)
    for (auto& app : applicants) {
        loanQueue.push(app);
    }

    // Decision Tree Structure (Expanded for More Granularity)
    DecisionTreeNode* tree = new DecisionTreeNode(600);
    tree->left = new DecisionTreeNode(500);
    tree->right = new DecisionTreeNode(700);
    tree->left->left = new DecisionTreeNode(450);
    tree->right->left = new DecisionTreeNode(650);
    tree->right->right = new DecisionTreeNode(750);

    // Train Naïve Bayes Classifier
    NaiveBayes nbClassifier;
    nbClassifier.train(applicants);

    // Display All Applicants
    displayApplicants(applicants);

    // Process Loan Applications
    while (!loanQueue.empty()) {
        Applicant app = loanQueue.top();
        loanQueue.pop();
        
        // Decision Tree Prediction
        string decisionTreeResult = classifyLoan(tree, app.credit_score);

        // Naïve Bayes Prediction
        string naiveBayesResult = nbClassifier.predict(app);

        // Final Decision
        string finalDecision = (decisionTreeResult == "Approved" && naiveBayesResult == "Approved") ? "APPROVED" : "REJECTED";

        // Display Results
        cout << "Applicant: " << app.name << "\n";
        cout << "Credit Score: " << app.credit_score << "\n";
        cout << "Debt-to-Income Ratio: " << fixed << setprecision(2) << app.dti << "\n";
        cout << "Risk Score: " << fixed << setprecision(2) << app.risk_score << "\n";
        cout << "Decision Tree Prediction: " << decisionTreeResult << "\n";
        cout << "Naïve Bayes Prediction: " << naiveBayesResult << "\n";
        cout << "Final Decision: " << finalDecision << "\n";
        cout << "---------------------------------------\n";
    }

    // Free Decision Tree Memory
    delete tree->left->left;
    delete tree->left;
    delete tree->right->left;
    delete tree->right->right;
    delete tree->right;
    delete tree;

    return 0;
}
