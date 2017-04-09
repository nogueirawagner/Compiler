main()
{	
	int &idade = 20;
	int &cpf = 10;
	int &nome, &borracha;
	&nome = 70;
	char &pessoa(15) = "Wagner Alves";
	char &professora(15) = "Aline Dayane", &aluno(15) = "Vitor Feio";
	char &nome(14);
	dec &real(1.0);
	dec &moeda(133.1) = 20;
	dec &conta(20.3) = &nome;
	int &show;
	&show = 20;
	dec &shofffw(20.3) = &show;
	char &tamanho(255);
	int &float;
	gets(&show);
	gets(&cpf, &idade);
	char &conc(20) = "Wagner" + "Alves";
}
