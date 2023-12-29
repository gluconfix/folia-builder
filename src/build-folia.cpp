#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <windows.h>
#include <string>

HWND hwndConsole;

void executeCommand(const char *command) {
    // Convert const char* to LPCWSTR
    int size = MultiByteToWideChar(CP_ACP, 0, command, -1, NULL, 0);
    wchar_t* wCommand = new wchar_t[size];
    MultiByteToWideChar(CP_ACP, 0, command, -1, wCommand, size);

    // Use the wide string in the function
    _wsystem(wCommand);

    delete[] wCommand; // Don't forget to free the allocated memory
}


void build() {
    SetConsoleTitleA("Build Folia • Cloning Repository...");
    executeCommand("cd / && git clone git@github.com:PaperMC/Folia.git");
    executeCommand("");
    SetConsoleTitleA("Build Folia • Applying patches...");
    executeCommand("cd /Folia && .\\gradlew applypatches --stacktrace");
    SetConsoleTitleA("Build Folia • Creating Mojmap Jar...");
    executeCommand("cd /Folia && .\\gradlew createMojmapPaperclipJar --stacktrace");
    SetConsoleTitleA("Build Folia • Creating Bundler Jar...");
    executeCommand("cd /Folia && .\\gradlew createReobfBundlerJar --stacktrace");
    SetConsoleTitleA("Finished creating Folia");
    executeCommand("move /Folia .");
    executeCommand("explorer Folia\\build\\libs");
    executeCommand("cd ..");
    executeCommand("pause");
}


bool directoryExists(const std::string& path) {
    DWORD attrib = GetFileAttributesA(path.c_str());
    return (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY));
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    std::wstring cmdLine = GetCommandLineW();
    if (!(cmdLine.find(L"/cli") != std::wstring::npos)) {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string command = "conhost.exe -- \"";
        command += buffer;
        command += "\" /cli";
        executeCommand(command.c_str());
        return 0;
    }
    
    // Allocate a console window
    AllocConsole();
    hwndConsole = FindWindowA("ConsoleWindowClass", NULL);

    // Redirect standard input/output to the console window
    //freopen("CONIN$", "r", stdin);
    //freopen("CONOUT$", "w", stdout);
    //freopen("CONOUT$", "w", stderr);

    printf("Please make sure that the folder of this application isn't as deeply nested, because this might lead to errors!\n\n");
    SetConsoleTitleA("Build Folia");

    executeCommand("chcp 65001");
    executeCommand("cls");

    const char folderPathC[] = "/Folia";  // C-style string
    const std::string folderPath(folderPathC, sizeof(folderPathC) - 1);

    if (directoryExists(folderPath)) {
        executeCommand("rmdir /s /q \"/Folia\"");
        if (directoryExists(folderPath)) {
            printf("Please delete Folia Folder before launching this application\n");
            executeCommand("pause");
            FreeConsole();
            return 1;
        }
    }

    build();

    // Free the console window when done
    FreeConsole();

    return 0;
}
