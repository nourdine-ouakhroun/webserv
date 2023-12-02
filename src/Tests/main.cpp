#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// string  getContentFile(string file)
// {
//     fstream new_file;
    
//     new_file.open("vid.mp4", ios::in);
//     if (new_file.is_open() == false)
//         return ("");
//     string  sa;
//     string  content;
//     while (getline(new_file, sa))
//         content.append(sa).append("\n");
//     new_file.close();
//     return content;
// }

int main() {
    fstream new_file;
    fstream file;
    
    new_file.open("vid.mp4", ios::in);
    file.open("vid_copy.mp4", ios::out);
    
    string  sa;
    string  content;
    while (getline(new_file, sa))
        content.append(sa).append("\n");
    cout << content.size() << endl;
    file << content;
    new_file.close();
    file.close();
}
