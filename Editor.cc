#include <iostream>
#include <string>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <cctype>

using namespace std;

void gotoxy(int x, int y) {

    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),coord);

}

class Editor {
    private:
        string content;
        int pLoc;

        void showPointer();
    public:
        Editor(string content);
        void openEditor();
        void editMode();
        string getContent();
};

Editor::Editor(string content = ""){
    this->content = content;
    pLoc = 0;
}

string Editor::getContent() {
    return content;
}

void Editor::showPointer() {
    int y = 0;
    int x = 0;
    //count the enters
    int lastEnter = 0;
    for(int i=0;i<pLoc;i++) {
        if(content.at(i) == '\n'){
            y++;
            lastEnter = i;
        }
    }
    x = pLoc - lastEnter;

    if(lastEnter != 0)
        x--;

    gotoxy(x,y);
}

void Editor::editMode() {
    char c = ' ';
    char prev = ' ';
    do{
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int row;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        row = csbi.srWindow.Bottom;

        prev = c;
        c = getch();
        system("cls");
        if(c == 8){
            // backspace
            if(pLoc > 0){

                content.erase(content.begin()+pLoc-1);
                pLoc--;
            }
        }
        else if(c == 13){
            //enter
            content.insert(content.begin()+pLoc,'\n');
            pLoc++;
        }
        else if(c == 27) {
            //escape
            return;
        }
        else if(c == 72) {
            //up
            if(pLoc > 0) {
                char currC = content.at(pLoc-1);
                if(currC == '\n' && pLoc > 0){
                    pLoc--;
                }
                else {
                    while(currC != '\n' && pLoc > 0){
                        if(pLoc == 1)
                            break;
                        pLoc--;
                        currC = content.at(pLoc-1);
                    }
                    if(pLoc > 0)
                        pLoc--;
                    }
            }
        }
        else if(c == 80 && prev == -32) {
            //down
            if(pLoc == 0)
                pLoc = 1;
            if(pLoc < content.size()) {
                char currC = content.at(pLoc-1);
                if(currC == '\n' && pLoc < content.size()){
                    pLoc++;
                    currC = content.at(pLoc-1);
                }
                while(currC != '\n' && pLoc < content.size()){
                    if(pLoc == content.size())
                        break;
                    pLoc++;
                    currC = content.at(pLoc-1);
                }
            }
        }
        else if(c == 75 && prev == -32) {
            //left
            if(pLoc > 0)
                pLoc--;
        }
        else if(c == 77 && prev == -32) {
            //right
            if(pLoc < content.size())
                pLoc++;
        }
        else if(c == 9){
            //tab
            content.insert(content.begin()+pLoc,' ');
            content.insert(content.begin()+pLoc,' ');
            content.insert(content.begin()+pLoc,' ');
            content.insert(content.begin()+pLoc,' ');
            pLoc+=4;
        }
        else if(c >= 32 && c <=126){
            //alphanumeric
            content.insert(content.begin()+pLoc,c);
            pLoc++;
        }
        gotoxy(0,0);
        cout << content << endl;
        gotoxy(0,row);
        cout << "EDIT MODE";
        showPointer();
    }while(c != 27);
    system("cls");
}

void Editor::openEditor() {
    //clear screen
    system("cls");
    if(content==""){
        cout << "\t\tWelcome to MSE(MyStupidEditor)" << endl;
        cout << "\n\n\n\t\tpress i to enter EDIT MODE" << endl;
        cout << "\t\tpress ESC to quit EDIT MODE" << endl;
        cout << "\t\tpress :q to quit" << endl;
    }
    else
        cout << content;
    char mode = ' ';
    char prev = ' ';
    char c = ' ';
    while(true) {
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        int row;

        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        row = csbi.srWindow.Bottom;

        prev = mode;
        mode = getch();
        system("cls");
        gotoxy(0,row);
        if(mode == ' '){
            cout << mode;
        }
        else if(mode == 'i') {
            cout << mode;
            editMode();
        }
        else if(mode == ':'){
            cout << mode;
        }
        else if(prev == ':' && mode == 'q'){
            cout << prev << mode;
            break;
        }
        else{
            cout << mode;
        }
        gotoxy(0,0);
        cout << content << endl;
        showPointer();
    }
}

int main() {

    Editor* e = new Editor();
    e->openEditor();

    delete e;

    return 0;
}
