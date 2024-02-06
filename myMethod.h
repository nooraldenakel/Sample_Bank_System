//
// Created by PC on 1/25/2024.
//

#ifndef MYMETHOD_H
#define MYMETHOD_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace MyMethod {
    string toLowarString(string txt) {
        string word;
        for (int i = 0; i < txt.length(); i++) {
            word += tolower(txt[i]);
        }
        return word;
    }

    string readString() {
        cout << "Enter Txt ?";
        string txt;
        getline(cin, txt);
        return txt;
    }

    vector<string> split(string txt, string seperator) {
        vector<string> vText;
        int pos;
        string word;
        while ((pos = txt.find(seperator)) != std::string::npos) {
            word = txt.substr(0, pos);
            if (word != " ")
                vText.push_back(word);
            txt.erase(0, pos + seperator.length());
        }
        if (txt != " ")
            vText.push_back(txt);
        return vText;
    }

    string join(vector<string> vText, string seperator) {
        string words;
        for (string &word: vText) {
            words += word + seperator;
        }
        return words.substr(0, words.length() - seperator.length());
    }

    string join(string arrayText[], int lenght, string seperator) {
        string words;
        for (int i = 0; i < lenght; i++) {
            words += arrayText[i] + seperator;
        }
        return words.substr(0, words.length() - seperator.length());
    }

    string reverse(string txt) {
        vector<string> vText = split(txt, " ");
        vector<string>::iterator iter = vText.end();
        string word;
        while (iter != vText.begin()) {
            --iter;
            word += *iter + " ";
        }
        return word.substr(0, word.length() - 1);
    }

    string replaceWordInVector(string txt, string newWord, string oldWord) {
        vector<string> vText = split(txt, " ");
        for (string &word: vText) {
            if (toLowarString(word) == toLowarString(oldWord))
                word = newWord;
        }
        return join(vText, " ");
    }

    string replaceWordInString(string txt, string newWord, string oldWord) {
        int pos = txt.find(oldWord);
        while (pos != std::string::npos) {
            txt.replace(pos, oldWord.length(), newWord);
            pos = txt.find(oldWord);
        }
        return txt;
    }

    int readNumber(int from, int to) {
        printf("Choise Srevice from %d-%d : ", from, to);
        short number;
        cin >> number;
        while (number < from || number > to) {
            cout << "Wrong Choise!! return Choise from [1-7] : ";
            cin >> number;
        }
        return number;
    }

    bool isPunc(char c) {
        for (int i = 32; i < 172; i++) {
            if ((i > 32 && i < 48) || (i > 57 && i < 65) || (i > 90 && i < 97)) {
                if (c == char(i))
                    return true;
            }
        }
        return false;
    }

    string removePunct(string txt) {
        string word;
        for (int i = 0; i < txt.length(); i++) {
            if (!isPunc(txt[i])) {
                word += txt[i];
            }
        }
        return word;
    }

    string removePuncFormWord(string txt) {
        vector<string> vText = split(txt, " ");
        for (string &word: vText) {
            word = removePunct(word);
        }
        return join(vText, " ");
    }
}


#endif //MYMETHOD_H
