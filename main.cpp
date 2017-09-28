/*
file name: main.cpp
This piece of code contains complete code for data parsing, Serialization and Deserialization.
NOTE: for exection instruction pleasee refere Readme.md file
@Author Tejas Patil
Date: 3 September 2017
Email: tejaspatil11@outlook.com
Web: www.tejaspatil.com
*/

#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <stdio.h>
#include <sstream>
#include <stdlib.h>

using namespace std;

//This is class names recoded which holds the values of type and amount form
//data.csv file

class records{
  public:
    string type;
    double amount;
  records(){
    type ="";
    amount =0;
    }
};

//This is a class which holds map which holds key val pair
//here key is datetime and vales are records structure which has two values
//one is transaction type and another is amount

class super{

  public:
    map<string, records*> data;
    map<string, records*> new_data;
};

//This function write all transaction ot new output file named output.csv
//Here all types of transaction are sorted with respect to datetime
//Parameters : it takes new_data map as a parameter and class object
//return values : void

void Final_Output_CSV(map<string, records*> new_data, super s1){

  map <string, records*>::iterator it;
  ofstream optfile;
  optfile.open("output.csv");

  for(it=s1.new_data.begin();it!=s1.new_data.end();it++){
        string outputf="";
        string tmp="";
        outputf.append(it->first);
        tmp=outputf.substr(5);
        tmp.insert(5,"/");
        tmp.insert(6,outputf.substr(0,4));
        double tmp2=it->second->amount;
        if(tmp2<0){
          optfile << tmp<< ","<<it->second->type <<","<< " " <<tmp2<< endl;;
        }
        else{
          optfile << tmp << ","<<it->second->type <<"," << "  " <<tmp2<< endl;;
        }
    }


}

/*--------------------------------------------------------------------------
function name Main();
in this function all step 1 , step 2 and step 3 takes place

Step1: Line No: 110 to 148
here it reads data from data,csv and parse all transaction to four different
maps with respect to TS TV TP TC and TF.

Step2:Line No: 132 to 188

here it loads data to five different object of super class which has data map.
After mapping values to data map it serializ objects of ts tv, tp ,tc and tf
into binary file named data.ts data.tv, data.tp ,data.tc and data.tf

Step3: Line Number: 201 to 291

Here it reads abjcts from the file data.ts, data.tv, data.tp ,data.tc and
data.tf to tsx, tvx, tpx ,tcx and tfx respectively
after getting objects form binary file it reads data with new objecta and
comine all types of transactions into single file names output.txt

---------------------------------------------------------------------------*/


int main(){

  long start_p=clock();
  super tc,tf,tp,ts,tv;
  super tcx,tfx,tpx,tsx,tvx;
  long start_s;
  super s1;
  string temp2;

  ifstream ip("data.csv");
  //opens data.csv
  map <string, records*>::iterator it;

  if(!ip.is_open()) std::cout << "ERROR: File Open" << '\n';

  string datetime;
  string type;
  string amt;
  string ipLine;

  while(!getline(ip,ipLine).eof()){

    stringstream line(ipLine);
    getline(line,datetime,',');
    getline(line,type,',');
    getline(line,amt,',');

    double amount = atof(amt.c_str());

    // reads data line by line and parse it into respective variables
    string temp = datetime.substr(6,4);
    temp.append("/");
    temp.append(datetime.substr(0,6));
    temp = temp.substr(0,10);
    temp.append(datetime.substr(10));

    // creates object of recordes and writes type and amuont to it
    records *rec = new records();

    rec->type = type;
    rec->amount = amount;
    start_s=clock();
    if(rec->type == " TC"){
      tc.data.insert(pair<string,records*>(temp,rec));
    }
    if(rec->type == " TV"){
      tv.data.insert(pair<string,records*>(temp,rec));
    }
    if(rec->type == " TP"){
      tp.data.insert(pair<string,records*>(temp,rec));
    }
    if(rec->type == " TS"){
      ts.data.insert(pair<string,records*>(temp,rec));
    }
    if(rec->type == " TF"){
      tf.data.insert(pair<string,records*>(temp,rec));
    }
  }

  ip.close();
  long stop_p=clock();
  long parsingTime = (stop_p - start_p)/double(CLOCKS_PER_SEC)*1000;


// serialization starts here
// it open files for each object and creates five different files
//---------------------------------------------------------------------------

  ofstream outc;
  outc.open("data.tc",std::ios::binary);
  outc.write((char*)(&tc), sizeof(tc));
  outc.close();

//---------------------------------------------------------------------------

  ofstream outv;
  outv.open("data.tv",std::ios::binary);
  outv.write(reinterpret_cast<char*>(&tv), sizeof(tv));
  outv.close();

//---------------------------------------------------------------------------

  ofstream outp;
  outp.open("data.tp",std::ios::binary);
  outp.write(reinterpret_cast<char*>(&tp), sizeof(tp));
  outp.close();

//---------------------------------------------------------------------------

  ofstream outs;
  outs.open("data.ts",std::ios::binary);
  outs.write(reinterpret_cast<char*>(&ts), sizeof(ts));
  outs.close();

//---------------------------------------------------------------------------

  ofstream outf;
  outf.open("data.tf",std::ios::binary);
  outf.write(reinterpret_cast<char*>(&tf), sizeof(tf));
  outf.close();

  // calculate serializationTime here
  long stop_s=clock();
  long serializationTime = (stop_s - start_s)/double(CLOCKS_PER_SEC)*1000;


//---------------------------------------------------------------------------
// Deserialization starts here
// it opens binary files and reads object form that


long start_d=clock();
std::ifstream inc;
inc.open("data.tc",std::ios::binary);
inc.read((char*)(&tcx), sizeof(tc));
inc.close();

std::ifstream inv;
inv.open("data.tv",std::ios::binary);
inv.read(reinterpret_cast<char*>(&tvx), sizeof(tv));
inv.close();

std::ifstream inp;
inp.open("data.tp",std::ios::binary);
inp.read(reinterpret_cast<char*>(&tpx), sizeof(tp));
inp.close();

std::ifstream ins;
ins.open("data.ts",std::ios::binary);
ins.read(reinterpret_cast<char*>(&tsx), sizeof(ts));
ins.close();

std::ifstream inf;
inf.open("data.tf",std::ios::binary);
inf.read(reinterpret_cast<char*>(&tfx), sizeof(tf));
inf.close();


// here once I get objet form binary file I can read transactions form it and
// pass it to new map names new_data
// as map sors all record accoing to key (datetime)


// reads all TC transactions

  it=tcx.data.begin();
  double profit_TC=0.0;
  for(unsigned int i=0 ;i<=tcx.data.size()-1;i++){
    records *rec2 = new records();
    rec2->type = it->second->type;
    rec2->amount = it->second->amount;;
    profit_TC+=rec2->amount;
    temp2 = it->first;
    s1.new_data.insert(pair<string,records*>(temp2,rec2));
    it++;
  }


// reads all TV transactions

  it=tvx.data.begin();
  double profit_TV=0.0;
  for(unsigned int i=0 ;i<=tvx.data.size()-1;i++){
    records *rec2 = new records();
    rec2->type = it->second->type;
    rec2->amount = it->second->amount;;
    profit_TV+=rec2->amount;
    temp2 = it->first;
    s1.new_data.insert(pair<string,records*>(temp2,rec2));
    it++;
  }


// reads all TP transactions

  it=tpx.data.begin();
  double profit_TP=0.0;
  for(unsigned int i=0 ;i<=tpx.data.size()-1;i++){
    records *rec2 = new records();
    rec2->type = it->second->type;
    rec2->amount = it->second->amount;
    profit_TP+=rec2->amount;
    temp2 = it->first;
    s1.new_data.insert(pair<string,records*>(temp2,rec2));
    it++;
  }

// reads all TS transactions

  it=tsx.data.begin();
  double profit_TS=0.0;
  for(unsigned int i=0 ;i<=tsx.data.size()-1;i++){
    records *rec2 = new records();
    rec2->type = it->second->type;
    rec2->amount = it->second->amount;;
    profit_TS+=rec2->amount;
    temp2 = it->first;
    s1.new_data.insert(pair<string,records*>(temp2,rec2));
    it++;
  }

  // reads all TF transactions

  it=tfx.data.begin();
  double profit_TF=0.0;
  for(unsigned int i=0 ;i<=tfx.data.size()-1;i++){
    records *rec2 = new records();
    rec2->type = it->second->type;
    rec2->amount = it->second->amount;;
    profit_TF+=rec2->amount;
    temp2 = it->first;
    s1.new_data.insert(pair<string,records*>(temp2,rec2));
    it++;

  }


  long stop_d=clock();
  long deSerializationTime = (stop_d - start_d)/double(CLOCKS_PER_SEC)*1000;
  Final_Output_CSV(s1.new_data,s1);



// Here I create and open new file named report.text and wirite all required
// report infromation

  ofstream ip1;
  ip1.open("report.txt");

  ip1 << "Step 1 Parsing Time : " << parsingTime << endl;
  ip1 << "Step 2 Serialization Time: " << serializationTime << endl;
  ip1 << "Step 3 Deserialization Time: " << deSerializationTime << endl<<endl;

  it = s1.new_data.begin();
  ip1 << "First Date of Record is: " << it->first << endl;

  it = s1.new_data.end();
  --it;
  ip1 << "Last Date of Record is: " << it->first << endl;

  ip1 << endl << "Total Profits for TS TV TP TC and TF" << endl <<
   "TS: " << profit_TS << endl <<
   "TV: " << profit_TV << endl <<
   "TP: " << profit_TP << endl <<
   "TC: " << profit_TC << endl <<
   "TF: " << profit_TF << endl;



}
