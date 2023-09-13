#include <string>
#include <iostream>
#include <fstream>
#include <chrono>
#include <windows.h>
#include <iomanip>

using namespace std;

const float INITIAL_MULT = 0.3f;

double measure(const string& program_name, const string& input_file, bool isJava) {
    // Open the input file
    ifstream input(input_file);
    if (!input.is_open()) {
        cerr << "Error: could not open input file " << input_file << endl;
        return -1;
    }

    // Create the pipe for standard input
    HANDLE child_stdin_read, child_stdin_write;
    SECURITY_ATTRIBUTES sa;
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&child_stdin_read, &child_stdin_write, &sa, 0)) {
        cerr << "Error: could not create pipe for child process standard input" << endl;
        return -1;
    }

    // Create the process to execute the program
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));
    si.hStdInput = child_stdin_read;
    si.dwFlags |= STARTF_USESTDHANDLES;

    if(!isJava)
    {    
        string command = program_name + ".exe";
        if (!CreateProcess(command.c_str(), NULL, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            cerr << "Error: could not create process for " << program_name << endl;
            return -1;
        }
    }
    else
    {
        string cmd = "java " + program_name;
        char* lpCommandLine = const_cast<char*>(cmd.c_str());
        if (!CreateProcess(NULL, lpCommandLine, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
            cerr << "Error: could not create process for " << program_name << endl;
            return -1;
        }
    }

    // Write the contents of the input file to the standard input pipe of the process
    string line;
    while (getline(input, line)) {
        line += '\n'; // Add the newline character that was stripped by getline
        DWORD written;
        if (!WriteFile(child_stdin_write, line.c_str(), line.length(), &written, NULL)) {
            cerr << "Error: could not write to child process standard input" << endl;
            return -1;
        }
    }

    // Close the input file and the write end of the standard input pipe
    input.close();
    CloseHandle(child_stdin_write);

    // Wait for the process to finish and measure its CPU execution time 
    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code;
    GetExitCodeProcess(pi.hProcess, &exit_code);

    //Process Information data
        FILETIME creation_time, exit_time, kernel_time, user_time;
    if (!GetProcessTimes(pi.hProcess, &creation_time, &exit_time, &kernel_time, &user_time)){
        cerr << "Error: could not retrieve process times" << endl;
        return -1;
    }

    ULARGE_INTEGER kernel_time_ul, user_time_ul;
    kernel_time_ul.LowPart = kernel_time.dwLowDateTime;
    kernel_time_ul.HighPart = kernel_time.dwHighDateTime;
    user_time_ul.LowPart = user_time.dwLowDateTime;
    user_time_ul.HighPart = user_time.dwHighDateTime;
    double cpu_time = (kernel_time_ul.QuadPart + user_time_ul.QuadPart)  / 10000.0; // Convert to milliseconds
    //cout << "GetProcessTimes CPU execution time: " << std::setprecision(15) << cpu_time << " ms" << endl;

    // Clean up
    CloseHandle(child_stdin_read);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return cpu_time;
}

double average(double times[], const int numMeasurements)
{
    int minId = 0, maxId = 0;
    double total = 0;
    for(int i=0; i<numMeasurements; i++)
    {
        if(times[i] < times[minId]) minId = i;
        if(times[i] > times[maxId]) maxId = i;
        total += times[i];
    }
    total -= times[minId];
    total -= times[maxId];
    total /= (numMeasurements-2);
    return total;
}

void run(string exeFile, ofstream& allFile, int maxLength, int problemType, bool isJava)
{
    string inputFilePaths[] = { "casos_33%.txt", "casos_66%.txt", "casos_99%.txt" };
    allFile << exeFile << (isJava ? "_JAVA" : "_CPP") << ", ";
	float mult = INITIAL_MULT;
    for (const string& inputFilePath : inputFilePaths) 
    {
        int lenght = maxLength * mult;
		mult += INITIAL_MULT;

        const int numMeasurements = 3;
        double times[numMeasurements];
        for(int i=0; i<numMeasurements; i++)
        {
            times[i] = measure(exeFile, inputFilePath, isJava);
            cout << "Time " << i << ": " << times[i] << endl;
            if(times[i] == 0.0){
                cout << "Time read was 0, repeating last execution..." << endl;
                i--;
            }
            else if(times[i] == -1){
                cout << "Error while launching this measurement..." << endl;
                return;
            }
        }
        double avg = average(times, numMeasurements);
        cout << "Time avg: " << avg << endl;
        allFile << lenght << ", " << avg << ", ";
    }
    allFile << problemType << ", " << problemType << endl;
}

int main()
{
	ifstream infoFile;
	string curFileName;
	int maxLength, problemType;
	infoFile.open("problemInfo.txt");
	if (!infoFile.is_open())
	{
		cout << "no info file found! name should be: problemInfo.txt" << endl;
		return 1;
	}
	infoFile >> curFileName >> maxLength >> curFileName >> problemType;
	infoFile >> curFileName;

	ofstream allFile;
	allFile.open("tiempos_all.txt");
	allFile << "name,tamano,tiempo,tamano,tiempo,tamano,tiempo,tipo,tipoProblema" << endl;

    bool isJava = false;
	while (!infoFile.eof())
	{
        cout << "Executing file " << curFileName << " in "<< (isJava ? "java": "c++") << endl;
        run(curFileName, allFile, maxLength, problemType, isJava);
		infoFile >> curFileName;
        if(curFileName == "java"){
            isJava=true; 
            infoFile >> curFileName;
        }
	}
    
    cout << "Executing file " << curFileName << endl;
    run(curFileName, allFile, maxLength, problemType, isJava);

	allFile.close();
	cout << "DONE!" << endl;
    system("pause");

	return 0;
}