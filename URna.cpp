//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "URna.h"
#include "NeuralNetwork.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TFmRna *FmRna;
Thread *MyThread;
NeuralNetwork *net;
//---------------------------------------------------------------------------

unsigned long contador = 0, epocas = 0;

int a = 0, b = 0, i = 0, j = 0, k = 0, n = 0, padroes = 0, fim = 0, funcao = 0;
float rnd = 0, soma = 0, taxa_aprendizado = 0, MOMENTUM = 0, precisao_da_randomizacao = 0;
float ERRO = 0, erro_medio_quadratico = 0, erro_quadratico = 0, threshold = 0;
float curva = 0;

const int cx = 100;         // Camada de entrada.
const int c1 = 15;          // Camada Intermedi�ria.
const int c2 = 5;          // Camada de Sa�da.

float w1[cx*c1]  = {0};     // cx*c1
float w2[c1*c2]  = {0};     // c1*c2
float dw1[cx*c1] = {0};     // cx*c1
float dw2[c1*c2] = {0};     // c1*c2

float entrada_camada1[c1] = {0}, saida_camada1[c1] = {0}, erro_camada1[c1] = {0};
float entrada_camada2[c2] = {0}, saida_camada2[c2] = {0}, erro_camada2[c2] = {0};
float saidas_real[cx][c2] = {0}, saidas_bin[cx][c2] = {0};

int saidas_formatadas_c1[c1] = {0};
int saidas_formatadas_c2[c2] = {0};

 // Padr�es de Entrada da Rede.
float p[10][cx] =
{
//sinal 1 Hz
0.000000,0.062791,0.125333,0.187381,0.248690,0.309017,0.368125,0.425779,0.481754,0.535827,0.587785,0.637424,0.684547,0.728969,0.770513,0.809017,0.844328,0.876307,0.904827,0.929777,0.951057,0.968583,0.982287,0.992115,0.998027,1.000000,0.998027,0.992115,0.982287,0.968583,0.951057,0.929777,0.904827,0.876307,0.844328,0.809017,0.770513,0.728969,0.684547,0.637424,0.587786,0.535827,0.481754,0.425780,0.368125,0.309018,0.248691,0.187382,0.125334,0.062792,0.000001,-0.062789,-0.125332,-0.187380,-0.248689,-0.309016,-0.368123,-0.425778,-0.481752,-0.535825,-0.587784,-0.637423,-0.684546,-0.728967,-0.770512,-0.809016,-0.844327,-0.876306,-0.904826,-0.929776,-0.951056,-0.968583,-0.982287,-0.992114,-0.998027,-1.000000,-0.998027,-0.992115,-0.982288,-0.968584,-0.951057,-0.929778,-0.904828,-0.876308,-0.844330,-0.809019,-0.770515,-0.728971,-0.684549,-0.637427,-0.587788,-0.535830,-0.481757,-0.425783,-0.368128,-0.309020,-0.248693,-0.187385,-0.125337,-0.062794,
//sinal 2 Hz
-0.000008,0.125326,0.248683,0.368118,0.481747,0.587779,0.684541,0.770508,0.844324,0.904824,0.951054,0.982286,0.998026,0.998027,0.982289,0.951059,0.904831,0.844333,0.770519,0.684554,0.587793,0.481762,0.368134,0.248699,0.125343,0.000010,-0.125323,-0.248680,-0.368115,-0.481744,-0.587777,-0.684539,-0.770506,-0.844322,-0.904822,-0.951053,-0.982285,-0.998026,-0.998027,-0.982289,-0.951060,-0.904832,-0.844334,-0.770521,-0.684556,-0.587795,-0.481765,-0.368136,-0.248702,-0.125346,-0.000013,0.125320,0.248677,0.368112,0.481742,0.587774,0.684537,0.770504,0.844320,0.904821,0.951052,0.982285,0.998026,0.998028,0.982290,0.951061,0.904833,0.844336,0.770523,0.684558,0.587798,0.481767,0.368139,0.248705,0.125349,0.000016,-0.125317,-0.248674,-0.368109,-0.481739,-0.587772,-0.684535,-0.770503,-0.844319,-0.904820,-0.951051,-0.982284,-0.998026,-0.998028,-0.982291,-0.951062,-0.904835,-0.844337,-0.770525,-0.684560,-0.587800,-0.481770,-0.368142,-0.248708,-0.125352,
//sinal 3 Hz
-0.000028,0.187356,0.368100,0.535804,0.684528,0.809001,0.904816,0.968576,0.998025,0.992118,0.951065,0.876320,0.770531,0.637446,0.481779,0.309044,0.125362,-0.062762,-0.248662,-0.425753,-0.587761,-0.728948,-0.844312,-0.929765,-0.982282,-1.000000,-0.982293,-0.929788,-0.844344,-0.728990,-0.587810,-0.425808,-0.248720,-0.062822,0.125302,0.308987,0.481725,0.637399,0.770492,0.876291,0.951046,0.992111,0.998029,0.968591,0.904841,0.809037,0.684572,0.535856,0.368156,0.187415,0.000035,-0.187347,-0.368092,-0.535797,-0.684521,-0.808996,-0.904812,-0.968574,-0.998024,-0.992119,-0.951068,-0.876324,-0.770537,-0.637452,-0.481786,-0.309052,-0.125370,0.062753,0.248653,0.425745,0.587754,0.728942,0.844307,0.929762,0.982280,1.000000,0.982295,0.929791,0.844349,0.728996,0.587817,0.425816,0.248729,0.062831,-0.125293,-0.308978,-0.481718,-0.637392,-0.770487,-0.876287,-0.951044,-0.992109,-0.998029,-0.968594,-0.904845,-0.809042,-0.684578,-0.535863,-0.368165,-0.187424,
//sinal 4 Hz
-0.000058,0.248634,0.481703,0.684504,0.844296,0.951038,0.998023,0.982298,0.904852,0.770551,0.587834,0.368181,0.125393,-0.125273,-0.368068,-0.587736,-0.770474,-0.904801,-0.982276,-0.998031,-0.951076,-0.844362,-0.684593,-0.481809,-0.248751,-0.000064,0.248628,0.481697,0.684500,0.844293,0.951037,0.998023,0.982300,0.904855,0.770555,0.587839,0.368186,0.125399,-0.125267,-0.368062,-0.587731,-0.770470,-0.904798,-0.982275,-0.998031,-0.951078,-0.844365,-0.684597,-0.481814,-0.248757,-0.000069,0.248622,0.481692,0.684496,0.844290,0.951035,0.998022,0.982301,0.904857,0.770559,0.587843,0.368192,0.125405,-0.125261,-0.368057,-0.587726,-0.770467,-0.904796,-0.982273,-0.998031,-0.951079,-0.844368,-0.684601,-0.481819,-0.248763,-0.000075,0.248617,0.481687,0.684492,0.844287,0.951033,0.998022,0.982302,0.904860,0.770563,0.587848,0.368197,0.125411,-0.125256,-0.368052,-0.587722,-0.770463,-0.904793,-0.982272,-0.998032,-0.951081,-0.844371,-0.684606,-0.481824,-0.248768,
//sinal 5 Hz
-0.000101,0.308927,0.587715,0.808970,0.951034,1.000000,0.951074,0.809047,0.587821,0.309052,0.000029,-0.308996,-0.587773,-0.809012,-0.951056,-1.000000,-0.951052,-0.809005,-0.587762,-0.308983,0.000043,0.309064,0.587831,0.809055,0.951079,1.000000,0.951030,0.808962,0.587704,0.308915,-0.000115,-0.309133,-0.587890,-0.809097,-0.951101,-1.000000,-0.951008,-0.808920,-0.587646,-0.308846,0.000187,0.309201,0.587948,0.809139,0.951123,1.000000,0.950985,0.808878,0.587588,0.308778,-0.000259,-0.309270,-0.588006,-0.809182,-0.951145,-1.000000,-0.950963,-0.808835,-0.587529,-0.308709,0.000331,0.309338,0.588064,0.809224,0.951168,1.000000,0.950941,0.808793,0.587471,0.308641,-0.000403,-0.309407,-0.588123,-0.809266,-0.951190,-1.000000,-0.950919,-0.808751,-0.587413,-0.308572,0.000475,0.309475,0.588181,0.809309,0.951212,1.000000,0.950896,0.808708,0.587355,0.308504,-0.000547,-0.309544,-0.588239,-0.809351,-0.951234,-1.000000,-0.950874,-0.808666,-0.587296,-0.308435,
//sinal 6 Hz
0.000742,0.368823,0.685101,0.905154,0.998075,0.950813,0.770007,0.481050,0.124528,-0.249484,-0.588455,-0.844776,-0.982445,-0.982127,-0.843865,-0.587080,-0.247837,0.126215,0.482540,0.771091,0.951339,0.997968,0.904430,0.683861,0.367241,-0.000958,-0.369023,-0.685258,-0.905246,-0.998089,-0.950747,-0.769869,-0.480861,-0.124314,0.249693,0.588630,0.844892,0.982486,0.982086,0.843749,0.586905,0.247628,-0.126430,-0.482729,-0.771228,-0.951405,-0.997955,-0.904338,-0.683703,-0.367041,0.001174,0.369224,0.685415,0.905337,0.998102,0.950680,0.769731,0.480671,0.124100,-0.249902,-0.588805,-0.845007,-0.982526,-0.982045,-0.843633,-0.586730,-0.247418,0.126644,0.482919,0.771366,0.951472,0.997941,0.904245,0.683546,0.366840,-0.001390,-0.369425,-0.685572,-0.905429,-0.998115,-0.950613,-0.769593,-0.480482,-0.123885,0.250111,0.588979,0.845123,0.982566,0.982005,0.843517,0.586555,0.247209,-0.126858,-0.483108,-0.771503,-0.951538,-0.997927,-0.904153,-0.683388,-0.366639,
//sinal 7 Hz
0.001874,0.427483,0.771719,0.969055,0.981927,0.807884,0.480058,0.060850,-0.369941,-0.730312,-0.951665,-0.991864,-0.843257,-0.534133,-0.123334,0.310942,0.686029,0.930527,0.997896,0.875310,0.586105,0.185332,-0.250719,-0.639045,-0.905726,-0.999998,-0.903916,-0.635769,-0.246601,0.189509,0.589544,0.877358,0.998162,0.928962,0.682930,0.306899,-0.127552,-0.537722,-0.845535,-0.992396,-0.950350,-0.727401,-0.365988,0.065093,0.483783,0.810383,0.982723,0.967997,0.769008,0.423636,-0.002378,-0.427939,-0.772040,-0.969179,-0.981831,-0.807587,-0.479616,-0.060347,0.370409,0.730656,0.951820,0.991800,0.842986,0.533707,0.122834,-0.311421,-0.686396,-0.930712,-0.997863,-0.875066,-0.585697,-0.184837,0.251207,0.639433,0.905939,0.999997,0.903700,0.635380,0.246113,-0.190003,-0.589951,-0.877600,-0.998193,-0.928775,-0.682561,-0.306419,0.128052,0.538147,0.845804,0.992458,0.950193,0.727055,0.365519,-0.065596,-0.484224,-0.810678,-0.982816,-0.967870,-0.768686,-0.423179,
//sinal 8 Hz
0.003293,0.484647,0.846100,0.998230,0.903400,0.585071,0.121996,-0.371260,-0.772667,-0.982918,-0.949998,-0.682050,-0.245365,0.252023,0.687061,0.952122,0.981630,0.768284,0.364867,-0.128817,-0.590632,-0.906327,-0.997798,-0.842416,-0.478622,0.003581,0.484899,0.846254,0.998247,0.903277,0.584837,0.121711,-0.371527,-0.772850,-0.982971,-0.949908,-0.681839,-0.245085,0.252302,0.687271,0.952210,0.981575,0.768100,0.364599,-0.129103,-0.590865,-0.906448,-0.997779,-0.842261,-0.478369,0.003869,0.485151,0.846407,0.998264,0.903153,0.584604,0.121425,-0.371794,-0.773032,-0.983024,-0.949818,-0.681629,-0.244806,0.252581,0.687480,0.952298,0.981520,0.767916,0.364331,-0.129388,-0.591097,-0.906570,-0.997759,-0.842105,-0.478116,0.004157,0.485403,0.846561,0.998281,0.903029,0.584370,0.121139,-0.372062,-0.773215,-0.983077,-0.949728,-0.681418,-0.244527,0.252859,0.687689,0.952386,0.981464,0.767731,0.364063,-0.129674,-0.591329,-0.906691,-0.997740,-0.841950,-0.477864,
//sinal 9 Hz
0.004974,0.540031,0.906945,0.991474,0.767300,0.304221,-0.253580,-0.732426,-0.983226,-0.927890,-0.583649,-0.057683,0.486242,0.878772,0.997690,0.805968,0.363302,-0.192481,-0.688334,-0.969868,-0.949426,-0.633373,-0.120114,0.430544,0.847148,0.999986,0.841470,0.420956,-0.130626,-0.641537,-0.952700,-0.967232,-0.680609,-0.182073,0.373154,0.812195,0.998354,0.873666,0.476956,-0.068258,-0.592220,-0.931789,-0.981238,-0.725171,-0.243316,0.314297,0.774052,0.992799,0.902430,0.531083,-0.005622,-0.540576,-0.907217,-0.991389,-0.766884,-0.303604,0.254207,0.732867,0.983344,0.927649,0.583122,0.057036,-0.486808,-0.879082,-0.997645,-0.805585,-0.362698,0.193117,0.688804,0.970026,0.949223,0.632871,0.119471,-0.431128,-0.847492,-0.999982,-0.841120,-0.420368,0.131269,0.642034,0.952897,0.967068,0.680134,0.181435,-0.373755,-0.812573,-0.998391,-0.873351,-0.476387,0.068905,0.592742,0.932024,0.981113,0.724725,0.242688,-0.314912,-0.774462,-0.992876,-0.902151,-0.530533,
//sinal 10 Hz
0.006967,0.593419,0.953195,0.948867,0.582088,-0.007039,-0.593477,-0.953217,-0.948844,-0.582030,0.007111,0.593535,0.953238,0.948822,0.581971,-0.007183,-0.593592,-0.953260,-0.948799,-0.581912,0.007254,0.593650,0.953282,0.948776,0.581854,-0.007326,-0.593708,-0.953304,-0.948753,-0.581795,0.007398,0.593766,0.953325,0.948731,0.581737,-0.007470,-0.593824,-0.953347,-0.948708,-0.581678,0.007542,0.593882,0.953369,0.948685,0.581620,-0.007614,-0.593940,-0.953391,-0.948662,-0.581561,0.007686,0.593998,0.953412,0.948640,0.581503,-0.007758,-0.594056,-0.953434,-0.948617,-0.581444,0.007830,0.594114,0.953456,0.948594,0.581385,-0.007902,-0.594172,-0.953478,-0.948571,-0.581327,0.007974,0.594230,0.953499,0.948548,0.581268,-0.008046,-0.594287,-0.953521,-0.948526,-0.581210,0.008118,0.594345,0.953543,0.948503,0.581151,-0.008190,-0.594403,-0.953564,-0.948480,-0.581093,0.008262,0.594461,0.953586,0.948457,0.581034,-0.008334,-0.594519,-0.953608,-0.948434,-0.580975
};

// Valores desejados dos padr�es ao final do treinamento.
float d[10][c2] =
{
/*
1.0,	0.0,    0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	1.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	1.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	1.0,	0.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	1.0,    0.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    1.0,	0.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	1.0,    0.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    1.0,	0.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	1.0,	0.0,
0.0,	0.0,	0.0,	0.0,	0.0,    0.0,	0.0,    0.0,	0.0,	1.0
*/

0.0,	0.0,    0.0,	0.0,	1.0,
0.0,	0.0,    0.0,	1.0,	0.0,
0.0,	0.0,	0.0,	1.0,	1.0,
0.0,	0.0,    1.0,	0.0,	0.0,
0.0,	0.0,	1.0,	0.0,	1.0,
0.0,	0.0,    1.0,	1.0,	0.0,
0.0,	0.0,	1.0,	1.0,	1.0,
0.0,	1.0,    0.0,	0.0,	0.0,
0.0,	1.0,	0.0,	0.0,	1.0,
0.0,	1.0,    0.0,	1.0,	0.0


};




//---------------------------------------------------------------------------










//---------------------------------------------------------------------------
__fastcall TFmRna::TFmRna(TComponent* Owner)
	: TForm(Owner)
{

}

//---------------------------------------------------------------------------

void __fastcall TFmRna::FormCreate(TObject *Sender)
{
	// Redimensiona o valor m�ximo do eixo x com o tamanho da tela desejada.
	Chart2->BottomAxis->Maximum = 99;

	// Expande o gr�fico para comportar a quantidade de amostras contidas em max_tela.
	for (unsigned int a = 0; a < 100; a++)
	{
		Chart2->Series[0]->AddY(0);
	}

	// Atualiza o chart.
	Chart2->Refresh();

	// inscancia da rede neural
	net = new NeuralNetwork();

}
//---------------------------------------------------------------------------


void __fastcall TFmRna::Button1Click(TObject *Sender)
{
	// Limpa as s�ries do chart para nova plotagem.
	Chart1->Series[0]->Clear();

	// Instancia��o Declara��o da Thread
	MyThread = new Thread(true); 		// cria a thread suspensa, sem executar ainda.
	// MyThread->FreeOnTerminate = True;   // destroi a thread automaticamente depois qu efinalizou.
	MyThread->Priority = tpNormal;  		// configura com prioridade mais baixa que a normal.
	MyThread->Resume();					// faz com que a thread seja executada.
}

//---------------------------------------------------------------------------

void __fastcall TFmRna::Button2Click(TObject *Sender)
{
	// Verifica��o da inst�ncia da thread de atualiza��o do gr�fico.
	if (MyThread != NULL)
	{
		MyThread->Terminate();
		Sleep(500);

		delete MyThread;    // se usar (MyThread->FreeOnTerminate = True;) n�o precisa deletar.
		MyThread = NULL;

		FmRna->Memo1->Lines->Add("finalizou a thread e deletou a inst�ncia.");
	}
}

//---------------------------------------------------------------------------

// Declara��o da thread de processamento

__fastcall Thread::Thread(bool CreateSuspended)
				: TThread(CreateSuspended)
{
	FmRna->Memo1->Lines->Add("iniciou a thread...");
}

//---------------------------------------------------------------------------

void __fastcall Thread::Execute()
{

    /* initialize random weights: */
    srand (time(NULL));

	padroes = 10;               	// N�mero de padr�es a treinar.
	funcao = 1;                 	// Fun��o Log�stica(0).
	taxa_aprendizado = 0.001;    	// Taxa de Aprendizado.
	precisao_da_randomizacao = 0.1; // Precis�o da randomiza��o (0.1, 0.01, 0.001)
	ERRO = 0.0001;              	// Erro m�nimo aceit�vel da rede (se aplic�vel).
	MOMENTUM = 0.7;             	// Termo de momentum.
	epocas = 10000;            		// N�mero m�ximo de �pocas de treinamento.
	rnd = 0;                    	// Vari�vel auxiliar para a randomiza��o dos pesos.
	soma = 0;                   	// Vari�vel auxiliar para a soma das sinapses.
	erro_medio_quadratico = 0;  	// Vari�vel auxiliar do Erro m�dio quadr�tico.
	erro_quadratico = 0;        	// Vari�vel auxiliar do erro quadr�tico.
	fim = 0;                    	// Vari�vel de controle do final do treinamento.
	contador = 0;               	// Vari�vel de controle do n�mero de �pocas.
	curva = 0.3;

    // Zera o vetor de pesos da camada de entrada da rede e da camada 1.
    for (j = 0; j < (cx * c1); j++)
    {
        w1[j] = 0.0;
    }

    // Zera o vetor de pesos da camada 1 e da camada 2.
    for (j = 0; j < (c1 * c2); j++)
    {
        w2[j] = 0.0;
    }

    // Zera os vetores envolvidos aos neur�nios da camada 1.
    for (j = 0; j < c1; j++)
    {
        entrada_camada1[j] = 0.0;
        saida_camada1[j] = 0.0;
        erro_camada1[j] = 0.0;
    }

	// Zera os vetores envolvidos aos neur�nios da camada 2.
    for (j = 0; j < c2; j++)
    {
        entrada_camada2[j] = 0.0;
        saida_camada2[j] = 0.0;
        erro_camada2[j] = 0.0;
    }

    // Randomiza��o dos pesos para a camada 1.
    for (j = 0; j < (cx*c1); j++)
	{
		// 10000 = pesos 0.001, 1000 = pesos 0.01, 100 = pesos 0.1
		w1[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}

	// Randomiza��o dos pesos para a camada 2.
	for (j = 0; j < (c1*c2); j++)
	{
		w2[j] = ((float)(random(200) - 100) * precisao_da_randomizacao) / 10;
	}



//-----------------------------------------------------------------------------//
//                 Grava��o dos Pesos antes do treinamento                     //
//-----------------------------------------------------------------------------//



    // Declara��o do arquivo que armazena os pesos iniciais da rede.
    FILE *fp;
	fp = fopen("pesos_rand.txt","wt");

	// Grava os pesos da camada 1.
	fprintf(fp,"Pesos Camada 1\n");
    for (j = 0; j < (cx * c1); j++)
    {
        fprintf(fp,"%f\n",w1[j]);
    }

    // Grava os pesos da camada 2.
	fprintf(fp,"Pesos Camada 2\n\n");
    for (j = 0; j < (c1 * c2); j++)
    {
        fprintf(fp,"%f\n",w2[j]);
    }

    // Fecha o ponteiro do arquivo.
    fclose(fp);


	// Declara��o do arquivo que armazena os error de treinamento.
	fp = fopen("treinamento.txt","wt");

	// Grava os padr�es de entrada utilizados no treinamento.
	fprintf(fp,"Padr�es de Entrada Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < cx; j++)
		{
			fprintf(fp,"%10.2f\t",p[i][j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Grava os padr�es desejados utilizados no treinamento.
	fprintf(fp,"Padr�es Desejados Utilizados\n");
	for (i = 0; i < padroes; i++)
	{
		for (j = 0; j < c2; j++)
		{
			fprintf(fp,"%10.2f\t",d[i][j]);
		}
		fprintf(fp,"\n");
	}
	fprintf(fp,"\n");

	// Informa��es dos par�metros utilizados no treinamento.
	fprintf(fp,"Parametros de Configura��o da Rede\n");
	fprintf(fp,"Padr�es............: %d\n", 	padroes);
	fprintf(fp,"Fun��o utilizada...: %d\n", 	funcao);
	fprintf(fp,"Taxa de Aprendizado: %10.4f\n", taxa_aprendizado);
	fprintf(fp,"Erro m�nimo........: %10.4f\n", ERRO);
	fprintf(fp,"Momentum...........: %10.4f\n", MOMENTUM);
	fprintf(fp,"�pocas.............: %d\n\n", 	epocas);
	fprintf(fp,"�pocas de Treinamento - Erro de Propaga��o\n");


//-----------------------------------------------------------------------------//
//       Inicio do treinamento da rede (propaga��o dos padr�es pela rede)      //
//-----------------------------------------------------------------------------//


	while(contador != epocas && !MyThread->Terminated)
	{
		contador++;

		// Propaga os k padr�es pela rede.
		for (k = 0; k < padroes; k++)
		{
			//C�lculo para camada C1.
			n = 0;
			for (j = 0; j < c1; j++)
			{
				soma = 0;
				for (i = 0; i < cx; i++)
				{
					soma += w1[n] * p[k][i];
					n += 1;
				}
				entrada_camada1[j] = soma;
				saida_camada1[j] = net->funcao_ativacao(entrada_camada1[j],funcao,curva);
			}

			//C�lculo para camada C2.
			n = 0;
			for (j = 0; j < c2; j++)
			{
				soma = 0;
				for (i = 0; i < c1; i++)
				{
					soma += w2[n] * saida_camada1[i];
					n += 1;
				}
				entrada_camada2[j] = soma;
				saida_camada2[j] = net->funcao_ativacao(entrada_camada2[j],funcao,curva);
			}



//-----------------------------------------------------------------------------//
//                      C�lculo do Erro Quadr�tico                             //
//-----------------------------------------------------------------------------//



			//Calculo do Erro Quadratico.
			erro_quadratico = 0;
			for(j = 0; j < c2; j++)
			{
				erro_quadratico += pow((d[k][j] - saida_camada2[j]),2);
			}

			//Acumula o Erro Quadratico (Criterio de Parada).
			erro_medio_quadratico += (0.5 * erro_quadratico);



//-----------------------------------------------------------------------------//
//                  Retropropaga��o do Erro Pela Rede                          //
//-----------------------------------------------------------------------------//



			//Calculo do erro para cada neur�nio da camada 2.
			for (i = 0; i < c2; i++)
			{
				erro_camada2[i] = (d[k][i] - saida_camada2[i]) * net->derivada(entrada_camada2[i],funcao,curva);
			}

			//Atualizacao dos pesos para camada 2.
			for (i = 0; i < c1; i++)
			{
				n = 0;
				for (j = 0; j < c2; j++)
				{
					dw2[n + i] = taxa_aprendizado * saida_camada1[i] * erro_camada2[j] + (MOMENTUM * dw2[n + i]);
					w2[n + i] = w2[n + i] + dw2[n + i];
					n += c1;
				}
			}

			//Calculo do erro para cada neur�nio da camada 1.
			for (i = 0; i < c1; i++)
			{
				n = 0;
				soma = 0;
				for (j = 0; j < c2; j++)
				{
					soma += (erro_camada2[j] * w2[n + i]);
					n += c1;
				}
				erro_camada1[i] = soma * net->derivada(entrada_camada1[i],funcao,curva);
			}

			//Atualizacao dos pesos para camada 1.
			for (i = 0; i < cx; i++)
			{
				n = 0;
				for (j = 0; j < c1; j++)
				{
					dw1[n + i] = taxa_aprendizado * p[k][i] * erro_camada1[j] + (MOMENTUM * dw1[n + i]);
					w1[n + i] = w1[n + i] + dw1[n + i];
					n += cx;
				}
			}
		}


//-----------------------------------------------------------------------------//
//          C�lculo do erro m�dio quadr�tico da �poca de treinamento           //
//-----------------------------------------------------------------------------//


		erro_medio_quadratico = (1.0 / padroes) * erro_medio_quadratico;
		printf("%d\t%10.12f\n", (int) contador, erro_medio_quadratico);
		fprintf(fp,"%d\t%10.12f\n", (int) contador, erro_medio_quadratico);


		// Plotagem dos dados sincronizados com a thread.
		Synchronize(FmRna->AtualizaGrafico);


		erro_medio_quadratico = 0;

	}

	// Fecha o ponteiro do arquivo de erros de treinamento.
	fclose(fp);


//-----------------------------------------------------------------------------//
//                   Grava��o dos Pesos Ap�s treinamento                       //
//-----------------------------------------------------------------------------//


	// Declara��o do arquivo que armazena os pesos de treinamento da rede.
	fp = fopen("pesos_treino.txt","wt");

	// Grava os pesos da camada 1.
	fprintf(fp,"\tPesos Camada 1\n");
	for (j = 0; j < (cx * c1); j++)
	{
		fprintf(fp,"\tw1[%d] = %f;\n",j, w1[j]);
	}

	// Grava os pesos da camada 2.
	fprintf(fp,"\n\tPesos Camada 2\n");
	for (j = 0; j < (c1 * c2); j++)
	{
		fprintf(fp,"\tw2[%d] = %f;\n",j, w2[j]);
	}

	// Fecha o ponteiro do arquivo dos pesos de treinamento da rede.
	fclose(fp);

}

//---------------------------------------------------------------------------

void __fastcall TFmRna::AtualizaGrafico()
{
	// Plota as amostras no gr�fico.
	FmRna->Memo1->Lines->Add(FloatToStrF(erro_medio_quadratico,ffFixed,10,6));
	FmRna->Chart1->Series[0]->AddY(erro_medio_quadratico);

}

//---------------------------------------------------------------------------


void __fastcall TFmRna::ListBox1Click(TObject *Sender)
{

	for (unsigned int a = 0; a < 100; a++)
	{
		Chart2->Series[0]->YValues->Value[a] = p[ListBox1->ItemIndex][a];
	}

	Chart2->Refresh();



//-----------------------------------------------------------------------------//
//                   Teste da Rede Neural Ap�s treinamento                     //
//-----------------------------------------------------------------------------//


	//C�lculo para camada C1.
	n = 0;
	for (j = 0; j < c1; j++)
	{
		soma = 0;
		for (i = 0; i < cx; i++)
		{
			soma += w1[n] * p[ListBox1->ItemIndex][i];
			n += 1;
		}
		entrada_camada1[j] = soma;
		saida_camada1[j] = net->funcao_ativacao(entrada_camada1[j],funcao,curva);


		// Formata a sa�da em bin�rio.
		if (saida_camada1[j] < 0.5)
		{
			saidas_formatadas_c1[j] = 0;
		}else{
			saidas_formatadas_c1[j] = 1;
		}


	}

	//C�lculo para camada C2.
	n = 0;
	for (j = 0; j < c2; j++)
	{
		soma = 0;
		for (i = 0; i < c1; i++)
		{
			soma += w2[n] * saida_camada1[i];
			n += 1;
		}
		entrada_camada2[j] = soma;
		saida_camada2[j] = net->funcao_ativacao(entrada_camada2[j],funcao,curva);


		// Formata a sa�da em bin�rio.
		if (saida_camada2[j] < 0.5)
		{
			saidas_formatadas_c2[j] = 0;
		}else{
			saidas_formatadas_c2[j] = 1;
		}

	}


	// Formata��o dos neur�nios da camada 1
	if (saidas_formatadas_c1[0] > 0.5)
		Shape1->Brush->Color = clRed;
	else
		Shape1->Brush->Color = clWhite;

	if (saidas_formatadas_c1[1] > 0.5)
		Shape2->Brush->Color = clRed;
	else
		Shape2->Brush->Color = clWhite;

	if (saidas_formatadas_c1[2] > 0.5)
		Shape3->Brush->Color = clRed;
	else
		Shape3->Brush->Color = clWhite;

	if (saidas_formatadas_c1[3] > 0.5)
		Shape4->Brush->Color = clRed;
	else
		Shape4->Brush->Color = clWhite;

	if (saidas_formatadas_c1[4] > 0.5)
		Shape5->Brush->Color = clRed;
	else
		Shape5->Brush->Color = clWhite;

	if (saidas_formatadas_c1[5] > 0.5)
		Shape6->Brush->Color = clRed;
	else
		Shape6->Brush->Color = clWhite;

	if (saidas_formatadas_c1[6] > 0.5)
		Shape7->Brush->Color = clRed;
	else
		Shape7->Brush->Color = clWhite;

	if (saidas_formatadas_c1[7] > 0.5)
		Shape8->Brush->Color = clRed;
	else
		Shape8->Brush->Color = clWhite;

	if (saidas_formatadas_c1[8] > 0.5)
		Shape9->Brush->Color = clRed;
	else
		Shape9->Brush->Color = clWhite;

	if (saidas_formatadas_c1[9] > 0.5)
		Shape10->Brush->Color = clRed;
	else
		Shape10->Brush->Color = clWhite;

	if (saidas_formatadas_c1[10] > 0.5)
		Shape11->Brush->Color = clRed;
	else
		Shape11->Brush->Color = clWhite;

	if (saidas_formatadas_c1[11] > 0.5)
		Shape12->Brush->Color = clRed;
	else
		Shape12->Brush->Color = clWhite;

	if (saidas_formatadas_c1[12] > 0.5)
		Shape13->Brush->Color = clRed;
	else
		Shape13->Brush->Color = clWhite;

	if (saidas_formatadas_c1[13] > 0.5)
		Shape14->Brush->Color = clRed;
	else
		Shape14->Brush->Color = clWhite;

	if (saidas_formatadas_c1[14] > 0.5)
		Shape15->Brush->Color = clRed;
	else
		Shape15->Brush->Color = clWhite;


	// Formata��o dos neur�nios da camada 2
	if (saidas_formatadas_c2[0] > 0.5)
		Shape16->Brush->Color = clRed;
	else
		Shape16->Brush->Color = clWhite;

	if (saidas_formatadas_c2[1] > 0.5)
		Shape17->Brush->Color = clRed;
	else
		Shape17->Brush->Color = clWhite;

	if (saidas_formatadas_c2[2] > 0.5)
		Shape18->Brush->Color = clRed;
	else
		Shape18->Brush->Color = clWhite;

	if (saidas_formatadas_c2[3] > 0.5)
		Shape19->Brush->Color = clRed;
	else
		Shape19->Brush->Color = clWhite;

	if (saidas_formatadas_c2[4] > 0.5)
		Shape20->Brush->Color = clRed;
	else
		Shape20->Brush->Color = clWhite;











}
//---------------------------------------------------------------------------



