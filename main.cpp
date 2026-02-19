#include <iostream>
#include <string>
#include <cstdlib>
#include <cmath>
#include <iomanip>
#include <cctype>
using namespace std;

// --- البيانات الأساسية (Arrays Only) ---
float matrix[100][100];
float constants[100];
string varNames[100];
int n;
int totalVars = 0;



// --- دالة البحث عن مكان المتغير ---
int getVarIndex(string v)
{
    for (int i = 0; i < totalVars; i++)
    {
        if (varNames[i] == v)
            return i;
    }
    return -1;
}

// --- تحويل المعادلة للصورة الصحيحة وترتيب المتغيرات ---
void extractAndSortVars(string eqs[])
{
    totalVars = 0;
    for (int i = 0; i < n; i++)
    {
        string s = eqs[i];
        for (int j = 0; j < (int)s.length(); j++)
        {
            /**********first atempt accept only x ******** */
            // if (s[j] == 'x') {
            //     string v = "x";
            //     int k = j + 1;
            //     while (k < (int)s.length() && isdigit(s[k])) { v += s[k]; k++; }

            /**********second atempt accept only a character as a variable ******** */
            if (isalpha(s[j]))
            {

                string v = "";
                v += s[j]; // الحرف الأول

                int k = j + 1;

                
                // while (k < (int)s.length() && isdigit(s[k]))
                // {
                //     v += s[k];
                //     k++;
                // }
                  // جمع باقي الحروف
                while (k < s.length() && isalpha(s[k])) {
                    v += s[k];
                    k++;
                }
                 // جمع الأرقام بعد الحروف (اختياري)
                while (k < s.length() && isdigit(s[k])) {
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

            /*************** */
        }
    }
    for (int i = 0; i < totalVars - 1; i++)
    {
        for (int j = i + 1; j < totalVars; j++)
        {
            if (varNames[i] > varNames[j])
                swap(varNames[i], varNames[j]);
        }
    }
}

string removeSpaces(string s) {
    string result = "";
    for (char c : s) {
        if (c != ' ')
            result += c;
    }
    return result;
}

void parseEquations(string eqs[])
{
    // تصقير المصفوفات
    for (int i = 0; i < 100; i++)
    {
        constants[i] = 0;
        for (int j = 0; j < 100; j++){
            matrix[i][j] = 0;
        }
    }
    /************ */
        // for (int i = 0; i < n; i++)
        // {
        // if (eqs[i].find('=') == string::npos) {
        //     cout << "invalid try" << endl;
        //     exit(0);
        // }
        // size_t eqPos = eqs[i].find('=');
        // string lhs = eqs[i].substr(0, eqPos);
        // string rhs = eqs[i].substr(eqPos + 1);
        for (int i = 0; i < n; i++)
        {
            // size_t eqPos = eqs[i].find('=');
            string cleaned = removeSpaces(eqs[i]);
            size_t eqPos = cleaned.find('=');

            if (eqPos == string::npos ||
                cleaned.find('=', eqPos + 1) != string::npos)
            {
                cout << "invalid try" << endl;
                exit(0);
            }



            string lhs = cleaned.substr(0, eqPos);
            string rhs = cleaned.substr(eqPos + 1);



/***************** */
        // دالة فرعية منطقية لمعالجة أي طرف (طرف أيسر معامل بـ +1، طرف أيمن بـ -1)
        auto parseSide = [&](string side, int multiplier)
        {
            string term = "";
            for (int j = 0; j <= (int)side.length(); j++)
            {
                if (j == (int)side.length() || side[j] == '+' || (side[j] == '-' && j != 0))
                {
                    if (term != "")
                    {
                        // size_t xPos = term.find('x');
                        // size_t xPos = term.find_first_of("xy");
                        size_t xPos = string::npos;

                        for (int t = 0; t < term.length(); t++)
                        {
                            if (isalpha(term[t]))
                            {
                                xPos = t;
                                break;
                            }
                        }

                        /*************/
                        if (xPos != string::npos)
                        {
                            string coeffStr = term.substr(0, xPos);
                            // string vName = term.substr(xPos);
                            /************* */
                            string vName = "";
                            int k = xPos;

                            // جمع الحروف
                            while (k < term.length() && isalpha(term[k])) {
                                vName += term[k];
                                k++;
                            }

                            // جمع الأرقام بعد الحروف
                            while (k < term.length() && isdigit(term[k])) {
                                vName += term[k];
                                k++;
                            }

                            /********* */
                            float val = (coeffStr == "" || coeffStr == "+") ? 1 : (coeffStr == "-") ? -1
                                                                                                    : atof(coeffStr.c_str());
                            // إذا كان في الطرف الأيمن نضربه في -1 لنقله للأيسر
                            // matrix[i][getVarIndex(vName)] += (val * multiplier);
                            int idx = getVarIndex(vName);
                            if (idx != -1)
                                matrix[i][idx] += (val * multiplier);
                        }
                        else
                        {
                            // إذا كان رقماً مطلقاً، نقوم بنقله للطرف الأيمن (الثوابت)
                            // الرقم في الطرف الأيسر يروح للأيمن بعكس الإشارة، والعكس صحيح
                            constants[i] -= (atof(term.c_str()) * multiplier);
                        }
                    }
                    if (j < (int)side.length())
                        term = side[j];
                }
                else
                    term += side[j];
            }
        };

        parseSide(lhs, 1);  // معالجة الطرف الأيسر كما هو
        parseSide(rhs, -1); // معالجة الطرف الأيمن وعكس إشاراته لنقله لليسار
    }
}

// --- دالة الطباعة بالصورة الصحيحة ---
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
       // 🔥 لو كل المعاملات صفر
    if (first){
        cout << "0";        
    }

    cout << "=" << rhs << endl;
}

// --- دالة حساب المحدد (Determinant) ---
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
 cout << "\n[Step 1] Enter number of equations: ";
    if (!(cin >> n)){
        exit(0);
    }

    cin.ignore();

    string eqs[100];

    cout << "[Step 2] Enter the " << n << " equations (e.g., 2x1+3x2=16):" << endl;

    for (int i = 0; i < n; i++)
    {
        cout << " Equation " << i + 1 << ": ";
        getline(cin, eqs[i]);
        if (eqs[i].empty()) { i--; continue; }
    }

    extractAndSortVars(eqs);
    parseEquations(eqs);

    cout << "\n>>> System initialized successfully! <<<" << endl;
    cout << "Available Commands: num_vars, equation, column, add, subtract, substitute, D, D_value, solve, quit" << endl;

}

int main()
{
    cout << "==========================================" << endl;
    cout << "   LINEAR EQUATIONS SOLVER (PROPER FORM)   " << endl;
    cout << "==========================================" << endl;

   void initializeSystem();
    string ch;
    
    while (true)
    {
        cout << "\nChoose an operation: ";
        cin >> ch;

        for (char &c : cmd){
            c = tolower(c);
        }

        if (cmd == "quit"){
            break;
        }
       if (cmd == "restart") {
            cin.ignore();
            initializeSystem();   // 🔥 إعادة إدخال النظام
            continue;
        }

        if (cmd == "num_vars")
        {
            cout << "Number of unique variables: " << totalVars << endl;
        }

        // else if (cmd == "equation")
        // {
        //     int idx;
        //     cout << "Enter equation number (1 to " << n << "): ";
        //     cin >> idx;
        //     cout << "Proper Form: ";
        //     printFormattedEq(matrix[idx - 1], constants[idx - 1]);
        // }
        else if (cmd == "equation")
        {
            int idx;
            cout << "Enter equation number (1 to " << n << "): ";
            cin >> idx;

            if (idx < 1 || idx > n)
            {
                cout << "invalid try" << endl;
            }
            else
            {
                cout << "Proper Form: ";
                printFormattedEq(matrix[idx - 1], constants[idx - 1]);
            }
        }
        else if (cmd == "column")
        {
            string v;
            cout << "Enter variable name (e.g., x1): ";
            cin >> v;
            int vIdx = getVarIndex(v);
            if (vIdx != -1)
            {
                cout << "Column values for " << v << ":" << endl;
                for (int i = 0; i < n; i++)
                    cout << " " << matrix[i][vIdx] << endl;
            }
            else
                cout << "Variable not found!" << endl;
        }
        // else if (cmd == "add")
        // {
        //     int i, j;
        //     cout << "Enter two equation numbers to add (e.g., 1 3): ";
        //     cin >> i >> j;
        //     float res[100];
        //     for (int k = 0; k < totalVars; k++)
        //         res[k] = matrix[i - 1][k] + matrix[j - 1][k];
        //     cout << "Result: ";
        //     printFormattedEq(res, constants[i - 1] + constants[j - 1]);
        // }
        else if (cmd == "add")
        {
            int i, j;
            cout << "Enter two equation numbers to add (e.g., 1 " << n << "): ";
            cin >> i >> j;

            if (i < 1 || i > n || j < 1 || j > n)
            {
                cout << "invalid try" << endl;
            }
            else
            {
                float res[100];

                for (int k = 0; k < totalVars; k++)
                    res[k] = matrix[i - 1][k] + matrix[j - 1][k];
                cout << "Result: ";
                printFormattedEq(res,
                                 constants[i - 1] + constants[j - 1]);
            }
        }

        // else if (cmd == "subtract")
        // {
        //     int i, j;
        //     cout << "Enter equation numbers (Equation i - Equation j): ";
        //     cin >> i >> j;
        //     float res[100];
        //     for (int k = 0; k < totalVars; k++)
        //         res[k] = matrix[i - 1][k] - matrix[j - 1][k];
        //     cout << "Result: ";
        //     printFormattedEq(res, constants[i - 1] - constants[j - 1]);
        // }

        else if (cmd == "subtract")
        {
            int i, j;
            cout << "Enter two equation numbers to subtract (e.g., 1 " << n << "): ";
            cin >> i >> j;

            if (i < 1 || i > n || j < 1 || j > n)
            {
                cout << "invalid try" << endl;
            }
            else
            {
                float res[100];

                for (int k = 0; k < totalVars; k++)
                    res[k] = matrix[i - 1][k] - matrix[j - 1][k];

                cout << "Result: ";
                printFormattedEq(res,
                                 constants[i - 1] - constants[j - 1]);
            }
        }

        // else if (cmd == "substitute")
        // {
        //     string v;
        //     int i, j;
        //     cout << "Substitute variable (var) from first Eq into the second Eq: (e.g., x2 2 1) ";
        //     cin >> v >> i >> j;
        //     int vIdx = getVarIndex(v);
        //     if (vIdx != -1 && matrix[j - 1][vIdx] != 0)
        //     {
        //       3  float factor = matrix[i - 1][vIdx] / matrix[j - 1][vIdx];
        //         float res[100];
        //         for (int k = 0; k < totalVars; k++)
        //             res[k] = matrix[i - 1][k] - factor * matrix[j - 1][k];
        //         cout << "New Equation: ";
        //         printFormattedEq(res, constants[i - 1] - factor * constants[j - 1]);
        //     }
        //     else
        //         cout << "Error: Check variable or division by zero." << endl;
        // }
        else if (cmd == "substitute")
        {
            string v;
            int i, j;

            cin >> v >> i >> j;

            if (i == j || i < 1 || i > n || j < 1 || j > n)
            {
                cout << "invalid try" << endl;
            }
            else
            {
                int vIdx = getVarIndex(v);

                if (vIdx == -1 ||
                    matrix[i - 1][vIdx] == 0 ||
                    matrix[j - 1][vIdx] == 0)
                {

                    cout << "invalid try" << endl;
                }
                else
                {

                    float factor =
                        matrix[i - 1][vIdx] /
                        matrix[j - 1][vIdx];

                    float res[100];

                    for (int k = 0; k < totalVars; k++)
                        res[k] = matrix[i - 1][k] - factor * matrix[j - 1][k];

                    cout << "New Equation: ";
                    printFormattedEq(res,
                                     constants[i - 1] - factor * constants[j - 1]);
                }
            }
        }

        /************/
        // else if (cmd == "D") {
        //     cout << "Coefficient Matrix (D):" << endl;
        //     for (int r = 0; r < n; r++) {
        //         for (int c = 0; c < totalVars; c++)
        //         cout << matrix[r][c] << (c == totalVars - 1 ? "" : "\t");
        //         cout << endl;
        //     }
        // }
      else if (cmd == "d"){

    string v;

    if (cin.peek() != '\n') {

        cin >> v;

        // منع D x2 x3
        if (cin.peek() != '\n') {
            cout << "invalid try" << endl;
            continue;
        }

        if (n != totalVars) {
            cout << "invalid try" << endl;
            continue;
        }

        int vIdx = getVarIndex(v);

        if (vIdx == -1) {
            cout << "invalid try" << endl;
        }
        else {
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < totalVars; c++) {

                    if (c == vIdx)
                        cout << constants[r];
                    else
                        cout << matrix[r][c];

                    if (c != totalVars - 1)
                        cout << " ";
                }
                cout << endl;
            }
        }
    }
    else {

        if (n != totalVars) {
            cout << "invalid try" << endl;
        }
        else {
            for (int r = 0; r < n; r++) {
                for (int c = 0; c < totalVars; c++) {
                    cout << matrix[r][c];
                    if (c != totalVars - 1)
                        cout << " ";
                }
                cout << endl;
            }
        }
    }
}

        // else if (cmd == "D_value")
        // {
        //     cout << "Determinant (D) = " << getDeterminant(matrix, n) << endl;
        // }
        else if (cmd == "d_value") {

    if (n != totalVars) {
        cout << "invalid try" << endl;
    }
    else {
        cout << "Determinant (D) = ";
        cout << getDeterminant(matrix, n) << endl;
    }
}
/***************** */
        // else if (cmd == "solve")
        // {
        //     float dMain = getDeterminant(matrix, n);
        //     if (abs(dMain) < 1e-9)
        //         cout << "No Unique Solution (D=0)" << endl;
        //     else
        //     {
        //         cout << "Solutions (Cramer's Rule):" << endl;
        //         for (int j = 0; j < totalVars; j++)
        //         {
        //             float tempMat[100][100];
        //             for (int r = 0; r < n; r++)
        //                 for (int c = 0; c < n; c++)
        //                     tempMat[r][c] = (c == j ? constants[r] : matrix[r][c]);
        //             cout << " " << varNames[j] << " = " << getDeterminant(tempMat, n) / dMain << endl;
        //         }
        //     }
        // }

else if (cmd == "solve") {

    if (n != totalVars) {
        cout << "No Solution Cramer's Rule needs number of variable to be equl to number of equations" << endl;
    }
    else {

        float dMain = getDeterminant(matrix, n);

        if (abs(dMain) < 1e-9) {
            cout << "No Solution" << endl;
        }
        else {

            for (int j = 0; j < totalVars; j++) {

                float tempMat[100][100];

                for (int r = 0; r < n; r++)
                    for (int c = 0; c < n; c++)
                        tempMat[r][c] =
                            (c == j ? constants[r]
                                    : matrix[r][c]);

                cout << varNames[j]
                     << "="
                     << getDeterminant(tempMat, n) / dMain
                     << endl;
            }
        }
    }
}

        /******************* */
        else
        {
            cout << "Unknown command. Please try again." << endl;
        }
    }

    cout << "\nExiting... Goodbye!" << endl;
    return 0;
}



// 1️⃣ عدد المعادلات ≠ عدد المتغيرات (قبل solve / D_value)
// لو عندك:
// 3 معادلات
// 4 متغيرات
// يبقى:
// solve → invalid try
// D_value → invalid try
// D x2 → invalid try
// /****************/
// 2️⃣ solve لما D = 0

// أنت حالياً بتطبع:

// No Unique Solution


// بس حسب نص المشروع لازم:

// No Solution


// /***********/

// 3️⃣ column

// لو كتب:

// column x9


// لازم invalid try
// (متأكد تعملها)

// بس كمان:

// لو كتب:

// column


// من غير متغير
// لازم تتحمي من crash.


// 4️⃣ D مع إدخال زيادة

// لو كتب:

// D x2 x3


// ده إدخال غلط
// لازم invalid try.

// 2️⃣ substitute نفس المعادلة

// لو كتب:

// substitute x2 1 1


// هل ده منطقي؟

// رياضياً:

// هتطرح المعادلة من نفسها → صفر.

// لكن غالباً الدكتور يعتبره invalid try.

// أنصح تتحقق:

// i != j


// /************/


// 6️⃣ division by zero غير مباشر

// في substitute أنت بتتأكد إن معامل المتغير في المعادلتين ≠ 0
// تمام.

// لكن في solve لازم تتأكد:

// totalVars == n


// وإلا determinant هيتحسب غلط.

// 7️⃣ add / subtract بأرقام سالبة أو صفر

// لو حد كتب:

// add 0 2
// add -1 3


// لازم invalid try.

// 8️⃣ equation رقم غير صحيح

// لو كتب:

// equation 0
// equation -3


// invalid try

// 9️⃣ أوامر غلط

// لو كتب:

// adds
// sub
// DD


// مفروض تطبع:

// invalid try


// مش:

// Unknown command


// لأن نص المشروع بيقول:

// مفيش كلام زيادة في الـ output

/***************** */

// دي النسخه القبل الاخيره من المشروع 
// دلوقتي انا run المشروع عايزك تبعتلي كل test case اجرب بيها و تكون علي 3 مراحل المرحله الاوله المرحله العاديه اتاكد ان كل حاجه شغاله في المشروع بكل الادخالات  
// المرحله التانيه انك تجرب ب test case تجرب بيها الحاجات اللي بتتاكد منها ان الuser مسمم يدخل كل الinputs الغلط عشان يطلع عيب 
// المرحله التالته 
// الtest case التعجيزيه الحاجات اللي بتعمل crash لل كود 



/***********************/



// upper و lower case 

// commands 


/*********************/


// غير عدد ال EQS و غيرهم نفسهم 
// كانك عملت RESTart 

/*********************/

// سيب الكود كله زي ما هو متغير في حاجه ضيف بس لكن متمسحش اي حاجه ولا حتي نقطه ولا مسافه.

// عايزك بس تعمل الي انا هقولك عليه 

// جوه الكود ده في اكواد كتير معمولها hash كانها comment, دي التجارب الاوله للجزء ده بعد ما لقيت في تعديل او غلط .
// انا عايز منك تقرا الكود كله بالاجزاء المعموله comment كمان و تكتب فوق كل جزء معمول comment تعليق بيقول ان دي محاوله رقم 1 مقلا المشكله: كذا كذا 
// و التعديل كان كذا كذا كذا 



/************************* */

// سيبك من "لا يوجد أي نص إضافي"

// عدل كل invalid try او error msg  لل كلام منطقي 
// و خليه يقول سبب ال user يفهم منه 
//  و ضيف كمان قبل كل cin ضيف cout توضيحيه للuser عشان يفهم يعمل ايه بظبط و كمان مثال توضيحي للطريقه زي كده مثلا: (e.g., 2x1+3x2=16)
//  ) 
//  و الحته دي عجبتني ضيفها بدل ما تكتب مثلا (e.g., x2 2 1) 
//  اكتب :
//  (e.g., 1 " << n << "):

// يعني من الاخر كده عايزه يحس ان uiux هو اللي عمل ال project ده يخليه يحس انه فتح app علي الcmd
// ميخرجش منه غير لو دخل quit 


/***********************************/




// 🔴 أهم اقتراح احترافي

// بدل ما تمشي حرف حرف manually، اعمل:

// Tokenizer بسيط

// يمشي على المعادلة كأنها Scanner:

// اقرأ sign

// اقرأ coefficient

// اقرأ variable

// خزنه

// ده هيخلي parser 100% stable.


/*******************/

// 🔥 ليه ده أفضل من substr؟

// لأنه يمنع حالات زي:

// 2xyabc+3


// إنه يعتبر xyabc متغير كامل لو حصل parsing غلط.

// دلوقتي أنت محدد:

// اقرأ حروف

// اقرأ أرقام

// وقف

// بالظبط زي extraction الأولاني.

// 🧠 بعد التعديل ده

// هيبقى يدعم:

// 2xy+3ab=10
// 4xx-2aB1=7


// بدون أي bugs في اسم المتغير.

// ⚠️ أهم نقطة

// لازم يكون extraction في:

// extractAndSortVars()

// parseSide()

// بنفس المنطق 100%

// وإلا هتظهر Bugs subtle جداً.

// لو عايز أعملك نسخة موحدة بدالة اسمها:

// string extractVariable(string term, int startIndex)


// ونستخدمها في الاتنين (احترافي جداً 👑)

// قولي:

// اعملها بدالة واحدة نظيفة

// ونرتب الكود صح.

/**************** */


// 👑 هل تحب:

// 1️⃣ نخليه clean بدون أي extra text نهائي
// 2️⃣ نخليه interactive وجاهز للاستعمال الحقيقي

// قولّي تختار أي مود 🔥