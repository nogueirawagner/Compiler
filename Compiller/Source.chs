main()
{
	int &cab;
	dec &cob(20.3);
	&cab = &cob;
	int &joca = 20.4;
	int &cc;
	dec &pp(20.3);
	&cc = &pp;
	&pp = &cc;

	dec &leng(2.4);
	dec &logo(3.4);
	&leng = &logo;

	dec &deng(2.4);
	dec &dogo(3.4) = &deng;	

	int &a = 0, &b = 10, &aA = 0, &b1B = 10;
	dec &raio(2.4) = 2.54;
	char &c(2) = "d";
	int &idade = 20, &idadex14 = 20, &idadexxxx16 = 20, &idadexx141 = -20, &idadexxx113 = 1, &idadexx211 = 20;
	int &cpf = 150;
	int &nome, &borracha;
	&nome = 70;
	char &pessoa(12) = "Wagner Alves";
	char &professora(5) = "Aline Dayane", &aluno(15) = "Vitor";
	char &nome(25);
	dec &real(1.0);
	dec &moeda(133.1) = 20;
	dec &conta(20.2) = &nome;
	int &show; &show = 20;
	dec &shofffw(20.3) = &show;
	char &tamanho(255);
	int &float;
	gets(&show);
	
	int &i;
	if(&i < 10 || &i > 20)

	gets(&cpf, &idade);
	char &conc(20) = " Jo"+"Mac";
	gets(&pessoa, &professora);
	gets(&cpf, &float);
	gets(&real);

	int &abelha, &azul;
	puts(&abelha, &azul);
	puts(&abelha + &azul);
	
	if(&i < 10)
	if(&i < 10 && &i > 20)
	if(&i < 10 || &i > 20)
	if(&i < 10 && &i < &a || &i == &i)
	if(&i > 10 || &i > 20 && &i > 10)
	
	for(int &i = 0; &i < 10; &i++)
	for(int &i = 0; &i < 10; &i--)
	for(int &i = 0; &i < 10; ++&i)
	for(int &i = 0; &i < 10; --&i)
	for(int &i = 0;;)
	for(10; &i < 10; &i++)
	for(; &i < 10; &i++)
	for(&i = 0; &i < 10; --&i)
	for(&i = 0; &i < 10; &i++)
}