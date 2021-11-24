#include <cstring>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <utility>
#define MAXNSIMBOLOS 1
using namespace std;
struct Node {
  int children[26];
  int frequence;
  char arquivos[3][31];
  Node() {
    memset(children, -1, sizeof(children));
    frequence = 0;
  }
};
struct Trie {
  FILE *trie;
  vector<Node> trieInMemory;
  Trie() {
    if (trie = fopen("trie.dat", "r+")) {
      fseek(trie, 0, SEEK_END);
      int tam = ftell(trie) / sizeof(Node);
      fseek(trie, 0, SEEK_SET);
      for(int i = 0; i < min(MAXNSIMBOLOS,tam); i++){
        Node aux;
        fread(&aux, sizeof(Node), 1, trie);
        trieInMemory.push_back(aux);
      }
      return;
    }
    trie = fopen("trie.dat", "w+");
    fclose(trie);
    trie = fopen("trie.dat", "r+");
    this->create_node();
  }

  void create_node() {
    fseek(trie, 0, SEEK_END);
    Node aux;
    fwrite(&aux, sizeof(Node), 1, trie);
  }

  void create_node(Node aux) {
    fseek(trie, 0, SEEK_END);
    fwrite(&aux, sizeof(Node), 1, trie);
  }

  int size() {
    fseek(trie, 0, SEEK_END);
    return ftell(trie) / sizeof(Node);
  }

  Node operator[](int pos) {
    if(pos<trieInMemory.size()){
        return trieInMemory[pos];
    }
    Node aux;
    fseek(trie, pos * sizeof(Node), SEEK_SET);
    fread(&aux, sizeof(Node), 1, trie);
    return aux;
  }

  void write(int pos, Node aux) {
    fseek(trie, pos * sizeof(Node), SEEK_SET);
    fwrite(&aux, sizeof(Node), 1, trie);
  }

  int root() { return 0; }

  void close() { fclose(trie); }
};
struct StringFile{
  FILE *s;
  string comeco;
  StringFile(char *fileName,string comeco){
    this->comeco = comeco;
     if (s = fopen(fileName, "r+")) {
      return;
    }
    s = fopen(fileName, "w+");
    fclose(s);
    s = fopen("fileName", "r+");
  }
  char operator[](int pos){
    if(pos <comeco.size()){
      return comeco[pos];
    }
    pos -= comeco.size();
    char aux;
    fseek(s,pos * sizeof(char), SEEK_SET);
    fread(&aux,sizeof(char),1,s);
    return aux;
  }
  int size(){
    fseek(s, 0, SEEK_END);
    return ftell(s) / sizeof(char) + comeco.size();
  }
  void close(){
    fclose(s);
  }
};
struct VectorPi{
  FILE *pi;
  VectorPi(){
    remove("pi");
    pi = fopen("pi","w+");  
  }
  void push_back(int aux){
    fseek(pi, 0, SEEK_END);
    fwrite(&aux, sizeof(int), 1, pi);
  }
  int operator[](int pos) {
    int aux;
    fseek(pi, pos * sizeof(int), SEEK_SET);
    fread(&aux, sizeof(int), 1, pi);
    return aux;
  }
  int size(){
    fseek(pi, 0, SEEK_END);
    return ftell(pi) / sizeof(int);
  }
  void close(){
    fclose(pi);
  }
};
void Insert(Trie &root, string s, string arquivo){
    int v = 0;
    for(int i = 0; i < s.size(); i++){
        Node newNode = root[v];
        int pos = s[i] - 'a';
        if(newNode.children[pos] == -1){
            root.create_node();
            newNode.children[pos] = root.size()-1;
            root.write(v,newNode);
        }
        v = newNode.children[pos];
    }
    if(root[v].frequence == 3){
        return;
    }
    Node newNode = root[v];
    for(int i = 0; i < arquivo.size(); i++){
        newNode.arquivos[root[v].frequence][i] = arquivo[i];
    }
    newNode.arquivos[root[v].frequence][arquivo.size()] = '\0';
    for(int i =0; i < newNode.frequence; i++){
        if(!strcmp(newNode.arquivos[root[v].frequence],newNode.arquivos[i])){
            return;
        }
    }
    newNode.frequence++;
    root.write(v,newNode);
}

bool insertFile(Trie &root, string fileName){
    FILE *file;
    char fileNameinArray[30];
    for(int i = 0; i < fileName.size();i++){
        fileNameinArray[i]  = fileName[i];
    }
    fileNameinArray[fileName.size()] = '\0';
    if(!(file = fopen(fileNameinArray,"r+"))){
        return false;
    }
    fseek(file,0,SEEK_END);
    string word;
    int fileSize = ftell(file)/sizeof(char);
    fseek(file,0,SEEK_SET);
    for(int i = 0; i <fileSize; i++){
        char aux;
        fread(&aux,sizeof(char),1, file);
        if(aux < 'a' or aux >'z'){
            if(word.size()>0){
                Insert(root, word, fileName);
            }
            word = "";
        }else{
            word += aux;
        }
    }
    return true;
}
void PrintOut(Trie &root, string s = "", int v = 0) {
  if (root[v].frequence>0) {
    cout << s << ' ' << root[v].frequence << endl;
  }
  for (int i = 0; i < 26; i++) {
    if (root[v].children[i] == -1)
      continue;
    PrintOut(root, s + (char)('a' + i), root[v].children[i]);
  }
}
vector<int> TabelaPi(string s) {
    int n = (int)s.length();
    vector<int> pi(n);
    for (int i = 1; i < n; i++) {
        int j = pi[i-1];
        while (j > 0 && s[i] != s[j])
            j = pi[j-1];
        if (s[i] == s[j])
            j++;
        pi[i] = j;
    }
    return pi;
}
VectorPi TabelaPi(StringFile s){
  int n = (int)s.size();
  VectorPi pi;
  pi.push_back(0);
  for (int i = 1; i < n; i++) {
      int j = pi[i-1];
      while (j > 0 && s[i] != s[j])
          j = pi[j-1];
      if (s[i] == s[j])
          j++;
      pi.push_back(j);
  }
  return pi;
}
bool Consult(Trie &root, string s,bool print = false) {
  int v = 0;
  for (int i = 0; i < s.size(); i++) {
    int x = s[i] - 'a';
    if (root[v].children[x] == -1) {
      return false;
    }
    v = root[v].children[x];
  }
  if(root[v].frequence == 0){
    return false;
  }
  if(print){
    cout << "palavra: "<< s << endl<< "arquivos em que ocorre:";
    for(int i = 0; i < root[v].frequence; i++){
      printf(" %s",root[v].arquivos[i]);
    }
    cout<<endl;
  }
  return true;
}
vector<string> ConsultExp(Trie &root, string s){
  int v = 0;
  for (int i = 0; i < s.size(); i++) {
    int x = s[i] - 'a';
    v = root[v].children[x];
  }
  vector<string> arquivos;
  for(int i =0 ; i < root[v].frequence; i++){
    string aux = "";
    for(int j = 0; j < 31; j++){
      if(root[v].arquivos[i][j] == '\0'){
        break;
      }
      aux += root[v].arquivos[i][j];
    }
    arquivos.push_back(aux);
  }
  
  return arquivos;
}

void consultExpecifics(Trie &root, string word){
  vector<string> arquivos;
  cout << "ocorrencias da palavra: " << word << ":"<<endl;
  arquivos = ConsultExp(root,word);
  for(int i = 0; i < arquivos.size(); i++) {
    cout << "arquivo: " << arquivos[i] << endl;
    char arquivo[31];
    for(int j = 0; j < arquivos[i].size(); j++){
      arquivo[j] = arquivos[i][j];
    }
    arquivo[arquivos[i].size()] = '\0';
    StringFile aux = StringFile(arquivo,word + "$");
    VectorPi pi = TabelaPi(aux);
    int line = 1, caracter = 1;
    for(int j = word.size()+1; j<aux.size(); j++){
      if(aux[j] == '\n'){
        line++;
        caracter = 0;
      }
      if(pi[j] == word.size() and (j == (int)aux.size()-1 or (aux[j+1]<'a' or aux[j+1] >'z')) and (j - (int)word.size() == -1 or (aux[j-(int)word.size()] <'a' or aux[j-(int)word.size()] > 'z'))){
        cout << "linha: " << line << " posicao: " << caracter - (int)word.size()+1 <<endl;
      }
      caracter++;
    }
    aux.close();
    pi.close();
  }
}
int main(){
    Trie root;
    while(true){
        char operation;
        cin >> operation;
        if( operation == 'i'){
            string arquivo;
            cin >> arquivo;
            if(insertFile(root,arquivo)){
                cout << "arquivo processado com sucesso: " << arquivo << endl;
            }else{
                cout << "arquivo nao encontrado" << endl;
            }
        }else if(operation == 'c'){
          string word;
          cin >> word;
          if(!Consult(root,word)){
            cout << "palavra nao ocorre na base: " << word << endl;
            continue;
          }
          consultExpecifics(root,word);
        }else if(operation == 'a'){
          string word;
          cin >> word;
          if(!Consult(root,word,true)){
            cout<<"nao ha ocorrencia da palavra: " << word << endl;
          };

        }else if(operation == 'p'){
            string word;
            cin >> word;
            vector<int> tabelaPi = TabelaPi(word);
            cout << "tabela pi para a palavra: " << word << ":"<<endl;
            for(int i = 0; i < tabelaPi.size(); i++){
                cout <<"'" << word[i] <<"': "<< tabelaPi[i] <<endl; 
            }
        }else if(operation == 'n'){
            cout << root.size() << endl;
        }else if(operation == 'e'){
            break;
        }
    }
    root.close();
}