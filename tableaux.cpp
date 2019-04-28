#include <bits/stdc++.h>
#include "bib/AnalyticTableaux.cpp"
using namespace std;

int findMainOperator(const string & proposition){

    int left_parentesis =0;
    int right_parentesis = 0;
    bool begin = true;

    //cout<<"SIZE = "<<proposition.size()<<endl;

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

string convert_expression(string &expression1, string &expression2){
    expression2.erase(0,1);
    expression2.pop_back();
    expression2.insert(1,"(");
    string expression;
    size_t pos;
    do{
        pos = expression2.find(',');
        //cout<<"I AM HERE"<< endl;
        //cout<<"expression2 = "<<expression2<<endl;
        if(pos != string::npos){
            //cout<<"IT IS NOT FAILLING IET"<<endl;
            //cout<<"pos = "<<pos<<endl;
            //expression2.insert(1,"(");
            expression = expression2.substr(pos+2,string::npos);
            expression2.insert(1,"(");
            size_t pos2 = expression.find(',');
            if(pos2!=string::npos)
                expression.insert(pos2,")");
            else
                expression.push_back(')');
            //expression+=" )";
            //cout<<"*"<<endl;
            expression2.erase(pos+1,string::npos);
            //cout<<"/"<<endl;
            expression2 += " & ";
            //cout<<'!'<<endl;
            expression2+=expression;
            //cout<<'+'<<endl;
            

        }

    }while(pos!=string::npos);


    
    expression2 += " & (~";
    expression2+=expression1;
    expression2.push_back(')');
    expression2.push_back(')');

    return expression2;

}

void convert_expression2(string & expression, Node &node){
   //cout<<"something"<<endl;
    expression.erase(0,1);
    expression.pop_back();
    //cout<<"!"<<expression<<"!"<<endl;
    int i=0;
    string aux;
    size_t pos;
    vector<Node*> insertedNodes;
    do{
        pos = expression.find(",");
        if(pos!=string::npos){
            aux = expression.substr(0,pos);
            expression.erase(0,pos+2);
            insertedNodes = node.insertFront(aux,true);
            if(insertedNodes[0]->checkContradiction()){
                insertedNodes[0]->markContradiction();
                return;
            }
        }
        //cout<<"aux ="<<' ';
        //cout<<aux<<endl;
        //cout<<"exp ="<<' ';
        //cout<<expression<<endl;
        //cout<<"tree "<<" ";
        
  
    }while(pos!=string::npos);
    insertedNodes = node.insertFront(expression,true);
    if(insertedNodes[0]->checkContradiction()){
                insertedNodes[0]->markContradiction();
                return;
    }
    //node->printTree();
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


vector<Node*>applyRule(Node *node){
        vector<Node*> insertedNodes;
        string expression = node->getExpression();
        string subExpression1, subExpression2;
        char op = divideExpression(expression,subExpression1,subExpression2);
        if(op == '~'){
            if(node->getTruthValue())
                insertedNodes = node->insertFront(subExpression1,false);
            else
                insertedNodes = node->insertFront(subExpression1,true);
        }
        else if(op == '&'){
            if(node->getTruthValue())
                insertedNodes = node->insertFront(subExpression1,true,subExpression2,true);
            else
                insertedNodes = node->insertSides(subExpression1,false,subExpression2,false);
        }
        else if(op == 'v'){
            if(node->getTruthValue())
                insertedNodes = node->insertSides(subExpression1,true,subExpression2,true);
            else
                insertedNodes = node->insertFront(subExpression1,false,subExpression2,false);
        }
        else if(op == '>'){
            if(node->getTruthValue())
                insertedNodes = node->insertSides(subExpression1,false,subExpression2,true);
            else
                insertedNodes = node->insertFront(subExpression1,true,subExpression2,false);
        }
        node->markApplied();
        if(node->checkContradiction())
            node->markContradiction();
        
        return insertedNodes;
}


vector<Node*>aplyRule(Node *node, string subExpression1,string subExpression2, char op){
        vector<Node*> insertedNodes;
        if(op == '~'){
            if(node->getTruthValue())
                insertedNodes = node->insertFront(subExpression1,false);
            else
                insertedNodes = node->insertFront(subExpression1,false);
        }
        else if(op == '&'){
            if(node->getTruthValue())
                insertedNodes = node->insertFront(subExpression1,true,subExpression2,true);
            else
                insertedNodes = node->insertSides(subExpression1,false,subExpression2,false);
        }
        else if(op == 'v'){
            if(node->getTruthValue())
                insertedNodes = node->insertSides(subExpression1,true,subExpression2,true);
            else
                insertedNodes = node->insertFront(subExpression1,false,subExpression2,false);
        }
        else if(op == '>'){
            if(node->getTruthValue())
                insertedNodes = node->insertSides(subExpression1,false,subExpression2,true);
            else
                insertedNodes = node->insertFront(subExpression1,true,subExpression2,false);
        }
        node->markApplied();
        if(node->checkContradiction())
            node->markContradiction();
        
        return insertedNodes;
}



void checkContradictions(vector<Node*> & insertedNodes){
    for(int i=0; i<insertedNodes.size(); i++){
        if(insertedNodes[i]->checkContradiction())
            insertedNodes[i]->markContradiction();
    }
}

bool checkDivergence(Node * node){
    string expression = node->getExpression();
    string subExpression1, subExpression2;
    char op = divideExpression(expression,subExpression1,subExpression2);
    if(op=='~')
        return true;
    else if(!(node->getTruthValue())){
        if(op== '>' || op == 'v')
            return true;
    }
    else if(node->getTruthValue()){
        if(op == '&')
            return true;
    }
        return false;
}





int main(){

    string expression, expression2,subExpression1, subExpression2, question;
    int counter = 0;
    ifstream Entrada("Entrada.in");
    int size;
    string str;

    if(Entrada.is_open()){
        getline(Entrada,str);
        while(getline(Entrada,expression)){
            //cout<<expression<<endl<<endl;
            cout<<"Problema #"<<++counter<<endl;
            question = separateQuestionAndExpression(expression, expression2);
            //cout<<"QUESTION is = "<<question<<endl;
            //cout<<"EXPRESSION is = "<<expression<<endl;
            //cout<<"EXPRESSION2 is = "<<expression2<<endl;

            
            Node tableaux(expression, false);
            if(question == "e satisfativel?")
                tableaux = Node(expression,true);
            else if( question == "e refutavel?")
                tableaux = Node(expression,false);
            else if( question == "e tautologia?")
                tableaux = Node(expression,false);
            else if(question == "e insatisfativel?")
                tableaux = Node(expression,true);
            else if(question == "e insatisfativel?")
                tableaux = Node(expression,true);
            else if(question == "e consequencia logica de"){
                //cout<<"something"<<endl;
                tableaux = Node(expression,false);
                convert_expression2(expression2,tableaux);
                //tableaux.printTree();
            }
            else
                cout<<"Sorry i've not understood your question, please try again."<<endl;
            
            vector<Node*> appliableNodes = tableaux.getAppliableNodes();
            vector <Node*> insertedNodes;
            Node* node;
            
            while(!tableaux.isClosed() && !appliableNodes.empty()){
                //tableaux.printTree();

                bool  notDiverge = false ;
                for(int i=0; i< appliableNodes.size() && !notDiverge; i++){
                    node = appliableNodes[i];
                    notDiverge = checkDivergence(node);
                }
                //cout<<"EXPRESSION = "<<node->getExpression()<<endl;
                insertedNodes = applyRule(node);
                checkContradictions(insertedNodes);
                appliableNodes = tableaux.getAppliableNodes();
                
            }
            if(question == "e satisfativel?")
                    tableaux.isClosed() ? cout<<"Nao, nao e satisfativel."<<endl : cout<<"Sim, e satisfativel."<<endl; 
            else if( question == "e refutavel?")   
                tableaux.isClosed() ? cout<<"Nao, nao e refutavel."<<endl : cout<<"Sim, e refutavel."<<endl;
            else if( question == "e tautologia?")
                tableaux.isClosed() ? cout<<"Sim, e tautologia."<<endl : cout<<"Nao, nao e tautologia."<<endl;
            else if(question == "e insatisfativel?") 
                tableaux.isClosed() ? cout<<"Sim, e insatisfativel."<<endl : cout<<"Nao, nao e insatisfativel."<<endl;
            else if(question == "e insatisfativel?")
                tableaux.isClosed() ? cout<<"Sim, e insatisfativel."<<endl : cout<<"Nao, nao e insatisfativel."<<endl;  
            else if(question == "e consequencia logica de")
                tableaux.isClosed() ? cout<<"Sim, e consequencia logica."<<endl : cout<<"Nao, nao e consequencia logica."<<endl;
                    
            else{
                cout<<"Sorry i've not understood your question, please try again."<<endl;
            }
cout<<endl;

        }
        Entrada.close();
    }

    
    
}
