#include <bits/stdc++.h>
#include "bib/AnalyticTableaux.cpp"
using namespace std;

int findMainOperator(const string & proposition){

    int left_parentesis =0;
    int right_parentesis = 0;
    bool begin = true;

    cout<<"SIZE = "<<proposition.size()<<endl;

    for(int i=0; i<proposition.size(); i++){

        if(proposition.at(i) == '(')
            left_parentesis++;
        else{
            if(proposition.at(i) == ')')
                left_parentesis--;
            if(left_parentesis == 1){
                if(proposition.at(i)=='~')
                    return i;
                else
                    return i+2;
            }
        }
    }

    return -1;
  
}

char divideExpression(string &expression, string & subExpression1, string &subExpression2){

    int position = findMainOperator(expression);

    if(position != -1){
        if(expression.at(position) == '~'){
            subExpression1 = expression.substr(position+1, expression.size()-position-2);
            subExpression2.erase(0,string::npos);
            return '~';
        }
        else{
            subExpression1 = expression.substr(1,position-2);
            subExpression2 = expression.substr(position+2,expression.size()-position-3);
            return expression.at(position);
        }

        
    }


    return '\0';
}

string separateQuestionAndExpression(string &expression, string &expression2){
    
    int pos = expression.find('e');
    string question = expression.substr(pos,expression.size()-pos);
    expression.erase(pos-1,expression.size()-pos+1);

    size_t pos2 = question.find("de");

    if(pos2!=string::npos){
        expression2 = question.substr(pos2+3,question.size()-pos2-4);
        question.erase(pos2+2,question.size()-pos2);
    }

    return question;
}




int main(){

    string expression, expression2,subExpression1, subExpression2, question;
    int counter = 0;

    while(getline(cin,expression)){
        cout<<"Problema #"<<++counter<<endl;
        question = separateQuestionAndExpression(expression, expression2);

        int pos = findMainOperator(expression);

        cout<<expression<<endl;

        cout<<"pos = "<<pos<<endl;

        //char op = divideExpression(expression,subExpression1,subExpression2);
        //cout<<"SUBEXPRESSION 1 = "<<subExpression1<<endl;
        //cout<<"SUBEXPRESSION 2 = "<<subExpression2<<endl;
        //cout<<"op is "<<op<<endl;
    

        if(question == "e satisfativel?"){
            Node tableaux(expression,true);
            tableaux.printTree();

            tableaux.isClosed() ? cout<<"Nao, nao e satisfativel"<<endl : cout<<"Sim, e satisfativel"<<endl; 
        }
        else if( question == "e refutavel?"){
            Node tableaux(expression,false);

            tableaux.isClosed() ? cout<<"Nao, nao e refutavel"<<endl : cout<<"Sim, e refutavel"<<endl;
        }

        else if( question == "e tautologia?"){
            Node tableaux(expression,false);


            tableaux.isClosed() ? cout<<"Sim, e tautologia."<<endl : cout<<"Nao, nao e tautologia."<<endl;
            
        }
        else if(question == "e insatisfativel?"){
            Node tableaux(expression,true);
            tableaux.isClosed() ? cout<<"Sim, e insatisfativel."<<endl : cout<<"Nao, nao e insatisfativel."<<endl;
        }
        else if(question == "e insatisfativel?"){
            Node tableaux(expression,true);
            tableaux.isClosed() ? cout<<"Sim, e insatisfativel."<<endl : cout<<"Nao, nao e insatisfativel."<<endl;
        }
        else if(question == "e consequencia logica de"){
            
        }
        else{
            cout<<"Sorry i've not understood your question, please try again."<<endl;
        }

    }
    
}