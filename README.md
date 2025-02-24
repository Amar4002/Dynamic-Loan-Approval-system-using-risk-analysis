**Introduction**

This document presents a comprehensive overview of a C++ program designed to evaluate loan applications using a combination of decision trees and a Naïve Bayes classifier. The program assesses applicants based on their credit scores, income, existing debts, and other financial metrics to determine the likelihood of loan approval.

**Key Concepts**

**The program employs several key concepts:**

1. Applicant Structure: A data structure to hold details about each loan applicant.
2. Min-Heap (Priority Queue): A data structure to prioritize applicants based on their risk scores.
3. Decision Tree: A model used to classify loan applications based on credit scores.
4. Naïve Bayes Classifier: A probabilistic model that predicts loan approval based on historical data.
5. Debt-to-Income Ratio (DTI): A financial metric used to assess an applicant's ability to manage monthly payments and repay debts.

**Code Structure**
The code is organized into several sections:

1. Applicant Structure: Defines the attributes of an applicant and calculates the risk score.
2. Priority Queue: Implements a min-heap to manage applicants based on their risk scores.
3. Decision Tree: Contains the logic for classifying loan applications.
4. Naïve Bayes Classifier: Implements the training and prediction logic for loan approval.
5. Display Function: Outputs the details of all applicants in a formatted manner.
6. Main Function: The entry point of the program, where the logic is executed.
