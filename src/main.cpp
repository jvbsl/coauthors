#include <iostream>
#include <fstream>
#include <vector>
#include <ui.h>
#include <cstddef>
#include <string>
#include <map>
#include <stdio.h>
#include<windows.h>

#ifdef LINUX
constexpr int UnicodeOutputCPCode = 1251;
#else
constexpr int UnicodeOutputCPCode = 65001;
#endif

static std::vector<bool> selectedAuthors;
int onClosing(uiWindow *w, void *data)
{
    uiQuit();
    return -1;
}

void onAuthorToggle(uiCheckbox *c, void *data)
{
    auto index = reinterpret_cast<size_t>(data);
    selectedAuthors[index] = (bool)uiCheckboxChecked(c);
}

void btnAbort_Clicked(uiButton *b, void *data)
{
    bool *res = static_cast<bool *>(data);
    *res = true;
    uiQuit();
}

void btnNone_Clicked(uiButton *b, void *data)
{
    bool *res = static_cast<bool *>(data);
    *res = false;
    uiQuit();
}

void btnAccept_Clicked(uiButton *b, void *data)
{
    bool *res = static_cast<bool *>(data);
    *res = true;
    uiQuit();
}

enum ERROR_CODES
{
    SUCCESS,
    COULD_NOT_OPEN_FILE,
    NO_AUTHORS,
    ABORTED,
    OTHER
};

std::map<std::string, const char*> localization = {
    {"deAbort", "Abbrechen"},
    {"deNone", "Keine"},
    {"deCommit", "Commit"},
    {"deAddCoAuthors", "Co-Autoren hinzufügen?"},
    {"deCouldNotOpenCommitFileAt", "Konnte die Commit datei nicht öffnen: "},
    {"deCommitCanceled", "Commit abgebrochen"},
    {"deCouldNotOpenFileAt", "Konnte Datei im Pfad nicht öffnen: "},
    {"deUsage", "Nutzung: coauthor ROOT_ORDNER COMMIT_DATEI [SPRACHCODE]"},
    {"deNoAuthorsConfigured", "Keine Co-Autoren konfiguriert"},

    {"enAbort", "Abort"},
    {"enNone", "None"},
    {"enCommit", "Commit"},
    {"enAddCoAuthors", "Add co authors?"},
    {"enCouldNotOpenCommitFileAt", "Could not open commit file at: "},
    {"enCommitCanceled", "Commit canceled"},
    {"enCouldNotOpenFileAt", "Could not open file at: "},
    {"enUsage", "Usage: coauthor ROOT_FOLDER COMMIT_FILE [LANGUAGECODE]"},
    {"enNoAuthorsConfigured", "No co authors configured"},
};

int main(int argc, char **argv)
{
    
    SetConsoleOutputCP(UnicodeOutputCPCode); 
    std::string languageCode;
    if (argc < 4)
    {
        languageCode = "en";
    }
    else
    {
        languageCode = std::string(argv[3]);
    }

    if (argc < 3)
    {
        std::cerr << localization[languageCode + "Usage"] << std::endl;
        return ERROR_CODES::OTHER;
    }
    std::ifstream coauthorfile;
    auto coauthorFileName = std::string(argv[1]) + "/.coauthors";
    coauthorfile.open(coauthorFileName);

    if (!coauthorfile.is_open())
    {
        std::cerr << localization[languageCode + "CouldNotOpenFileAt"] << coauthorFileName << std::endl;
        return ERROR_CODES::SUCCESS;
    }

    uiInitOptions o = {};

    auto err = uiInit(&o);
    /*if (err != nullptr) {
        std::cerr << "error initializing ui: " << err << std::endl;
        return ERROR_CODES::OTHER;
    }*/

    auto w = uiNewWindow(localization[languageCode + "AddCoAuthors"], 300, 10, 0);
    uiWindowSetMargined(w, 1);

    auto box = uiNewVerticalBox();
    uiWindowSetChild(w, uiControl(box));

    uiWindowOnClosing(w, onClosing, nullptr);

    uiControlShow(uiControl(w));

    std::string coauthorLine;

    size_t currentLine = 0;

    std::vector<std::string> authors;

    while (std::getline(coauthorfile, coauthorLine))
    {
        bool isDefault = coauthorLine[0] == '+';

        if (!isDefault && coauthorLine[0] != '-')
        {
            continue;
        }

        auto cb = uiNewCheckbox(coauthorLine.c_str() + 1);

        uiCheckboxSetChecked(cb, isDefault);

        authors.push_back(coauthorLine.substr(1));
        selectedAuthors.push_back(isDefault);

        uiCheckboxOnToggled(cb, onAuthorToggle, reinterpret_cast<void *>(currentLine));

        uiBoxAppend(box, uiControl(cb), 0);
        currentLine++;
    }

    if (currentLine == 0)
    {
        std::cerr <<  localization[languageCode + "NoAuthorsConfigured"] << std::endl;
        return ERROR_CODES::SUCCESS;
    }

    bool doAccept = false;
    bool doAbort = false;

    auto btnCancel = uiNewButton(localization[languageCode + "Abort"]);
    uiButtonOnClicked(btnCancel, btnAbort_Clicked, &doAbort);
    auto btnNone = uiNewButton(localization[languageCode + "None"]);
    uiButtonOnClicked(btnNone, btnNone_Clicked, &doAccept);
    auto btnAccept = uiNewButton(localization[languageCode + "Commit"]);
    uiButtonOnClicked(btnAccept, btnAccept_Clicked, &doAccept);
    auto resultBox = uiNewHorizontalBox();

    uiBoxAppend(resultBox, uiControl(btnCancel), 1);
    uiBoxAppend(resultBox, uiControl(btnNone), 1);
    uiBoxAppend(resultBox, uiControl(btnAccept), 1);

    uiBoxAppend(box, uiControl(resultBox), 1);

    uiMain();
    if (doAbort)
    {
        std::cerr << localization[languageCode + "CommitCanceled"] << std::endl;
        return ERROR_CODES::ABORTED;
    }

    if (doAccept)
    {
        std::ofstream commitFile;
        auto commitFileName = std::string(argv[2]);
        commitFile.open(commitFileName, std::ofstream::out | std::ofstream::app);

        if (!commitFile.is_open())
        {
            std::cerr << localization[languageCode + "CouldNotOpenCommitFileAt"] << commitFileName << std::endl;
            return ERROR_CODES::OTHER;
        }

        for (int i = 0; i < currentLine; i++)
        {
            if (selectedAuthors[i])
            {
                commitFile << "\nCo-authored-by: " << authors[i];
            }
        }
    }

    return ERROR_CODES::SUCCESS;
}
