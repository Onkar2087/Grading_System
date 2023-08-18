#include <Windows.h> //for - WndProc, CreateWindow, RegisterClassEx, while(GetMessage(...))), case WM_Command, HWND, UNIT, WPARAM, LPARAM,CS_HREDRAW,
//WS_OVERLAPPEDWINDOW, BS_DEFPUSHBUTTON, GetModuleHandle, LoadIcon, LoadCursor, CreateWindow, SetWindowText, GetWindowText, DefWindowProc
#include <string>
#include <vector>
#define IDC_CALCULATE 1001 //used as an identifier and have a constant value
using namespace std;

// Global variables 
HWND hNameEdit, hGradeEdit, hResultsText; // //HWND[Handle to a Window] - Data Type, variables will handle to edit control Name, Grade and Result resp

// Student structure
struct Student {
    wstring name; //for non ASCII characters or wide characters
    double grade;
};

vector<Student> students;

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
//Windows Procedure - function responsible for processing messages sent to a window
//Long Result - Data type - represents result of a window procedure
//  CALLBACK - used to specify the calling convention that the function uses, 
// WindowProcedure - a callback function that processes messages sent to a window, 
//UINT[Unsigned Integer] - Data Type - 
//WPARAM[Word Parameter] - Data Type - 
//LPARAM[Long Paraneter] - Data Type - 
//hWnd, message, wParam, lParam - Parameteres resp

{
    switch (message) //message is of type Unsigned Integer
    {
    case WM_COMMAND: //WM_COMMAND - message code that is sent when a user selects a command item from a menu or clicks a button
        if (LOWORD(wParam) == IDOK) { //When a user clicks the OK button, wParam will be set to IDOK - here it is ADD
            // Get student name and grade from edit controls
            wchar_t name[100]; //wide character - Data Tyoe - to handle Unicode character - eg - Latin/Chinese characters or symbols
            wchar_t grade[10];
            GetWindowText(hNameEdit, name, sizeof(name)); //hNameEdit - retrieves the name and stor it in "name", Get Window Text - getting the text entered in the Window
            GetWindowText(hGradeEdit, grade, sizeof(grade)); //hGradeEdit - retrieves the grade and stor it in "grade"

            Student student;
            student.name = name;
            student.grade = stod(grade); // stod() - fn converts a string to double value

            students.push_back(student);

            // Clear edit controls
            SetWindowText(hNameEdit, L""); //sets the text of the edit control "hNameEdit" to an empty string.
            SetWindowText(hGradeEdit, L""); //sets the text of the edit control "hGradeEdit" to an empty string.

        }
        else if (LOWORD(wParam) == IDC_CALCULATE) //if Low Order matches the value 
        { 
            double totalGrade = 0.0;
            double highestGrade = students[0].grade; // initializes highestGrade with the first student's grade.
            double lowestGrade = students[0].grade; // initializes lowestGrade with the first student's grade.

            for (const auto& student : students) //range for loop, which traverse through students(vector) - each student one ny one
            {
                totalGrade += student.grade;
                if (student.grade > highestGrade) 
                {
                    highestGrade = student.grade;
                }
                if (student.grade < lowestGrade) 
                {
                    lowestGrade = student.grade;
                }
            }

            double averageGrade = totalGrade / students.size();

            // Display results
            wstring resultText = L"Average grade: " + to_wstring(averageGrade) + // L refers to that, it is a wide string
            L"\r\nHighest grade: " + to_wstring(highestGrade) +  //\r\n represents a newline character sequence in Windows-style line endings.
            L"\r\nLowest grade: " + to_wstring(lowestGrade); //"to_wstring" - converts int/float into wide string, conversion is done so concatenation would be possible
            //The + operator is used to concatenate these wide string segments and numeric values together into the resultText variable. As a result, resultText will contain a 
            //formatted string that displays the average, highest, and lowest grades, each on a new line.

            SetWindowText(hResultsText, resultText.c_str()); //setting the value of edit control "hResultsText" to the calculated result
        }
        break;

    case WM_DESTROY: //WM_DESTRY - That the window is being closed by the user or programmatically
        PostQuitMessage(0); //fn posts a quit message to the message queue, indicating that the program should exit
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam); //used for unhandled messages and calls the default window procedure
    }
    return 0;
}

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) //Entry point for the Windows application, similar to the main()
{
    // Create a main window
    WNDCLASSEX wcex = { sizeof(WNDCLASSEX), CS_HREDRAW | CS_VREDRAW, WndProc, 0, 0, GetModuleHandle(0), LoadIcon(0, IDI_APPLICATION),
                        LoadCursor(0, IDC_ARROW), HBRUSH(COLOR_WINDOW + 1), 0, L"StudentGradeApp", LoadIcon(0, IDI_APPLICATION) };
    //This structure contains the characteristics of the window class to be registered

    RegisterClassEx(&wcex); //This function registers the window class with Windows
    HWND hWnd = CreateWindow(L"StudentGradeApp", L"Student Grade Management", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        400, 300, 0, 0, hInstance, 0); //fn creates the main application window with specified properties

    // Create controls
    hNameEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 10, 200, 20, hWnd, 0, hInstance, 0); 
    //fn creates an edit control for entering student names
    hGradeEdit = CreateWindow(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER, 10, 40, 200, 20, hWnd, 0, hInstance, 0);
    //fn creates an edit control for entering student grades
    CreateWindow(L"BUTTON", L"Add", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 220, 10, 60, 50, hWnd, (HMENU)IDOK, hInstance, 0);
    //creates a button control labeled "Add" with the specified style
    CreateWindow(L"BUTTON", L"Calculate", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 290, 10, 80, 50, hWnd, (HMENU)IDC_CALCULATE, hInstance, 0);
    //creates a button control labeled "Calculate" with the specified style
    hResultsText = CreateWindow(L"STATIC", L"", WS_CHILD | WS_VISIBLE, 10, 80, 360, 180, hWnd, 0, hInstance, 0);
    //creates a static text control, which displays the calculated results

    ShowWindow(hWnd, nCmdShow); //fn displays the main window based on the nCmdShow
    UpdateWindow(hWnd); // fn updates the client area of the window and makes it visible.

    // Message loop
    MSG msg; //declares a message structure, which will be used to store messages retrieved from the message queue
    while (GetMessage(&msg, 0, 0, 0)) //loop retrieves messages from the message queue and processes them, runs untill "quit" message is recieved
    {
        TranslateMessage(&msg); //function translates certain messages (e.g., keyboard messages) before dispatching them to the 'WndProc' for processing.
        DispatchMessage(&msg); //dispatches the message to WndProc for processing
    }

    return (int)msg.wParam; //returns the exit code of the application, which is extracted from the wParam member of the quit message.
}