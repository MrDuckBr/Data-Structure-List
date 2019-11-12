/*
 Trabalho de Estrutura de Dados
 Ambulatório LAW
 Copyright 2019 by Arthur Maia, Lyvia Abreu e Walisson Ferreira
 Arquivo principal, exerce todas as funções
*/

//====================BIBLIOTECAS NECESSARIAS==================
#include <Keypad.h> //BIBLIOTECA DO TECLADO
#include <LiquidCrystal.h> //BIBLIOTECA DA TELA LCD

//=====================KEY PAD==================================
const byte qtdLinhas = 4; //QUANTIDADE DE LINHAS DO TECLADO
const byte qtdColunas = 4; //QUANTIDADE DE COLUNAS DO TECLADO
 
//CONSTRUÇÃO DA MATRIZ DE CARACTERES
char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte PinosqtdLinhas[qtdLinhas] = {3, 7, 5, 6}; //PINOS UTILIZADOS PELAS LINHAS
byte PinosqtdColunas[qtdColunas] = {8, 9, 10,11}; //PINOS UTILIZADOS PELAS 

//===================INICIALIZAÇÃO DO TECLADO===============
Keypad keyPad= Keypad( makeKeymap(matriz_teclas), PinosqtdLinhas, PinosqtdColunas, qtdLinhas, qtdColunas); 

//==============SETANDO PORTAS DO LCD========================

LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

//=====================FUNÇAO DE PEGAR A SENHA==================
int Senha()
{
    char digito=NO_KEY;
    int senha = 0; //Senha para cancelamento;
    int contador = 100;
    int aux=10;
    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Senha para");
    lcd.setCursor(0, 1);
    lcd.print("cancelar:");
    lcd.setCursor(10,1);
    Serial.print("Senha para cancelar: ");

    while (digito != '#' and contador != 0 and digito==NO_KEY) { //"#" é pra finalizar a insercao  
        while(digito==NO_KEY){
        digito=keyPad.getKey();
        lcd.setCursor(aux,1);
        lcd.print(digito);
        Serial.print(digito);
        }
        if ((digito != NO_KEY) and (digito != '#')) {
            if (digito == '*') { //Cancelamento de Inserção
                Serial.println("Corrige");
                senha = 0;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Senha para");
                lcd.setCursor(0, 1);
                lcd.print("cancelar:");
                lcd.setCursor(10,1);
                Serial.println();
                Serial.print("Senha para cancelar ");
                aux=10;
                contador=100;
            }else{
                senha = senha + ((digito - '0') * contador);
                contador = contador / 10;
            }
            digito = NO_KEY;
            aux++;
        }
    }
    Serial.println();
    if (senha % 100 == 0) { //se for 0 no mod 100 tem um digito só a senha
        senha = senha / 100;
    }else if (senha % 10 == 0) { // se for 0 no mod 10 e nao for 0 no mod 100 quer dizer que a senha tem 2 digitos
        senha = senha / 10;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Senha Digitada: ");
    Serial.print("Senha digitada: ");
    lcd.setCursor(0,1);
    lcd.print(senha);
    Serial.println(senha);
    delay(2000);
    return senha;
}

//=================LISTA==================================================
typedef int Dado;
/*classe Noh, será utilizada com duplo encadeamento*/
class Noh {
    friend class Lista;
    public:
        Noh(Dado mDado);
    private:
        Noh* mPtProximo;
        Noh* mPtAnterior;
        Dado mDado;
};
/*classe Lista, todas as operações ocorrerão de acordo com ela*/
class Lista {
    public:
        Lista();
        ~Lista();
        void Insere(char tipo);
        Dado RemoveInicio();
        void RemoveMeio(Dado dado,char tipo);
        void Imprime(char tipo);
        bool Vazia();
        int mTamanho;
    private:
        Noh* mPtPrimeiro;
        Noh* mPtUltimo;
        Dado mVerDado;
};

//Instanciando funcão para o uso
void Atendimento(Lista& listap, Lista& listan, int& cont);

/*Funçao Atendimento, verifica se é a vez do atendimento prioritario
ou normal, exibe o paciente a ser atendido e remove ele da lista*/
void Atendimento(Lista& listap, Lista& listan, int& cont){
    Serial.println("Atendimento");
    int senha;
    if (listap.Vazia() and listan.Vazia()) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Não há pacientes");
        Serial.println("Não há pacientes.");
        delay(2000);
        cont = 0;
    }else if (listap.Vazia()) {
        senha=listan.RemoveInicio();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha atendida: ");
        lcd.setCursor(0, 1);
        lcd.print("C");       
        Serial.print("C");
        lcd.print(senha);
        Serial.println(senha);
        delay(2000);
        cont = 0;
    }else if (listan.Vazia()) {
       senha=listap.RemoveInicio();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha atendida ");
        lcd.setCursor(0, 1);
        lcd.print("P ");
        Serial.print("P ");
        lcd.print(senha);
        Serial.println(senha);
        delay(2000);
        if (cont < 3) {
            cont++;
        }
    }else{
        if(cont == 3){
            senha=listan.RemoveInicio();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Senha atendida: ");
            lcd.setCursor(0, 1);
            lcd.print("C ");
            Serial.print("C ");
            lcd.print(senha);
            Serial.println(senha);
            delay(2000);
            cont = 0;
        }else{
            senha=listap.RemoveInicio();
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Senha atendida : ");
            lcd.setCursor(0, 1);
            lcd.print("P ");
            Serial.print("P ");
            lcd.print(senha);
            Serial.println(senha);
            delay(2000);
            cont++;
        }
    }
}

/*Construtor do Noh*/
Noh::Noh(Dado dado) :
    mPtProximo(NULL)
    ,mPtAnterior(NULL)
    ,mDado(dado){
}

/*Construtor da lista*/
Lista::Lista()  :
    mTamanho(0)
    ,mPtPrimeiro(NULL)
    ,mPtUltimo(NULL)
    ,mVerDado(0){
}

/*Destrutor da lista*/
Lista::~Lista(){
    while (not Vazia()) {
        RemoveInicio();
    }
}

/*Funçao Insere, irá criar um novo dado automático e inserir na lista*/
void Lista::Insere(char tipo){
    mVerDado++;
    Noh* novo = new Noh(mVerDado);
    if (Vazia()) {
        mPtPrimeiro = novo;
        mPtUltimo = novo;
    }else {
        mPtUltimo->mPtProximo = novo;
        novo->mPtAnterior = mPtUltimo;
        mPtUltimo = novo;
    }
    delay(1000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Sua senha e:");
    Serial.print("Sua senha e: ");
    if(tipo=='1'){
        lcd.print("P");
        Serial.print("P");
    }if(tipo=='2'){
        lcd.print("C");
        Serial.print("C");
    }
    lcd.print(mVerDado);
    Serial.println(mVerDado);
    delay(3000);
    mTamanho++;
}

/*Funçao RemoveInicio, vai remover dados no inicio da lista */
Dado Lista::RemoveInicio(){
    Dado aux;
    if(mPtPrimeiro == mPtUltimo){
        aux = mPtPrimeiro->mDado;
        mVerDado = 0;
        mPtPrimeiro = NULL;
        mPtUltimo = NULL;
        mTamanho--;
        return aux;
    }else{
        aux = mPtPrimeiro->mDado;
        mPtPrimeiro = mPtPrimeiro->mPtProximo;
        mPtPrimeiro->mPtAnterior = NULL;
        mTamanho--;
        return aux;
    }
}

/*Funcao RemoveMeio, caso o paciente não possa ser atendido o mesmo poderá
solicitar ocancelamento da sua vez por meio da senha que geramos para ele*/
void Lista::RemoveMeio(Dado dado,char tipo){
    if (Vazia()){
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Nao ha ninguem");
        lcd.setCursor(2,1);
        lcd.print("para remover");
        Serial.println("Nao ha ninguem para remover");
        delay(2000);
    }else if(mPtPrimeiro->mDado > dado or mPtUltimo->mDado < dado) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Nao ha pacientes");
        Serial.println("Nao ha pacientes");
        delay(3000);
    }else if(mPtPrimeiro==mPtUltimo and mPtPrimeiro->mDado==dado){ //.
	    mPtPrimeiro=NULL;
	    mPtUltimo=NULL;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Senha ");
        Serial.print("Senha");
        if(tipo=='1'){
            lcd.print(" P");
            Serial.print(" P");
        }
        if(tipo=='2'){
            lcd.print("C");
            Serial.print(" C");
        }
        lcd.print(dado);
        Serial.print(dado);
        lcd.setCursor(0, 1);
        lcd.print("removido");
        Serial.println(" removido ");
        delay(1000);
    }else {
        Noh* teste;
        teste = mPtPrimeiro;
        while (teste->mDado != dado and teste != mPtUltimo) {
            teste = teste->mPtProximo;
        }
        if (teste->mDado == dado) {
            if (teste->mPtAnterior != NULL) {
                teste->mPtAnterior->mPtProximo = teste->mPtProximo;
            }else {
                mPtPrimeiro = teste->mPtProximo;
                mPtPrimeiro->mPtAnterior = NULL;
            }
            if (teste->mPtProximo != NULL) {
                teste->mPtProximo->mPtAnterior = teste->mPtAnterior;
            }else {
                mPtUltimo = teste->mPtAnterior;
                mPtUltimo->mPtProximo = NULL;
            }
            mTamanho--;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Senha ");
            Serial.print("Senha");
            if(tipo=='1'){
                lcd.print(" P");
                Serial.print(" P");
            }
            if(tipo=='2'){
                lcd.print("C");
                Serial.print(" C");
            }
            lcd.print(dado);
            Serial.print(dado);
            lcd.setCursor(0, 1);
            lcd.print("removido");
            Serial.println(" removido ");
            delay(1000);
        }else{
            lcd.clear();
            lcd.setCursor(2, 0);
            lcd.print("Paciente ja");
            lcd.setCursor(4, 1);
            lcd.print("removido");
            Serial.print("Paciente já removido");
            delay(2000);
        }
        delete teste;
    }
}

/*Funcao Vazia,verifica se a lista esta vazia*/
bool Lista::Vazia(){
    if(mPtPrimeiro == NULL) {
        return true;
    }else{
        return false;
    }
}

void Lista::Imprime(char tipo){
	if(Vazia()){
		Serial.println("Sem Pacientes");
	}
	else{
		Noh *teste= mPtPrimeiro;
		while(teste!=NULL){
			if(tipo=='P'){
				Serial.print("P");
			}
			if(tipo=='C'){
				Serial.print("C");
			}
			Serial.print(teste->mDado);
			Serial.print(" ");
			teste=teste->mPtProximo;
		}
		Serial.println();
	}
}
void setup(){
  //Inicializacao do LCD
  lcd.begin(16, 2); 
  lcd.clear();
  //Inicializacao da Porta Serial
  Serial.begin(9600);
  Serial.println("Ambulatório LAW ");
}
//============Variaveis Globais==========================
Lista listap; //Lista Prioritaria
Lista listan; //LISTA Comum
int cont=0;
Dado num; //Senha a ser cancelada


void loop(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("AMBULATORIO  LAW");
    Serial.println("Digite A para atender,B para Inserir, C para remover e D para vizualizar as senhas cadastradas");
    lcd.setCursor(0,1);
    lcd.print("Comando:");
    delay(1000);
    char comando=keyPad.getKey(); //tecla pressionada sera salva aqui
    while(comando==NO_KEY){
        comando = keyPad.getKey();
    }
    lcd.setCursor(9,1);
    lcd.print(comando);
    delay(2000);
//===============Cadastro=======================================================
    if (comando == 'B') {
        Serial.println("Comando selecionado : B");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Selecione o tipo");
        lcd.setCursor(0, 1);
        lcd.print("de Senha...");
        Serial.println("Selecione o tipo de senha desejado...");
        delay(2000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Prioritario: 1");
        Serial.println("Prioridade: 1");
        lcd.setCursor(0, 1);
        lcd.print("Comum: 2");
        Serial.println("Comum: 2");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tipo desejado:");
        Serial.print("Tipo desejado: ");
        lcd.setCursor(0, 1);
        comando = NO_KEY;
        while(comando==NO_KEY){
            comando = keyPad.getKey();  
        }
        lcd.print(comando);
        Serial.println(comando);
        delay(900);
        if (comando == '1') {
            listap.Insere(comando);
        }else if (comando == '2') {
            listan.Insere(comando);
        }else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("COMANDO INVALIDO");
            Serial.println("COMANDO INVALIDO");
        }
    }else if (comando == 'A') { // Atendimento
        Serial.println("Comando selecionado : A");
        lcd.clear();
        Atendimento(listap, listan, cont);
    }else if (comando == 'C') { // Cancelamento(Remoção)
        Serial.println("Comando selecionado : C");
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("Cancelamento!");
        Serial.println("Cancelamento!");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Prioridade:1");
        Serial.println("Prioridade: 1");
        lcd.setCursor(0, 1);
        lcd.print("Comum:2");
        Serial.println("Comum: 2");
        delay(1000);
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Tipo Desejado: ");
        Serial.print("Tipo Desejado: ");
        comando = NO_KEY;
        while(comando==NO_KEY){
            comando = keyPad.getKey();  
        }
        lcd.setCursor(0,1);
        lcd.print(comando);
        Serial.println(comando);
        delay(1000);
        lcd.clear();
        lcd.setCursor(10, 1);
        num = Senha(); //Senha a ser cancelada salva nesta variavel
        lcd.clear();
        if (comando == '1') {
            Serial.println("Remoção Prioritaria Selecionada");
            listap.RemoveMeio(num, comando);
        }else if (comando == '2') {
            Serial.println("Remoção Comum Selecionada");
            listan.RemoveMeio(num,comando);
        }
        else {
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("COMANDO INVALIDO");
            Serial.println("COMANDO INVALIDO");
            delay(1000);
        }
	}else if(comando=='D'){
		lcd.clear();
		lcd.setCursor(0,0);
		lcd.print("Impressao no");
		lcd.setCursor(0,1);
		lcd.print("serial");
		Serial.print("Senhas prioritarias: ");
		listap.Imprime('P');
		Serial.print("Senhas Comuns: ");
		listan.Imprime('C');
		delay(1000);
	}else { //Caso o comando nao seja o especificado
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COMANDO INVALIDO");
        Serial.println("COMANDO INVALIDO");
        delay(1000);
    }
    comando=NO_KEY;
}
