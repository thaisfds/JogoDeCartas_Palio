#include "menuInicial.hpp"
using namespace std;

//Inicializa o menu inicial com valores padrão
MenuInicial::MenuInicial() {}

//Mostra todas as opções válidas para o usuário selecionar
void MenuInicial::menu(){
	system("cls||clear");
	//Exibe as possibilidades do jogo
	cout << "- MENU DE INICIO - " << endl;
	cout << "Escolha uma das opcoes:" << endl;
	cout << "1 - Iniciar jogo" << endl;
	cout << "2 - Ver Regras e Cartas" << endl;
	cout << "3 - Sair e fechar" << endl;
	//Escolhe a opção (caso seja válida) 
	string opcao;
	bool opcaoValida;
	do{
		cin >> opcao;
		try{
			if (stoi(opcao) <= 0 || stoi (opcao) > 3 ){
				throw OpcaoInvalida();
			}
			else {
				opcaoValida = true;
				switch (stoi(opcao)){		
				case 1:
					iniciar();			           
					break;
				case 2:
					pedirAjuda();
					break;
				case 3:
					break;
				}
			}
		} catch (const OpcaoInvalida& vci){
			cout << vci.what() << endl;
			opcaoValida = false;
		} catch (const exception& e){
			cout << "Por favor insira um número!" << endl;
			opcaoValida = false;
		}
	}while (!opcaoValida);
}

//Exibe o menu de ajuda 
void MenuInicial::pedirAjuda (){
	ajuda.menuDeAjuda();
	menu();
}

//Inicia o jogo
void MenuInicial::iniciar (){
	system("cls||clear");
	cout << "O jogo foi iniciado" << endl;
	cout << "Quantos jogadores vao jogar? *Permitido de 2 a 6 jogadores" << endl;
	
	//Armazena os jogadores em um vetor. Número varia de 2 a 6
	vector<Jogador*> jogadores;
	string numeroJogadores;
	string nomeJogadores;
	bool numeroValidoJog;

	//Testa se o número de jogadores é válido
	do{
		cin >> numeroJogadores;
		try {
			if (stoi(numeroJogadores) < 2 || stoi(numeroJogadores) > 6){
				throw NumeroInvalidoJogadores();
			}
			else{
				numeroValidoJog = true;
			}
		}catch (NumeroInvalidoJogadores& nij){
			cout << nij.what() << endl;
			numeroValidoJog = false;
		}catch (const exception& e){
			cout << "Por favor insira um número!" << endl;
			numeroValidoJog = false;
		}
	}while (!numeroValidoJog);

	//Pega o nome dos jogadores
	for(int i=0; i<stoi(numeroJogadores); i++){
		cout << "Digite o nome do " << i+1 << "º jogador: ";
		cin >> nomeJogadores;
		bool temJogador=false;

		for(Jogador* j : jogadores){
			if(j->getNome() == nomeJogadores){
				temJogador = true;
			}
		}

		//Dois jogadores não podem ter o mesmo nome
		if (!temJogador){
			Jogador* temp = new Jogador(nomeJogadores);
			jogadores.push_back(temp);
		}
		else{
			i--;
			cout << "Esse jogador já existe." << endl;
		}
	}

	//Limpa a tela e mostra as opções do modo de jogo
	system("cls||clear");
	cout << "Escolha o modo de jogo." << endl;
	cout << "1 - Modo padrao." << endl;
	cout << "2 - Modo personalizado" << endl;
	string modo;
	bool modoValido;

	//Trata as entradas relacionadas ao modo
	do{	
		cin >> modo;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		try{
			if (stoi(modo) <= 0 || stoi (modo) > 2 ){
				throw OpcaoInvalida();
			}
			else {
				//Entra no modo 1, o padrão
				modoValido = true;
				switch (stoi(modo)){
				case 1:
				{
					system("cls||clear");
					Mesa* jogo = new Mesa(jogadores, regras); 
					Jogador* ganhador = jogo->gerenciarPartida();
					delete jogo;
					cout << "Parabéns " << ganhador->getNome() << "! Você venceu o jogo! (talvez os outros jogadores não queiram mais ser seus amigos :) )" << endl;
					break;
				}  
				
				//Entra no modo 2, o modo personalizado
				case 2:
					system("cls||clear");
					//Escolhe as regras e exibe
					escolherRegras();
					system("cls||clear");
					regrasEmUso();
					cout << "Digite enter para continuar:";
					cin.ignore();
					system("cls||clear");
					Mesa* jogo2 = new Mesa(jogadores, regras);
					Jogador* ganhador = jogo2->gerenciarPartida();
					delete jogo2;
					cout << ganhador->getNome() << endl;
					break;
				}
			}
			} catch (const OpcaoInvalida& vci){
				cout << vci.what() << endl;
				modoValido = false;
			} catch (const exception& e){
				cout << "Por favor, insira um número!" << endl;
				modoValido = false;	
			}
	}while(!modoValido);
}   

//Escolhe as regras que serão ativadas no modo personalizado
void MenuInicial::escolherRegras(){
	char resposta;
	//Ativa as regras
	ativarRegra("EmpilharComprarCarta", "O jogador pode jogar cartas de comprar carta em resposta à cartas de comprar carta");
	for(Regra* r : this->regras){
		if(r->getNome() == "EmpilharComprarCarta"){
			ativarRegra("+4Em+2", "O jogador pode jogar a carta +4 em resposta à carta +2");
		}
	}
	ativarRegra("JogarDepoisDeComprar", "O jogador pode jogar depois de comprar");
	ativarRegra("CompraOpcional", "O jogador pode comprar carta sem precisar");
	ativarRegra("PularTurno", "O jogador pode pular o turno");
	cout << "Gostaria de jogar com as cartas padrão?" << endl;
	do{
		cin >> resposta;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		if(!(tolower(resposta) == 's') && !(tolower(resposta) == 'n')){
			throw RespostaInvalida();
		}
	} while(!(tolower(resposta) == 's') && !(tolower(resposta) == 'n'));
	
	system("cls||clear");

	if(tolower(resposta) == 'n'){
		int cartasTiradas = 0;
		string resposta;
		cout << "Digite o id das cartas que deseja retirar. Pode retirar no máximo 5 cartas. Digite p para parar." << endl;
		for(int i = 0; i < 10; i++){
			cout << i << ":|  " << i << "  | ";
		}
		cout << "10:|  @  | 11:|  S  | 12:| +2  | 13:|  C  | 14:| +4  |" << endl;
		do{
			cin >> resposta;
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			try{
				if((stoi(resposta) < 0 || stoi(resposta) > 14) && resposta != "p"){
					throw ValorCartaInvalido();
				}
				else{
					string regraCarta = "TirarCarta" + resposta;
					switch (stoi(resposta)){
						case 10:
							resposta = "@";
							break;
						case 11:
							resposta = "S";
							break;
						case 12:
							resposta = "+2";
							break;
						case 13:
							resposta = "C";
							break;
						case 14:
							resposta = "+4";
							break;
						default:
							break;
					}

					//Adiciona as regras selecionadas
					string descricaoRegraCarta = "A carta " + resposta + " foi retirada do jogo.";
					bool regraAdicionada = false;
					for(Regra* r : this->regras){
						if(r->getNome() == regraCarta){
							regraAdicionada = true;
						}
					}
					if(!regraAdicionada){
						regras.push_back(new Regra(regraCarta, descricaoRegraCarta));
						cartasTiradas++;
					}
					//Uma carta não pode ser retirada duas vezes
					else{
						cout << "A carta " + resposta + " já tinha sido retirada" << endl;
					}
				}
			} catch(const ValorCartaInvalido& vci){
				cout << vci.what()<< endl;
			} catch (const exception& e){
				cout << "Por favor insira um número!" << endl;
			}
		}while(!(resposta == "p") && cartasTiradas < 5);
		cout << "Digite o id das cartas que deseja adicionar. Digite p para parar." << endl;

	}
}

//Ativa as regras selecionadas
//@param nomeRegra Nome/descrição mais sintetizada da regra 
//@param descricao Descição mais detalhada da regra  
void MenuInicial::ativarRegra(string nomeRegra, string descricao){
	char resposta;
	bool respostaEhValida;
	cout << descricao << "?" << " s/n" << endl;
	do{
		cin >> resposta;
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		try{
			if(!(tolower(resposta) == 's') && !(tolower(resposta) == 'n')){
				throw RespostaInvalida();
			}
			else{
				respostaEhValida = true;
			}
		} catch(const RespostaInvalida& ri){
			cout << ri.what() << endl;
			respostaEhValida = false;
		}
	} while(!respostaEhValida);
	if(tolower(resposta) == 's'){
		regras.push_back(new Regra(nomeRegra, descricao));
	}
}

//Exibe as regras que foram selecionadas
void MenuInicial::regrasEmUso(){
	cout << "As regras escolhidas foram: "<< endl;
	for(Regra* r: this->regras){
		cout << r->getDescricao() << endl;
	}
}