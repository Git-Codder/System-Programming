
#include<bits/stdc++.h>
#include<sstream>
using namespace std;

//I/O operation Functions
string getInput(int key ,int &sz){
	fstream fs;
    string file = " ";
    if(key==1){
        file = "input.txt";
    }
    else if(key==2){
        file = "input.pdf";
    }
    else if(key==3){
        file = "input.exe";
    }
    else {
        file = "input.zip";
    }
	fs.open(file, ios::in);
    sz = sizeof(fs)/8;
	string str = "";
	// str = stream_as_string(fs);
	while (fs.good())
        getline(fs, str); 
	fs.close();

	return str;
}

int main(int argc, char* argv[]){

    label :

    //taking input;
    int sz,key=-1;
    cout<<"First Choose the File Extenstion using number given below : "<<endl;
    cout<<"For Abort      file : 0"<<endl;
    cout<<"For input.text file : 1"<<endl;
    cout<<"For input.pdf  file : 2"<<endl;
    cout<<"For input.exe  file : 3"<<endl;
    cout<<"For input.zip  file : 4"<<endl;

    cin>>key;

    if(key<0 && key>4){
        key = -1;
    }

    if(key==-1){
        "Please Choose a file with right extenstion";
        goto label;
    }
    else if(key==0){
        return 0;
    }

    //getting input from file in string
    string str = getInput(key, sz);

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
    cout<<"======================="<<endl;

    //If user Choose wrong formate file
    if(ans==0){
        cout<<"You chooose wrong file formate, Please choose right "<<endl;
        cout<<"======================="<<endl;
        cout<<endl;
        goto label;
    }
    
    cout<<"File size in Byte : "<<sz<<endl;
    cout<<"Entropy : "<<ans<<endl;
    cout<<"======================="<<endl;
    cout<<endl;

    return 0;
}
