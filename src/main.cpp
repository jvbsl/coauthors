#include <iostream>
#include <fstream>
#include <vector>
#include <ui.h>
#include <cstddef>
#include <string>


#ifdef LINUX
constexpr int ArgumentStartIndex = 1;
#else
constexpr int ArgumentStartIndex = 0;
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




int main(int argc, char** argv)
{
    if (argc < 2 + ArgumentStartIndex)
    {
        std::cerr << "Usage: coauthor ROOT_DIRECTORY COMMIT_FILE" << std::endl;
        return ERROR_CODES::OTHER;
    }
    std::ifstream coauthorfile;
    auto coauthorFileName = std::string(argv[0 + ArgumentStartIndex]) + "/.coauthors";
    coauthorfile.open(coauthorFileName);
    
    if (!coauthorfile.is_open()) {
        std::cerr << "Could not open file at: " << coauthorFileName << std::endl;
        return ERROR_CODES::COULD_NOT_OPEN_FILE;
    }

    uiInitOptions o = {};
    
    auto err = uiInit(&o);
    if (err != nullptr) {
        std::cerr << "error initializing ui: " << err << std::endl;
        return ERROR_CODES::OTHER;
    }
    
    auto w = uiNewWindow("Add co authors?", 300, 10, 0);
    uiWindowSetMargined(w, 1);
    
    
    auto box = uiNewVerticalBox();
    uiWindowSetChild(w, uiControl(box));
    
    uiWindowOnClosing(w, onClosing, nullptr);
    
    uiControlShow(uiControl(w));
    
    std::string coauthorLine;
    
    size_t currentLine = 0;
    
    std::vector<std::string> authors;
    
    while(std::getline(coauthorfile, coauthorLine))
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
        
        uiCheckboxOnToggled(cb, onAuthorToggle, reinterpret_cast<void*>(currentLine));
   
        uiBoxAppend(box, uiControl(cb), 0);
        currentLine++;
    }
    
    if (currentLine == 0)
    {
        std::cerr << "No authors configured" << std::endl;
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
        std::ofstream commitFile;
        auto commitFileName = std::string(argv[1 + ArgumentStartIndex]);
        commitFile.open(commitFileName, std::ofstream::out | std::ofstream::app);
        
        if (!commitFile.is_open())
        {
            std::cerr << "Could not open commit file at: " << commitFileName << std::endl;
            return ERROR_CODES::OTHER;
        }
        
        for (int i=0;i<currentLine;i++)
        {
            if (selectedAuthors[i])
            {
                commitFile << "\nCo-authored-by: " << authors[i];
            }
        }
    
        return ERROR_CODES::SUCCESS;
    }
    
    
    return ERROR_CODES::ABORTED;
}
