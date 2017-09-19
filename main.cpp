#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <list>

using namespace std;

struct palavra{
    string token;
    int ocorrencias;
    string lexema;
    palavra(string t, int o, string l):
        token(""),
        ocorrencias(0),
        lexema("")
    { token = t; ocorrencias = o; lexema = l;}
};

list<palavra> tabela;

string getLexema(string p){
    std::regex identifier("[a-z]([a-z]|[0-9])*"),
               num("[0-9]+");

    if(p == "if" || p == "else" || p == "then") return "CONDICIONAL";
    else if(p == "for" || p == "do" || p == "while") return "LOOP";
    else if(p == "+" || p == "-" || p == "/" || p == "*") return "OP_ARITMETICO";
    else if(p == ">" || p == "<" ||p == "!" || p == "=") return "OP_RELACIONAL";
    else if(p == ">=" || p == "<=" ||p == "!=" || p == "==") return "OP_RELACIONAL";
    else if(regex_match(p,num)) return "NUM";
    else if(p == "main" || p == "bool" || p == "unsigned" || p == "char" || p == "float" || p == "int" || p == "string") return "PALAVRA RESERVADA";
    else if(regex_match(p,identifier)) return "IDENTIFICADOR";
}

bool eDelimitador(char c){
    if(c == '(' || c == ')' || c == ',' || c == ' ' || c == ';' || c == '{' || c == '}' || c == '\t')
        return true;
    return false;
}

bool eOperadorLogico(char a, char b){
    if(b == '=' && (a == '<' || a == '>' || a == '!' || a == '='))
        return true;
    return false;
}

bool possuiOcorrencia(string s){
    for(list<palavra>::iterator j = tabela.begin(); j != tabela.end(); j++){
        palavra p = *(j);
        if(p.token == s){
            (*(j)).ocorrencias = p.ocorrencias + 1;
            return true;
        }
    }

    return false;
}

int main()
{
    ifstream arquivo("C:/Users/Isabela/texto.txt");
    string linha;

    list<char> palavras;
    list<string> operadores;

    if(arquivo.is_open()){
        while(getline(arquivo,linha)){
            for(int i = 0; i < linha.length(); i++){
                stringstream ss;
                ss << linha.at(i);
                string a;
                ss >> a;
                if(!eDelimitador(linha.at(i))){
                   if(getLexema(a) != "OP_ARITMETICO" && getLexema(a) != "OP_RELACIONAL"){
                       palavras.push_back(linha.at(i));
                   } else {
                       if(eOperadorLogico(linha.at(i), linha.at(i+1))){
                           string t;
                           stringstream s;
                           s << linha.at(i) << linha.at(i+1);
                           s >> t;
                           operadores.push_back(t);
                           i++;
                       } else operadores.push_back(a);
                       palavras.push_back(',');
                   }
                } else {
                    if(!possuiOcorrencia(a)){
                        string lexema;
                        if(linha.at(i) == ' ' || linha.at(i) == '\t')
                            lexema = "ESPAÇO";
                        else if(a == "(")
                            lexema = "AB_PARENTESE";
                        else if(a == ")")
                            lexema = "FC_PARENTESE";
                        else
                            lexema = "DELIMITADOR";
                        palavra d(a,1,lexema);
                        tabela.push_back(d);
                    }
                    palavras.push_back(',');
                }
            }
        }        
    }

    arquivo.close();

    string a = "";

    for(list<char>::iterator j = palavras.begin(); j != palavras.end(); j++){
        char c = *(j);
        if(c != ',') a+=c;
        else{
            if(!possuiOcorrencia(a)){
                palavra d(a,1,getLexema(a));
                tabela.push_back(d);
            }
            a = "";
        }
    }

    for(list<string>::iterator j = operadores.begin(); j != operadores.end(); j++){
        string a = *(j);
        if(!possuiOcorrencia(a)){
            string lexema;
            if(a == "=")
                lexema = "ATRIBUICAO";
            else
                lexema = getLexema(a);
            palavra d(a,1,lexema);
            tabela.push_back(d);
        }
        a = "";
    }

    std::ofstream saida;
        saida.open("C://Users/Isabela/saida.txt",
                                       std::ios::out|std::ios::app);
        saida << " Token\t| Ocorrências\t| Lexema\t" << endl;

        for(list<palavra>::iterator i = tabela.begin();i != tabela.end();i++){
                palavra p = *(i);
                saida << "  " << p.token << "\t|  "<<p.ocorrencias << "\t\t|  "<< p.lexema << endl;
        }
    saida.close();
}
