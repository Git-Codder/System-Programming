
#include<bits/stdc++.h>
#include<sstream>
#include<errno.h>

#define colorPrefix "\033[1;"
#define colorSuffix "\033[0m"
#define Red 31
#define Magenta 35
#define Green 32
#define Blue 34

using namespace std;

//get colored text;
string getColored(string s,int col){
    return colorPrefix + to_string(col)+ "m" + s + colorSuffix;
}

//I/O operation Functions
string getInput(int &key ,int &sz, string file){
	fstream fs;
    string str = "";
    key = 1;
    
	fs.open(file, ios::in);
    if(!fs){
        key = -1;
        return str;
    }
    sz = sizeof(fs)/8;
	

	while (fs.good())
        getline(fs, str); 
	fs.close();

	return str;
}

int main(int argc, char* argv[]){

    label :

    //taking input;
    int sz,key=-1;
    cout<<getColored("For Exit Enter : exit ",Magenta)<<endl;
    cout<<getColored("Enter the File Name : ",Magenta);

    string fileName;
    cin>>fileName;
    
    if(fileName=="exit"){
        cout<<getColored("Exiting.....", Green)<<endl;
        cout<<endl;
        return 0;
    }

    // getting input from file in string
    string str = getInput(key, sz, fileName);

    //check the condition failure
    if(key==-1){
        cout<<endl;
        cout<<getColored("Oops! Something Went Wrong.", Red)<<endl;
        cout<<getColored("Write file name that exists in current directory.", Green)<<endl;
        cout<<endl;
        goto label;
    }

    //calculating frequency of each charator
    vector<int> freq(256,0);
    for(auto ch : str){
        freq[ch]++;
    }

    //calculating Entropy of file 
    double ans = 0.0;
    int len = str.length();
    for(auto i : freq){
        if(i==0)
            continue;
        double frequency = (double)i / len;
        ans -= frequency * (log(frequency)/log(2));
    }

    cout<<endl;
    cout<<getColored("=======================",Green)<<endl;
    cout<<getColored("File size in Byte : ",Blue)<<getColored(to_string(sz),Magenta)<<endl;
    cout<<getColored("Entropy : ",Blue)<<getColored(to_string(ans),Magenta)<<endl;
    cout<<getColored("=======================",Green)<<endl;
    cout<<endl;

    return 0;
}
