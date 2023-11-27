#include <iostream>
#include <fstream>
#include <string>

using namespace std;

string  getContentFile(string file)
{
    fstream new_file;
    
    new_file.open("banner.png", ios::in);
    if (new_file.is_open() == false)
        return ("");
    string  sa;
    string  content;
    while (getline(new_file, sa))
        content.append(sa).append("\n");
    new_file.close();
    return content;
}

int main() {
    fstream new_file;
    
    new_file.open("banner.png", ios::in);
    
    string  sa;
    string  content;
    while (getline(new_file, sa))
        content.append(sa).append("\n");
    new_file.close();
}
