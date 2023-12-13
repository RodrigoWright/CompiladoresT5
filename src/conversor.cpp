#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
using namespace std;

typedef struct AutomatoFinito {
    vector<string> estados; //Estados do Automato Finito
    vector<string> transicoes; //Tabela de transições do Automato finito
    vector<string> estadosFinais; // Estados finais do Automato finito
    vector<string> simbolos; //Simbolos de transição
    string estadoInicial; // Estado inicial do automato finito
}AutomatoFinito;

AutomatoFinito Escrita (AutomatoFinito AFN){
    int qtdE, qtdT, qtdS, qtdEF; // Quantidade de Estados no Automato, e Quantidade de Transições no automato e quantidade de símbolos;
    string tmp; // String para alocação temporária de dados
    cout << "Quantidade de Estados:" << endl;
    cin >> qtdE;
    cout << "Quantidade de Estados Finais:" << endl;
    cin >> qtdEF;
    cout << "Quantidade de Transições:" << endl;
    cin >> qtdT;
    cout << "Quantidade de Símbolos no Alfabeto:" << endl;
    cin >> qtdS;

    cout << "Alfabeto:" << endl;
    for (int i = 0; i < qtdS; i++){
        cin >> tmp;
        AFN.simbolos.push_back(tmp);
    }

    cout << "Estado inicial:" << endl;
    cin >> AFN.estadoInicial;

    cout << "Estado Final:" << endl;
    for (int i = 0; i < qtdEF; i++){
        cin >> tmp;
        AFN.estadosFinais.push_back(tmp);
        AFN.estados.push_back(tmp);
    }

    cout << "Outros estados:" << endl;
    for(int i = 0; i < qtdE-qtdEF-1 ; i++){
        cin >> tmp;
        AFN.estados.push_back(tmp);
    }

    cout << "Transições:" << endl << "E.g: q0,q1,a" << endl;
    for (int i = 0; i < qtdT; i ++){
        bool valido = true; // Determina se a entrada é válida ou não
        string token;        
        cin >> tmp;
        istringstream tmp2(tmp);
        
        while(getline(tmp2, token , ',')){
            if(token != AFN.estadoInicial )
                if (find(begin(AFN.estados), end(AFN.estados), token) == end(AFN.estados))
                    if (find(begin(AFN.simbolos), end(AFN.simbolos), token) == end (AFN.simbolos)) valido = false;
        }
        if (valido) AFN.transicoes.push_back(tmp);
        else {
            cout << "Valor inválido" << endl;
            i--;  
        }                     
    }
    return AFN;
}

void LerAutomato (AutomatoFinito AF){
    cout << "--------------------" << endl;
    cout << "Estado Inicial: " << endl << AF.estadoInicial << endl;
    cout << "Estados Finais (F):" << endl;
    for (int i = 0; i < AF.estadosFinais.size(); i ++){
        cout << AF.estadosFinais[i] << endl;
    }
    cout << "Estados:" << endl;
    for (int i = 0; i < AF.estados.size(); i++){
        cout << AF.estados[i] << endl;
    }
    cout << "Alfabeto:" << endl;
    for (int i = 0; i < AF.simbolos.size(); i++){
        cout << AF.simbolos[i] << endl;
    }
    cout << "Transições:" << endl;
    vector<string> usadas; //saidas já imprimidas (para evitar repetição)
    for (string t : AF.transicoes){
        string qi, qj, s;
        istringstream tmp(t);
        getline(tmp, qi, ',');
        getline(tmp, qj, ',');
        getline(tmp, s);
        if (find(begin(AF.estadosFinais), end(AF.estadosFinais), qi) != end(AF.estadosFinais)) qi = "F";
        if (find(begin(AF.estadosFinais), end(AF.estadosFinais), qj) != end(AF.estadosFinais)) qj = "F";
        string print = qi + ',' + qj + ',' + s;
        if (find(begin(usadas), end(usadas), print) == end(usadas)){
            cout << print << endl;
            usadas.push_back(print); 
        }       
    }
}

AutomatoFinito thompson (AutomatoFinito AFN){
    vector<string> workList; //Lista de transições em e(epsilon, vazio)
    for (string e : AFN.transicoes){
        if (e.back() == 'e') workList.push_back(e);
    }
    while (!workList.empty()){
        string e; //Transição da worklist que será usada.
        e = workList[0]; //Pega a primeira transição da work list, formato "qo,q1,e"
        workList.erase(workList.begin()); //remove 'e' da worklist

        if (e.back() == 'e'){
            string qi; //Estado inicial da transição e
            string qj; // Estado Final da transição e
            string ql; //Estado inicial da transição t
            string qk; //Estado Final da transição t
            istringstream tmp(e); //Alocação temporária da transição e
            string add; //transição que será adicionada a worklist

            getline(tmp, qi, ',');
            getline(tmp, qj, ',');
            vector<string> auxTransicoes = AFN.transicoes;
            for(string t : auxTransicoes){ //t é uma transição da lista de transições do automato
                char transita = t.back(); //Transição em x;
                istringstream tmp(t); //Alocação temporária da transição t

                getline(tmp, ql, ','); // pega o estado de onde a transição saí
                getline(tmp, qk, ','); // pega o estado final da transição

                if (ql == qj){
                    add = qi + "," + qk + "," + transita;
                    cout << add << endl << qi << "," << qj << endl << ql << "," << qk <<endl;
                    AFN.transicoes.push_back(add); //Adiciona nova transição a lista de transições
                    if (transita == 'e') workList.push_back(add); // Se a transição resultante for em epsilon, adiciona a worklist
                }
                
            }
            if (find(begin(AFN.estadosFinais), end(AFN.estadosFinais), qj) != end(AFN.estadosFinais) || find(begin(AFN.estadosFinais), end(AFN.estadosFinais), qk) != end(AFN.estadosFinais)) {
                if (find(begin(AFN.estadosFinais), end(AFN.estadosFinais), qi) == end(AFN.estadosFinais)) {
                    AFN.estadosFinais.push_back(qi); // Se um dos estados da transição criada for final, adiciona o estado qi a lista de estados finais.
                }
            }
            add = qi + "," + qj + "," + e.back();
            AFN.transicoes.erase(find(begin(AFN.transicoes), end(AFN.transicoes), add)); //Apaga a transição antiga da lista de transições
        }
        
    }
    bool change = true; //determina se ouve mudança na lista de estados;
    while (change){ // Este while apaga todos os estados que são inacecíveis dado qualquer input
        change = false;
        vector<string> estados = AFN.estados; //Alocação temporária da lista de estados do automato
        for (string e : estados){
            bool temTransicao = false;
            for (string t : AFN.transicoes){
                string aux;
                istringstream tmp(t); //Alocação temporária da transição t

                getline(tmp, aux, ',');
                getline(tmp, aux, ',');

                if (e == aux){
                    temTransicao = true;                    
                    break;
                }
            }
            if (!temTransicao) {
                cout << "oi" << e << endl;
                AFN.estados.erase(find(begin(AFN.estados), end(AFN.estados), e));
                while(find(begin(AFN.estadosFinais), end(AFN.estadosFinais), e) != end(AFN.estadosFinais)){
                    AFN.estadosFinais.erase(find(begin(AFN.estadosFinais), end(AFN.estadosFinais), e));
                }
                vector<string> transicoes = AFN.transicoes;
                for (string t : transicoes){
                    string aux;
                    istringstream tmp(t);

                    getline(tmp, aux, ',');

                    if (aux == e) AFN.transicoes.erase(find(begin(AFN.transicoes), end(AFN.transicoes), t));
                }
                // cout << "Sem transição inicial: " << e << endl;
                change = true;
            }
        }
    }
    AFN.simbolos.erase(find(begin(AFN.simbolos), end(AFN.simbolos), "e"));
    return AFN;
}

AutomatoFinito AFNtoAFD (AutomatoFinito AFN){
    return AFN;
}


int main (){   
    AutomatoFinito AFNe; //Definição do Automato Finito Não determinístico com transições em epsilon
    AutomatoFinito AFN; //Automato Finito Nao determinístico sem transições em epsilon
    AutomatoFinito AFD; //Automato finito Determinístico
    AutomatoFinito minAFD; //Automato finito determinístico minimizado

    AFNe = Escrita(AFNe); // Escreve o AFNe com base no input do usuário
    LerAutomato (AFNe);
    cout << "----- thompson -------" << endl; 
    AFN = thompson(AFNe);
    LerAutomato (AFN);
    cout << "----- AFN > AFD ------" << endl;
    cout << "-- Minimização AFD ---" << endl;
    cout << "------ saída ---------" << endl;
    

    bool exit = false; //Determina se a opc digitada pelo usuário é valida ou não.
    while (!exit)
    {
        char opc;
        cout << "Deseja continuar? (s/n)" << endl;
        cin >> opc;
        if (opc == 'n') return 0;
        else if (opc == 's') {
            exit = true;
            main();
        }
        else exit = false;
    }
    return 0;
}