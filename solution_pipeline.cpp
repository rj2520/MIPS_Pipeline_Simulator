#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <chrono>
#include <thread>



using namespace std;

class solution {
    
private:
    
    bool debug;
    int clck;
    int instr_num = 0;
    bool exec = true;
    int fetch_instr = -1;
    int exe_instr = -1;
    int wb_instr = -1;
    
    
    vector<string> vect_lines;
    vector<int>* t_vars = new vector<int>();
    
    string read_line;
    string temp_string;
    unordered_map< string, int> label_map;
    unordered_map<int, string > instr_map;
    
    public :
    
    solution(ifstream &file_in,int clck_in = 10 ,bool DEBUG_in = false){
        getline(file_in, read_line);
        istringstream ss(read_line);
        string token;
        while(getline(ss, token, ',')){
            t_vars->push_back(stoi(token));
        }
        int i = 0;
        
        while(file_in){
            getline(file_in, read_line);
            if(read_line.substr(0,5)== "label"){
                unsigned long pos = read_line.find(" ",0);
                label_map[read_line.substr(0,pos)] = i;
                instr_map[i] = read_line.substr(pos+1);
            }
            else{
                instr_map[i] = read_line;
            }
            //cout << instr_map[i] << endl;
            i++;
            
        }
        
        
        
    }
    void dbg(const string &msg);
    
    void instr_handle(string instr, int &instr_num, bool &exec, vector<int> *t_vars,
                      unordered_map<string, int> label_map){
        unsigned long pos = instr.find(" ",0);
        int a, b, c;
        //static int dep_reg = 9;
        static int num_cyc = -1;
        
        string op = instr.substr(0,pos);
        if(op == "add"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            c = stoi(instr.substr(pos+8,1));
            
            (*t_vars)[a] = (*t_vars)[b]+(*t_vars)[c];
            instr_num ++;
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "addi"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            c = stoi(instr.substr(pos+7));
            (*t_vars)[a] = (*t_vars)[b]+c;
            instr_num ++;
            
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "sub"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            c = stoi(instr.substr(pos+8,1));
            (*t_vars)[a] = (*t_vars)[b]-(*t_vars)[c];
            instr_num ++;
            
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "mul"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            c = stoi(instr.substr(pos+8,1));
            (*t_vars)[a] = (*t_vars)[b]*(*t_vars)[c];
            instr_num ++;
            
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "div"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            c = stoi(instr.substr(pos+8,1));
            (*t_vars)[a] = (*t_vars)[b]/(*t_vars)[c];
            instr_num ++;
            
            num_cyc ++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "b"){
            instr_num = label_map[instr.substr(pos+1)];
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = -1;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
            
            num_cyc++;
            wb_instr = -1;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
            
            
        }
        
        if(op == "beq"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            
            if((*t_vars)[a] == (*t_vars)[b]){
                instr_num = label_map[instr.substr(pos+7)];
            }else{
                instr_num++;
            }
            
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = -1;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
            
            num_cyc++;
            wb_instr = -1;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
        }
        
        if(op == "bnq"){
            a = stoi(instr.substr(pos+2,1));
            b = stoi(instr.substr(pos+5,1));
            
            if((*t_vars)[a] == (*t_vars)[b]){
                instr_num++;
            }else{
                instr_num = label_map[instr.substr(pos+7)];
            }
            
            num_cyc++;
            
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = -1;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            cout << endl;
//            print_register();
            
            num_cyc++;
            wb_instr = -1;
            exe_instr = fetch_instr;
            fetch_instr = instr_num;
            
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);

            cout << endl;
//            print_register();
        }
        
        if(op == "end"){
            cout<< "end cycle" << endl;
            num_cyc ++;
            wb_instr = exe_instr;
            exe_instr = fetch_instr;
            fetch_instr = -1;
            printOut(fetch_instr, exe_instr, wb_instr, num_cyc);
            
            exec = false;
        }
        
    }
    
    vector<int>* alu(){
        while(exec){
            if(!mips_clock()){
                continue;
            }else{
                string instr = instr_map[instr_num];
                instr_handle(instr, instr_num, exec, t_vars,label_map);
            }
        }
        
        return t_vars;
    }
    
    void printOut(int fe_instr, int exe_instr, int wb_instr, int num_cyc){
        if(fe_instr != -1 || exe_instr != -1 || wb_instr != -1){
          cout << "number of cycle: " << num_cyc << endl;
        }
        
        if(fe_instr != -1){
            cout << "Fetch: "<< instr_map[fe_instr]<< endl;
        }
        if(exe_instr != -1){
            cout << "Execute: "<< instr_map[exe_instr]<< endl;
        }
        if(wb_instr != -1){
            cout << "Write_back: " << instr_map[wb_instr] << endl;
        }
        
    }
    
    int mips_clock(){
        chrono::milliseconds timespan(clck);
        
        this_thread::sleep_for(timespan);
        static int cycle = 0;
        if (cycle == 0 )
            cycle = 1;
        else
            cycle = 0;
        return cycle;
    };
    
    void print_register(){
        for(int i=0;i<(*t_vars).size();i++){
            cout << (*t_vars)[i] << " ";
        }
        cout << endl;
        cout << endl;
    }
};


