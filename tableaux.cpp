#include <bits/stdc++.h>
#include "bib/AnalyticTableaux.cpp"
using namespace std;

//Função responsável por achar o operador principal de uma expressão
int findMainOperator(const string & proposition){

    int left_parentesis =0;
    int right_parentesis = 0;
    bool begin = true;


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

//Função responsável por tratar os problemas de consequência lógica, encontrando as expressões e as inserindo na árvore
void convert_expression2(string & expression, Node &node){
 
    expression.erase(0,1);
    expression.pop_back();
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
    
        
  
    }while(pos!=string::npos);
    insertedNodes = node.insertFront(expression,true);
    if(insertedNodes[0]->checkContradiction()){
                insertedNodes[0]->markContradiction();
                return;
    }
   
}

//Função resporsável por dividir a expressão recebida e retornar o operador principal
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

//Função responsável por separar a questão da expressão
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

//Função responsável por aplicar as regras e retornar os nós inseridos
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


//Função respónsável por checar se há contradição em algum dos nós inseridos
void checkContradictions(vector<Node*> & insertedNodes){
    for(int i=0; i<insertedNodes.size(); i++){
        if(insertedNodes[i]->checkContradiction())
            insertedNodes[i]->markContradiction();
    }
}

//Função responsável por verificar se aplicação da regra em determinado nó bifurca a árvore
bool checkDivergence(Node * node){
    string expression = node->getExpression();
    int op = findMainOperator(expression);
    if(expression.at(op)=='~')
        return true;
    else if(!(node->getTruthValue())){
        if(expression.at(op)== '>' || expression.at(op) == 'v')
            return true;
    }
    else if(node->getTruthValue()){
        if(expression.at(op) == '&')
            return true;
    }
        return false;
}





int main(){

    string expression, expression2,subExpression1, subExpression2, question;
    int counter = 0;
    ifstream Entrada("Entrada.in");
    ofstream Saida("Saida.out");
    int size;

    if(Entrada.is_open() && Saida.is_open()){
        Entrada>>size;
        Entrada.ignore();
        while(getline(Entrada,expression)){
         
            cout<<"Problema #"<<++counter<<endl;
	    Saida<<"Problema #"<<counter<<endl;
            question = separateQuestionAndExpression(expression, expression2);
     

            
            Node tableaux(expression, false);
            if(question == "e satisfativel?")
                tableaux = Node(expression,true);
            else if( question == "e refutavel?")
                tableaux = Node(expression,false);
            else if( question == "e tautologia?")
                tableaux = Node(expression,false);
            else if(question == "e insatisfativel?")
                tableaux = Node(expression,true);
            else if(question == "e consequencia logica de"){
                tableaux = Node(expression,false);
                convert_expression2(expression2,tableaux);
            }
            else
                cout<<"Sorry i've not understood your question, please try again."<<endl;
            
            vector<Node*> appliableNodes = tableaux.getAppliableNodes();
            vector <Node*> insertedNodes;
            Node* node;
            
            while(!tableaux.isClosed() && !appliableNodes.empty()){
                bool  notDiverge = false ;
		//Verifica se a regra que será aplicada bifurca a árvore bifurca árvore, e procura pelo nó que não irá bifurcar a árvore depois da aplicação da regra
		//Caso não encontre a regra será aplicada no último nó d
                for(int i=0; i< appliableNodes.size() && !notDiverge; i++){
                    node = appliableNodes[i];
                    notDiverge = checkDivergence(node);
                }
                insertedNodes = applyRule(node);
                checkContradictions(insertedNodes);
                appliableNodes = tableaux.getAppliableNodes();                
            }
            if(question == "e satisfativel?"){
                    tableaux.isClosed() ? cout<<"Nao, nao e satisfativel."<<endl : cout<<"Sim, e satisfativel."<<endl; 
		    tableaux.isClosed() ? Saida<<"Nao, nao e satisfativel."<<endl : Saida<<"Sim, e satisfativel."<<endl; 
	   }
            else if( question == "e refutavel?"){   
                tableaux.isClosed() ? cout<<"Nao, nao e refutavel."<<endl : cout<<"Sim, e refutavel."<<endl;
		tableaux.isClosed() ? Saida<<"Nao, nao e refutavel."<<endl : Saida<<"Sim, e refutavel."<<endl;
	    }
            else if( question == "e tautologia?"){
                tableaux.isClosed() ? cout<<"Sim, e tautologia."<<endl : cout<<"Nao, nao e tautologia."<<endl;
 		tableaux.isClosed() ? Saida<<"Sim, e tautologia."<<endl : Saida<<"Nao, nao e tautologia."<<endl;
	    }
 
            else if(question == "e insatisfativel?"){
                tableaux.isClosed() ? cout<<"Sim, e insatisfativel."<<endl : cout<<"Nao, nao e insatisfativel."<<endl; 
 		tableaux.isClosed() ? Saida<<"Sim, e insatisfativel."<<endl : Saida<<"Nao, nao e insatisfativel."<<endl; 
	    }
            else if(question == "e consequencia logica de"){
                tableaux.isClosed() ? cout<<"Sim, e consequencia logica."<<endl : cout<<"Nao, nao e consequencia logica."<<endl;
 		tableaux.isClosed() ? Saida<<"Sim, e consequencia logica."<<endl : Saida<<"Nao, nao e consequencia logica."<<endl;
	    }
                    
            else{
                cout<<"Sorry i've not understood your question, please try again."<<endl;
            }
    if(counter!=size){
	    cout<<endl;
	    Saida<<endl;
    }
        }
        Entrada.close();
	Saida.close();
    }

    
    
}
