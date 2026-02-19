#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
// #include <iomanip>
#include <cctype>
using namespace std;

float matrix[100][100];
float constants[100];
string varNames[100];
int n;
int totalVars = 0;

int getVarIndex(string v)
{
    for (int i = 0; i < totalVars; i++)
        if (varNames[i] == v)
            return i;
    return -1;
}

void extractAndSortVars(string eqs[])
{
    totalVars = 0;
    for (int i = 0; i < n; i++)
    {
        string s = eqs[i];
        for (int j = 0; j < (int)s.length(); j++)
        {
            if (isalpha(s[j]))
            {
                string v = "";
                v += s[j];
                int k = j + 1;

                while (k < s.length() && isalpha(s[k]))
                {
                    v += s[k];
                    k++;
                }
                while (k < s.length() && isdigit(s[k]))
                {
                    v += s[k];
                    k++;
                }

                bool found = false;
                for (int m = 0; m < totalVars; m++)
                    if (varNames[m] == v)
                        found = true;

                if (!found)
                    varNames[totalVars++] = v;

                j = k - 1;
            }
        }
    }

    for (int i = 0; i < totalVars - 1; i++)
        for (int j = i + 1; j < totalVars; j++)
            if (varNames[i] > varNames[j])
                swap(varNames[i], varNames[j]);
}

string removeSpaces(string s)
{
    string result = "";
    for (char c : s)
        if (c != ' ')
            result += c;
    return result;
}

void parseSide(string side, int multiplier, int eqIndex)
{
    string term = "";

    for (int j = 0; j <= (int)side.length(); j++)
    {
        if (j == (int)side.length() || side[j] == '+' || (side[j] == '-' && j != 0))
        {
            if (term != "")
            {
                size_t xPos = string::npos;

                for (int t = 0; t < term.length(); t++)
                {
                    if (isalpha(term[t]))
                    {
                        xPos = t;
                        break;
                    }
                }

                if (xPos != string::npos)
                {
                    string coeffStr = term.substr(0, xPos);
                    string vName = "";
                    int k = xPos;

                    while (k < term.length() && isalpha(term[k]))
                        vName += term[k++];

                    while (k < term.length() && isdigit(term[k]))
                        vName += term[k++];

                    float val =
                        (coeffStr == "" || coeffStr == "+") ? 1 :
                        (coeffStr == "-") ? -1 :
                        atof(coeffStr.c_str());

                    int idx = getVarIndex(vName);

                    if (idx != -1)
                        matrix[eqIndex][idx] += val * multiplier;
                }
                else
                {
                    constants[eqIndex] -= atof(term.c_str()) * multiplier;
                }
            }

            if (j < (int)side.length())
                term = side[j];
        }
        else
        {
            term += side[j];
        }
    }
}


void parseEquations(string eqs[])
{

    for (int i = 0; i < 100; i++)
    {
        constants[i] = 0;
        for (int j = 0; j < 100; j++){
            matrix[i][j] = 0;
        }
            
    }

    for (int i = 0; i < n; i++)
    {
        string cleaned = removeSpaces(eqs[i]);
        size_t eqPos = cleaned.find('=');

        if (eqPos == string::npos || cleaned.find('=', eqPos + 1) != string::npos)
        {
            cout << "Invalid equation format. Each equation must contain exactly one '=' sign." << endl;
            cout << "Example of valid format: 2x1+3x2=16" << endl;
            return;
        }

        string lhs = cleaned.substr(0, eqPos);
        string rhs = cleaned.substr(eqPos + 1);


        parseSide(lhs, 1, i);
        parseSide(rhs, -1, i);
        
    }
}

void printFormattedEq(float coeffs[], float rhs)
{
    bool first = true;
    for (int j = 0; j < totalVars; j++)
    {
        if (coeffs[j] != 0)
        {
            if (!first && coeffs[j] > 0)
                cout << "+";
            if (coeffs[j] == 1)
                cout << varNames[j];
            else if (coeffs[j] == -1)
                cout << "-" << varNames[j];
            else
                cout << coeffs[j] << varNames[j];
            first = false;
        }
    }

    if (first)
        cout << "0";

    cout << "=" << rhs << endl;
}

float getDeterminant(float mat[100][100], int size)
{
    float temp[100][100];
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            temp[i][j] = mat[i][j];

    float det = 1.0;

    for (int i = 0; i < size; i++)
    {
        int pivot = i;
        for (int j = i + 1; j < size; j++)
            if (abs(temp[j][i]) > abs(temp[pivot][i]))
                pivot = j;

        swap(temp[i], temp[pivot]);

        if (pivot != i)
            det *= -1;

        if (abs(temp[i][i]) < 1e-9)
            return 0;

        det *= temp[i][i];

        for (int j = i + 1; j < size; j++)
        {
            float f = temp[j][i] / temp[i][i];
            for (int k = i + 1; k < size; k++)
                temp[j][k] -= f * temp[i][k];
        }
    }
    return det;
}

void initializeSystem()
{
    cout << "\nEnter number of equations: ";
    cout << "(Example: 3)" << endl;
    cin >> n;

    cin.ignore();

    string eqs[100];

    cout << "\nNow enter the " << n << " equations one per line." << endl;
    cout << "Each equation must follow this format: 2x1+3x2=16" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << "Equation " << i + 1 << ": ";
        getline(cin, eqs[i]);

        if (eqs[i].empty())
        {
            cout << "Equation cannot be empty. Please re-enter." << endl;
            i--;
            continue;
        }
    }

    extractAndSortVars(eqs);
    parseEquations(eqs);

    cout << "\nSystem loaded successfully." << endl;
    cout << "Available commands:" << endl;
    cout << "num_vars, equation, column, add, subtract, substitute, d, d_value, solve, restart, quit" << endl;
}

int main()
{
    cout << "==========================================" << endl;
    cout << "       LINEAR EQUATIONS SOLVER APP       " << endl;
    cout << "==========================================" << endl;

    initializeSystem();

    string cmd;

    while (true)
    {
        cout << "\nEnter a command: ";
        cin >> cmd;

        for (char &c : cmd)
            c = tolower(c);

        if (cmd == "quit")
        {
            cout << "Application closed successfully." << endl;
            break;
        }

        if (cmd == "restart")
        {
            cin.ignore();
            initializeSystem();
            continue;
        }

        if (cmd == "num_vars")
        {
            cout << "Total number of unique variables = " << totalVars << endl;
        }

        else if (cmd == "equation")
        {
            int idx;
            cout << "Enter equation number (1 to " << n << "): ";
            cin >> idx;

            if (idx < 1 || idx > n)
                cout << "Invalid equation number. It must be between 1 and " << n << "." << endl;
            else
                printFormattedEq(matrix[idx - 1], constants[idx - 1]);
        }

        else if (cmd == "column")
        {
            string v;
            cout << "Enter variable name (Example: x1): ";
            cin >> v;

            int vIdx = getVarIndex(v);

            if (vIdx == -1)
                cout << "Variable not found in the system." << endl;
            else
                for (int i = 0; i < n; i++)
                    cout << matrix[i][vIdx] << endl;
        }

        else if (cmd == "add")
        {
            int i, j;
            cout << "Enter two equation numbers to add (e.g., 1 " << n << "): ";
            cin >> i >> j;

            if (i < 1 || i > n || j < 1 || j > n)
                cout << "Invalid input. Equation numbers must be between 1 and " << n << "." << endl;
            else
            {
                float res[100];
                for (int k = 0; k < totalVars; k++)
                    res[k] = matrix[i - 1][k] + matrix[j - 1][k];

                printFormattedEq(res, constants[i - 1] + constants[j - 1]);
            }
        }

        else if (cmd == "subtract")
        {
            int i, j;
            cout << "Enter two equation numbers to subtract (e.g., 1 " << n << "): ";
            cin >> i >> j;

            if (i < 1 || i > n || j < 1 || j > n)
                cout << "Invalid input. Equation numbers must be between 1 and " << n << "." << endl;
            else
            {
                float res[100];
                for (int k = 0; k < totalVars; k++)
                    res[k] = matrix[i - 1][k] - matrix[j - 1][k];

                printFormattedEq(res, constants[i - 1] - constants[j - 1]);
            }
        }

        else if (cmd == "substitute")
        {
            string v;
            int i, j;

            cout << "Enter variable and equation numbers (e.g., x2 1 " << n << "): ";
            cin >> v >> i >> j;

            if (i == j)
                cout << "Substitution requires two different equations." << endl;
            else if (i < 1 || i > n || j < 1 || j > n)
                cout << "Equation numbers must be between 1 and " << n << "." << endl;
            else
            {
                int vIdx = getVarIndex(v);

                if (vIdx == -1)
                    cout << "Variable not found in the system." << endl;
                else if (matrix[j - 1][vIdx] == 0)
                    cout << "Cannot substitute. The variable coefficient in equation " << j << " is zero." << endl;
                else
                {
                    float factor = matrix[i - 1][vIdx] / matrix[j - 1][vIdx];
                    float res[100];

                    for (int k = 0; k < totalVars; k++)
                        res[k] = matrix[i - 1][k] - factor * matrix[j - 1][k];

                    printFormattedEq(res, constants[i - 1] - factor * constants[j - 1]);
                }
            }
        }

        else if (cmd == "d")
        {
            if (n != totalVars)
                cout << "Cramer's Rule requires number of equations equal to number of variables." << endl;
            else
                for (int r = 0; r < n; r++)
                {
                    for (int c = 0; c < totalVars; c++)
                        cout << matrix[r][c] << " ";
                    cout << endl;
                }
        }

        else if (cmd == "d_value")
        {
            if (n != totalVars)
                cout << "Cramer's Rule requires number of equations equal to number of variables." << endl;
            else
                cout << getDeterminant(matrix, n) << endl;
        }

        else if (cmd == "solve")
        {
            if (n != totalVars)
                cout << "Cannot solve using Cramer's Rule. Equations count must equal variables count." << endl;
            else
            {
                float dMain = getDeterminant(matrix, n);

                if (abs(dMain) < 1e-9)
                    cout << "No Solution. Determinant equals zero." << endl;
                else
                    for (int j = 0; j < totalVars; j++)
                    {
                        float tempMat[100][100];

                        for (int r = 0; r < n; r++)
                            for (int c = 0; c < n; c++)
                                tempMat[r][c] = (c == j ? constants[r] : matrix[r][c]);

                        cout << varNames[j] << "=" << getDeterminant(tempMat, n) / dMain << endl;
                    }
            }
        }

        else
        {
            cout << "Unknown command. Please enter a valid command from the list." << endl;
        }
    }

    return 0;
}
