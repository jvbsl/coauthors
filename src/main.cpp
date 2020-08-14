#include <iostream>
#include <fstream>
#include <vector>
#include <ui.h>
#include <cstddef>
#include <string>

int onClosing(uiWindow *w, void *data)
{
	uiQuit();
	return -1;
}

void btnCancel_Clicked(uiButton *b, void *data)
{
    bool* res = static_cast<bool*>(data);
    *res = false;
	uiQuit();
}

void btnAccept_Clicked(uiButton *b, void *data)
{
    bool* res = static_cast<bool*>(data);
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

#ifdef LINUX
constexpr int ArgumentStartIndex = 1;
#else
constexpr int ArgumentStartIndex = 0;
#endif


int main(int argc, char** argv)
{
    if (argc < 2 + ArgumentStartIndex)
    {
        std::cout << "Usage: coauthor ROOT_DIRECTORY COMMIT_FILE" << std::endl;
        return ERROR_CODES::OTHER;
    }
    std::ifstream coauthorfile;
    auto coauthorFileName = std::string(argv[0 + ArgumentStartIndex]) + "/.coauthors";
    coauthorfile.open(coauthorFileName);
    
    if (!coauthorfile.is_open()) {
        std::cout << "Could not open file at: " << coauthorFileName << std::endl;
        return ERROR_CODES::COULD_NOT_OPEN_FILE;
    }

    uiInitOptions o = {};
    
    auto err = uiInit(&o);
    if (err != nullptr) {
        std::cout << "error initializing ui: " << err << std::endl;
    }
    
    auto w = uiNewWindow("Add co authors?", 300, 10, 0);
    uiWindowSetMargined(w, 1);
    
    
    auto box = uiNewVerticalBox();
    uiWindowSetChild(w, uiControl(box));
    
    uiWindowOnClosing(w, onClosing, nullptr);
    
    uiControlShow(uiControl(w));
    
    std::string coauthorLine;
    
    int currentLine = 0;
    while(std::getline(coauthorfile, coauthorLine))
    {
        bool isDefault = coauthorLine[0] == '+';
        
        if (!isDefault && coauthorLine[0] != '-')
        {
            continue;
        }

        auto cb = uiNewCheckbox(coauthorLine.c_str() + 1);
        
        uiCheckboxSetChecked(cb, isDefault);
   
        uiBoxAppend(box, uiControl(cb), 0);
        currentLine++;
    }
    
    if (currentLine == 0)
    {
        return ERROR_CODES::NO_AUTHORS;
    }

    bool doAccept = false;

    auto btnCancel = uiNewButton("Cancel");
    uiButtonOnClicked(btnCancel, btnCancel_Clicked, &doAccept);
    auto btnAccept = uiNewButton("Accept");
    uiButtonOnClicked(btnAccept, btnAccept_Clicked, &doAccept);
    auto resultBox = uiNewHorizontalBox();

    
    uiBoxAppend(resultBox, uiControl(btnCancel), 1);
    uiBoxAppend(resultBox, uiControl(btnAccept), 1);
    
    uiBoxAppend(box, uiControl(resultBox), 1);
    
    uiMain();
    
    if (doAccept)
    {
        return ERROR_CODES::SUCCESS;
    }
    
    
    return ERROR_CODES::ABORTED;
}
