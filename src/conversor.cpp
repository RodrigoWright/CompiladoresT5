#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <math.h>
#include <cmath>
#include <iterator>
using namespace std;

typedef struct AutomatoFinito {
    vector<string> estados; //Estados do Automato Finito
    vector<string> transicoes; //Tabela de transições do Automato finito
    vector<string> estadosFinais; // Estados finais do Automato finito
    vector<string> simbolos; //Simbolos de transição
    vector<string> estadosRepresentados; //Estados referentes aos estados antigos
    string estadoInicial; // Estado inicial do automato finito
}AutomatoFinito;
typedef struct listaEncadeada {
    string value; //Valor referente x
    string value0; //valor referente y
    listaEncadeada *prox;
} listaEncadeada;

AutomatoFinito Escrita (AutomatoFinito AFN){ //Recebe Input do usuário
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
        if (tmp != AFN.estadoInicial)AFN.estados.push_back(tmp);
        else qtdE++;
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
            cout << "Valor inválido:" << tmp <<endl;
            i--;  
        }                     
    }
    return AFN; //Retorna o Input do usuário
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
        
        cout << AF.estados[i];
        if (begin(AF.estadosRepresentados) != end(AF.estadosRepresentados)){
            cout << " = {" << AF.estadosRepresentados[i] << "}" <<endl;
        } else cout << endl;
    }
    cout << "Alfabeto:" << endl;
    for (int i = 0; i < AF.simbolos.size(); i++){
        cout << AF.simbolos[i] << endl;
    }
    cout << "Transições:" << endl;
    vector<string> usadas; //saidas já imprimidas (para evitar repetição)
    // for (string t : AF.transicoes){
    //     string qi, qj, s;
    //     istringstream tmp(t);
    //     getline(tmp, qi, ',');
    //     getline(tmp, qj, ',');
    //     getline(tmp, s);
    //     if (find(begin(AF.estadosFinais), end(AF.estadosFinais), qi) != end(AF.estadosFinais)) qi = "F";
    //     if (find(begin(AF.estadosFinais), end(AF.estadosFinais), qj) != end(AF.estadosFinais)) qj = "F";
    //     string print = qi + ',' + qj + ',' + s;
    //     if (find(begin(usadas), end(usadas), print) == end(usadas)){
    //         cout << print << endl;
    //         usadas.push_back(print); 
    //     }       
    // }
    for (string t : AF.transicoes){
        cout << t << endl;
    }
}

vector<vector<string>> generateTable (AutomatoFinito AF){ //Gera tabela de transição
    vector<string> normal (AF.simbolos.size(), "");
    vector<vector<string>> tabelaTransicao(AF.estados.size()+1, normal); //Tabela de Transições do automato
    for(string transicao : AF.transicoes){
        string qi, qj, simbolo; //Estados que serão transitados e o simbolo
        int posicaoI, posicaoJ, posicaoS; //Refere a posição na tabela de estados;
        istringstream tmp(transicao);
        getline(tmp, qi, ',');
        getline(tmp, qj, ',');
        getline(tmp, simbolo);
        //Busca Pelas posições dos estados e simbolos de alfabeto
        if(qi == AF.estadoInicial) posicaoI = -1;
        else for (posicaoI = 0; posicaoI < AF.estados.size(); posicaoI++) if (AF.estados[posicaoI] == qi) break;

        if(qj == AF.estadoInicial) posicaoJ = -1;
        else for (posicaoJ = 0; posicaoJ < AF.estados.size(); posicaoJ++) if (AF.estados[posicaoJ] == qj) break;

        for (posicaoS = 0; posicaoS < AF.simbolos.size(); posicaoS++) if (AF.simbolos[posicaoS] == simbolo) break;
        
        //Adiciona qj à tabela de transições        
        istringstream tmp2(tabelaTransicao[posicaoI+1][posicaoS]);        
        getline(tmp2, qi, ',');       
        while(qi != "\0"){
            if(qi == qj){
                qj = "-1";
                break;
            }
            getline(tmp2, qi, ',');
        }
        if (qj != "-1") tabelaTransicao[posicaoI+1][posicaoS] += qj + ",";  
    }
    return tabelaTransicao;
}

AutomatoFinito removetrasitions(AutomatoFinito AF, vector<string> transitionsSet){ //Remove transições inalcansáveis
    bool change = true; //determina se ouve mudança na lista de estados;
    while (change){ // Este while apaga todos os estados que são inacecíveis dado qualquer input
        change = false;
        vector<string> estados = AF.estados; //Alocação temporária da lista de estados do automato
        for (string e : estados){
            bool temTransicao = false;
            if (e != AF.estadoInicial){
                for (string t : transitionsSet){ //Transição t da lista de transições
                    string aux;
                    istringstream tmp(t); //Alocação temporária da transição t

                    getline(tmp, aux, ',');
                    if (e == aux){
                        temTransicao = false;
                    } else {
                        getline(tmp, aux, ',');

                        if (e == aux){
                            temTransicao = true;                    
                            break;
                        }
                    }
                    
                }
            } else temTransicao = true;
            if (!temTransicao) {
                cout << "Inalcansável: " << e << endl;
                vector<string>::iterator ptr; 
                ptr = find(begin(AF.estados), end(AF.estados), e);
                int remove = ptr - AF.estados.begin();
                AF.estados.erase(ptr);
                if (begin(AF.estadosRepresentados) != end(AF.estadosRepresentados))
                    AF.estadosRepresentados.erase(find(begin(AF.estadosRepresentados), end(AF.estadosRepresentados), AF.estadosRepresentados[remove]));
                while(find(begin(AF.estadosFinais), end(AF.estadosFinais), e) != end(AF.estadosFinais)){
                    AF.estadosFinais.erase(find(begin(AF.estadosFinais), end(AF.estadosFinais), e));
                }
                vector<string> transicoes = transitionsSet;
                for (string t : transicoes){
                    string aux;
                    istringstream tmp(t);

                    getline(tmp, aux, ',');

                    if (aux == e) transitionsSet.erase(find(begin(transitionsSet), end(transitionsSet), t));
                }
                // cout << "Sem transição inicial: " << e << endl;
                change = true;
            }
        }
    }
    AF.transicoes = transitionsSet;
    return AF;
}

AutomatoFinito thompson (AutomatoFinito AFN){ //Executa o algorítimo de thompson
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
    AFN = removetrasitions(AFN, AFN.transicoes);
    if (find(begin(AFN.simbolos), end(AFN.simbolos), "e") != end(AFN.simbolos)) 
        AFN.simbolos.erase(find(begin(AFN.simbolos), end(AFN.simbolos), "e"));
    return AFN;
}

void generateCombinations(const vector<string>& elements, vector<string>& current, int index, vector<string>& result) {//Gera combinações do vetor
    if (index == elements.size()) {
        string combination;
        for (int i = 0; i < current.size(); ++i) {
            combination += current[i];
            if (i < current.size() - 1) {
                combination += ",";
            }
        }
        result.push_back(combination);
        return;
    }

    // Inclui o elemento atual na combinação
    current.push_back(elements[index]);
    generateCombinations(elements, current, index + 1, result);
    current.pop_back();

    // Não inclui o elemento atual na combinação
    generateCombinations(elements, current, index + 1, result);
}

AutomatoFinito AFNtoAFD (AutomatoFinito AFN){ //Converte um AFN para um AFD
    vector<vector<string>> tabelaTransicao = generateTable(AFN); //Tabela de Transições do automato
    
    //-------Tabela de transição AFN---------------
    cout << "Tabela de transição AFN:" << endl;
    for (int i = 0; i < AFN.estados.size()+1; ++i) {
        for (int j = 0; j < AFN.simbolos.size(); ++j) {
            cout << tabelaTransicao[i][j] << "\t";
        }
        cout << endl;
    }
    // ---- Tabela de conversão ----
    int sizeEstados = (int) pow(2.0, AFN.estados.size() + 1); //Número de combinações possíveis (incluindo vazio)
    string tabelaConversao[sizeEstados-1][AFN.simbolos.size()]; //Tabela que gerará novas transições

    vector<string> v = AFN.estados; //Vetor para alocação temporária de estados
    v.push_back(AFN.estadoInicial);
    vector<string> current;
    vector<string> result; //Lista de novos estados
    generateCombinations(v, current, 0, result); //Gera todas as combinações possíveis dos estados
    
    for(int i = 0; i < sizeEstados-1; i++){
        istringstream tmp(result[i]);
        string qi; //novo estado inicial
        while(getline(tmp, qi, ',')){
            int posicaoI; //Posição de qi na tabela estados
            if(qi == AFN.estadoInicial) posicaoI = -1;
            else for (posicaoI = 0; posicaoI < AFN.estados.size(); posicaoI++) if (AFN.estados[posicaoI] == qi) break;
            posicaoI++;
            for(int j = 0; j < AFN.simbolos.size(); j++){
                bool repetido = false; //diz se o estado já foi adicionado
                string qj, qk; //Estado que será transitado
                istringstream tmp2(tabelaTransicao[posicaoI][j]); //Tabela de transição na posição i j        
                while(getline(tmp2, qk, ',')){
                    istringstream tmp3(tabelaConversao[i][j]);//Tabela de converção na posição i j
                    while(getline(tmp3, qj, ',')){
                        if (qk != qj) repetido = false;
                        else {
                            repetido = true;
                            break;
                        }
                    }
                    if (repetido == false) tabelaConversao[i][j] += qk + ',';
                }
            }
        }
    }
    
    AFN.estados = result; //Nova tabela de estados
    AFN.estados.pop_back(); //Remove o estado vazio do final da lista
    AFN.estadosRepresentados = AFN.estados; //Atribuição dos estados sendo representados
    vector<string> newFinais; //Nova lista de estados Finais
    string antigoInicial;
    for (int i = 0; i < AFN.estados.size(); i++){
        istringstream tmp(result[i]); //Alocação temporária do result
        istringstream inicial(result[i]); //Alocação temporária do result        
        string qi;
        getline(inicial, qi);
        if (qi == AFN.estadoInicial){
            AFN.estadoInicial = "S" + to_string(i);
            antigoInicial = qi;
        }
        while(getline(tmp, qi, ',')){
            if(find(begin(AFN.estadosFinais), end(AFN.estadosFinais), qi) != end(AFN.estadosFinais)){
                newFinais.push_back("S" + to_string(i));
                break;
            }
        }
        AFN.estados[i] = "S" + to_string(i); //Altera o nome do novo estado
    }
    AFN.estadosFinais = newFinais;
    vector<string> newTransicoes; // nova tabela de transições
    for (int x = 0; x < AFN.estados.size(); x++){ //Deslocamento na matriz, vetor x e y;
        for(int y = 0; y < AFN.simbolos.size(); y++){
            if (tabelaConversao[x][y] != "\0") {
                tabelaConversao[x][y].pop_back();
                
                for(int i = 0; i < result.size()-1; i++){
                    istringstream tabela(tabelaConversao[x][y]);
                    istringstream res(result[i]);
                    string qi, qj;
                    bool equal = false; //Determina se os estados são iguais
                    vector<string> arrayres;
                    int count = 0; //Tamanho da tabela
                    while (getline(res, qj, ',')){
                        arrayres.push_back(qj);
                    }
                    while(getline(tabela, qi, ',')){
                        equal = false;
                        count++;
                        for(int n = 0; n < arrayres.size(); n++){
                            if (arrayres[n] == qi) {
                                equal = true;
                                break;
                            }
                        }
                        if(equal == false) break;
                    }           
                    if (equal && count == arrayres.size()) {
                        tabelaConversao[x][y] = AFN.estados[i];
                        newTransicoes.push_back(AFN.estados[x] + ',' + tabelaConversao[x][y] + ',' + AFN.simbolos[y]);                    
                        break;
                    }
                }
            }
        }       
    }

    //-------------------
    cout << "Tabela de transição AFD derivada:" << endl;
    for(int i = 0; i < sizeEstados-1; i++){
        for(int j = 0; j < AFN.simbolos.size(); j++){
            cout << tabelaConversao[i][j] << "\t";
        }
        cout << endl;
    }
    //-------------------

    AFN = removetrasitions(AFN, newTransicoes);  
    return AFN;
}

void addToList (listaEncadeada* lista, string value, string value0){
    if (lista->prox != NULL) addToList(lista->prox, value, value0);
    if (lista->value == "A"){
        lista->value = value;
        lista->value0 = value0;
        return;
    }
    listaEncadeada *add = new listaEncadeada();
    add->prox = NULL;
    add->value = value;
    add->value0 = value0;
    lista->prox = add;
}

void notEqual (vector<string> estados, vector<vector<listaEncadeada*>> tabelaEquivalencia, listaEncadeada *prox){
    cout << "Lista" <<prox->value << prox->value0 << endl;
    if (prox == nullptr) return;
    if (prox->prox != nullptr) notEqual(estados, tabelaEquivalencia, prox->prox);
    if (prox->value == "X") return;
    if (prox->value != "A"){
        int posi = find(begin(estados), end(estados), prox->value) - begin(estados);
        int posj = find(begin(estados), end(estados), prox->value0) - begin(estados);
        
        if (tabelaEquivalencia[posi][posj]->value != "X") notEqual (estados, tabelaEquivalencia, tabelaEquivalencia[posi][posj]);          
    }
    prox->value = "X";
    prox->value0 = "X";
    
    return;
}

AutomatoFinito minimoAFD (AutomatoFinito AFD){ // Minimiza um AFD
    vector<vector<string>> tabelaTransicao = generateTable(AFD); //Tabela das transições do automato AFD
    vector<string> transicaoInicial = tabelaTransicao[0];
    tabelaTransicao.erase(tabelaTransicao.begin());
    tabelaTransicao.emplace(tabelaTransicao.end()-1, transicaoInicial);
    // tabelaTransicao.emplace(tabelaTransicao.begin(), transicaoInicial);
    bool transicaoVazia = false;
    cout << "Tabela de transição AFD:" << endl;
    for (int i = 0; i < AFD.estados.size(); ++i) {
        for (int j = 0; j < AFD.simbolos.size(); ++j) {
            if (tabelaTransicao[i][j] == "") {
                tabelaTransicao[i][j] = "V,";
                if (transicaoVazia == false){
                    transicaoVazia = true;
                    AFD.estados.push_back("V");
                }               
            }
            cout << tabelaTransicao[i][j] << "\t";
        }
        cout << endl;
    }  
    vector<vector<listaEncadeada*>> tabelaEquivalencia (AFD.estados.size()); //Tabela de equivalência dos estados
    //Marcar com X, os estados diferentes (final e não final)
    for (int x = 1; x < AFD.estados.size(); x++){//Deslocamento x y na tabela
        for(int y = 0; y < x; y++){
            listaEncadeada* add = new listaEncadeada();
            add->prox = nullptr;

            bool finalX = false, finalY = false; //Situação do estado na tabela final
            if (find(begin(AFD.estadosFinais),end(AFD.estadosFinais), AFD.estados[x]) != end(AFD.estadosFinais))
                finalX = true;
            if (find(begin(AFD.estadosFinais),end(AFD.estadosFinais), AFD.estados[y]) != end(AFD.estadosFinais))
                finalY = true;

            if(finalX != finalY) add->value = "X";
            else add->value = "A";
            tabelaEquivalencia[x].push_back(add);
        }
    }
    //Tabela gerada
    cout << "Tabela dos finais com não finais:" << endl;
    for (int i = 1; i < AFD.estados.size(); i++){       
        cout << AFD.estados[i] << "\t";
        for (int j = 0; j < i; j++){
            cout << tabelaEquivalencia[i][j]->value << "\t";            
        }
        cout << endl;
    }
    cout << "\t ";
    for (int i = 0; i < AFD.estados.size()-1;i++){
        cout << AFD.estados[i] << "\t";
    } 
    cout << endl;
    //Verificação de equivalência para situações não triviais
    for (int x = 1; x < AFD.estados.size(); x++){//Deslocamento x y na tabela de equivalência      
        for (int y = 0; y < x; y++){
            if (tabelaEquivalencia[x][y]->value != "X"){
                for(int s = 0; s < AFD.simbolos.size(); s++){ //Para cada símbolo s
                    string qi, qj; //Estados para onde x e y transitam
                    qi = tabelaTransicao[x][s];
                    qj = tabelaTransicao[y][s];
                    qi.pop_back();
                    qj.pop_back();
                    cout << AFD.estados[x] << ":" << AFD.estados[y] << endl;
                    
                    if(qi != qj){
                        auto itI = find(begin(AFD.estados), end(AFD.estados), qi);
                        auto itJ = find(begin(AFD.estados), end(AFD.estados), qj);
                        int posi, posj;//Posição X e Y do estado de referência na tabela de equivalência
                        if (itI < itJ){                        
                            string tmp = qi;
                            auto tmp2 = itI;
                            itI = itJ;
                            itJ = tmp2;
                            qi = qj;
                            qj = tmp;
                        }
                        cout << "R:" << qi << ":" << qj << endl;
                        posi = itI - begin(AFD.estados);
                        posj = itJ - begin(AFD.estados);
                        cout << tabelaEquivalencia[posi][posj]->value << endl;
                        if(tabelaEquivalencia[posi][posj]->value != "X"){
                            addToList(tabelaEquivalencia[posi][posj], AFD.estados[x], AFD.estados[y]);
                            cout << "oi" << endl;
                        }
                        else{
                            notEqual(AFD.estados,tabelaEquivalencia,tabelaEquivalencia[x][y]);
                        }
                    }                      
                }
            }            
        }
    }
    cout << "Tabela de equivalência:" << endl;
    for (int i = 1; i < AFD.estados.size(); i++){       
        cout << AFD.estados[i] << "\t";
        for (int j = 0; j < i; j++){
            cout << tabelaEquivalencia[i][j]->value << "\t";            
        }
        cout << endl;
    }
    cout << "\t ";
    for (int i = 0; i < AFD.estados.size()-1;i++){
        cout << AFD.estados[i] << "\t";
    } 
    cout << endl;
    //Remove o estado vazio V da lista
    if(transicaoVazia){
        string cu = AFD.estados.back();
        cout << cu << endl;
        AFD.estados.pop_back();
    }
    
    int count = 0; //numero de novos estados;
    
    vector<string> estados = AFD.estados;
    for (int i = 1; i < estados.size(); i++){       
        cout << estados[i] << "\t" << endl;
        for (int j = 0; j < i; j++){
            
            if (tabelaEquivalencia[i][j]->value != "X"){
                for (int x = 0; x < AFD.transicoes.size(); x++){
                    
                    istringstream tmp(AFD.transicoes[x]);//Alocação temporária da transição
                    string estadoI, estadoJ, simbol;
                    getline(tmp, estadoI, ',');
                    getline(tmp, estadoJ, ',');
                    getline(tmp, simbol, ',');
                    if (estadoI == estados[i] || estadoI == estados[j]){
                        estadoI = "N" + to_string(count);
                    }
                    if (estadoJ == estados[i] || estadoJ == estados[j]){
                        estadoJ = "N" + to_string(count);
                    }
                    AFD.transicoes[x] = estadoI + ',' + estadoJ + ',' +simbol;
                }      
                auto it = find (AFD.estadosFinais.begin(), AFD.estadosFinais.end(), estados[i]);
                if (it != AFD.estadosFinais.end()){
                    int pos = it - AFD.estadosFinais.begin();
                    AFD.estadosFinais[pos] =  "N" + to_string(count);   
                }              
                it = find (AFD.estadosFinais.begin(), AFD.estadosFinais.end(), estados[j]);
                if (it != AFD.estadosFinais.end()) AFD.estadosFinais.erase(it);
                if (estados[i] == AFD.estadoInicial) AFD.estadoInicial = "N" + to_string(count);

                it = find (AFD.estados.begin(), AFD.estados.end(), estados[i]);
                if (it != AFD.estados.end()){
                    int pos = it - AFD.estados.begin();
                    AFD.estados[pos] =  "N" + to_string(count);   
                }
                AFD.estados.erase(find(AFD.estados.begin(), AFD.estados.end(), AFD.estados[j]));
                // AFD.estadosRepresentados.erase(find(AFD.estadosRepresentados.begin(), AFD.estadosRepresentados.end(), AFD.estadosRepresentados[j]));
                AFD.estadosRepresentados.erase(AFD.estadosRepresentados.begin(), AFD.estadosRepresentados.end());
                count++;
            }
            
            // cout << tabelaEquivalencia[i][j]->value << "\t";            
        }
        cout << endl;
    }
    cout << "\t ";
    for (int i = 0; i < AFD.estados.size()-1;i++){
        cout << AFD.estados[i] << "\t";
    } 
    cout << endl;
    return AFD;
}

int main (){   
    AutomatoFinito AFNe; //Definição do Automato Finito Não determinístico com transições em epsilon
    AutomatoFinito AFN; //Automato Finito Nao determinístico sem transições em epsilon
    AutomatoFinito AFD; //Automato finito Determinístico
    AutomatoFinito minAFD; //Automato finito determinístico minimizado

    AFNe = Escrita(AFNe); // Escreve o AFNe com base no input do usuário
    LerAutomato (AFNe);
    cout << "----- thompson -------" << endl; 
    AFN = thompson(AFNe); //Executa o algorítimo de thompson
    cout << "----- AFN > AFD ------" << endl;
    AFD = AFNtoAFD(AFN); //Converte a saída do Algorítmo de thompson para AFD
    LerAutomato (AFD);
    cout << "-- Minimização AFD ---" << endl;
    minAFD = minimoAFD(AFD); //Minimiza o AFD    
    cout << "------ saída ---------" << endl;
    LerAutomato (minAFD);

    

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