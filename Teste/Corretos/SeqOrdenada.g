principal 
/* Esse programa pede uma sequencia de numeros e diz se ela e crescente.
A interacao foi reescrita para nao deixar o usuario "no vacuo" com o cursor piscando.*/

	{
		quant: int;
	}
	{
		escreva "=== BEM-VINDO AO TESTE DE ORDENACAO ==="; novalinha;
		escreva "Digite o tamanho da sequencia de numeros (ou digite 0 para encerrar): ";
		leia quant;
		
		enquanto (quant != 0)
		{
			cont, valAtual:int;
			ordenado:car;
		}
		{	
			ordenado='v';
			cont=1;
			
			novalinha;
			escreva "-> OK! Agora, digite um por um os ";
			escreva quant;
			escreva " numeros da iteracao (aperte ENTER ou ESPACO apos cada numero):";
			novalinha;
			
			leia valAtual; /* Pega o primeiro numero com a tela ja avisando o q fazer */
			
			enquanto (cont < quant)
			{
				proxVal: int; 
			}
			{
				leia proxVal;
				se (valAtual < proxVal)
				entao 
					valAtual=proxVal;
				senao
				{
					ordenado='f';
					/* Removido o 'cont=quant' (Break early) daqui!
					   Se o usuario colocar '5 4 3 2' e a gente quebrar cedo, 
					   ele vai continuar lendo o lixo do terminal. 
					   A gente deixa ele fazer o calculo ate o fim pra varrer o lixo! */
				}
				fimse
				cont=cont+1;		
			}
			
			novalinha;
			se (ordenado=='v')
			entao{ 
				escreva ">> RESULTADO: A sua sequencia esta ORDENADA de forma crescente!";
				novalinha;
			}
			senao{
				escreva ">> RESULTADO: A sua sequencia esta DESORDENADA.";
				novalinha;
			}
			fimse
			
			novalinha;
			escreva "--------------------------------------------------------";
			novalinha;
			escreva "NOVO TESTE: Digite o tamanho de outra sequencia (ou 0 para terminar): ";
			leia quant;	 /* Aqui nao vai ter lixo porque varremos tudo no laco de cima! */
		}
	}
