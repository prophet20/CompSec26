#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define MAX_MATCHES 500

typedef struct {
    int length;
    int count;
} Candidate;

void remove_spaces(char *str){
    int count = 0;

    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i]; 
                                   
    str[count] = '\0';
}

void normalize_text(char* plaintext, char* output){
	for (int i = 0; i < strlen(plaintext); i++) 
	{
		if ( isalpha(plaintext[i]) ) 
		{
			output[i] = toupper(plaintext[i]);
		}
		else 
		{
			output[i] = ' ';
		}
	}
	
	remove_spaces(output);
}

void analyze_kasiski(const char *text) {
    int len = strlen(text);
    int distances[MAX_MATCHES];
    int match_count = 0;

    for (int i = 0; i < len - 3; i++) {
        for (int j = i + 3; j < len - 3; j++) {
            if (strncmp(&text[i], &text[j], 3) == 0) {
                int d = j - i;
				if (match_count < MAX_MATCHES) {
					distances[match_count++] = d;
					printf("Found: \"%c%c%c\" on Pos %d & Pos %d - > Distance: %d\n", 
                    text[i], text[i+1], text[i+2], i, j, d);
				}
            }
        }
    }
    
    int votes[21] = {0};
	for (int i = 0; i < match_count; i++) {
		for (int len = 2; len <= 20; len++)
		{
			if (distances[i] % len == 0) votes[len]++; 
		}
	}

	printf("\n--- Results for key length votes ---\n");
	for (int len = 2; len <= 20; len++){
		if (votes[len] > 0) printf("Length %2d: %d votes\n", len, votes[len]);
	}
	
	Candidate results[21];

	for (int i = 2; i <= 20; i++) {
		results[i].length = i;
		results[i].count = votes[i];
	}

	for (int i = 2; i < 20; i++) {
		for (int j = i + 1; j <= 20; j++) {
			if (results[i].count < results[j].count) {
				Candidate temp = results[i];
				results[i] = results[j];
				results[j] = temp;
			}
		}
	}

	printf("Top 3 lengths:\n");
	for (int i = 2; i < 5; i++) {
		printf("%d: Length %d (%d votes)\n", i-1, results[i].length, results[i].count);
	}
}
	
int main()
{
	//char plaintext[1000]; = "Lfwki mjc lps iswk hj oglkmvguragk m kmxmamjc vxwuyl: Ggi isw al xa eycx mf km kbqbdc laef lfwki mjc guzugskec zg bwymoacfvmqk, yfw xtw mlaid oyq bw fg ksdi ul qg vsyhjavefwb laef lfwki mjc fh snnggnw pwdavmqfaaxw. Fzc xbvel kwmlav gk yed emjx huxdavyxl.";
	//char plaintext[1000] = "VCHYIMEWPBGSFGFLVCHYIWAYXAGWHYIMTNCGSFGFLCHZQPSMKILSYXYMSQXWVCHYIMEWPBGSFGFLZQWSMXWVCHYIMEWPBGSFGFLSYXYMSQXWVCHYIMEWPBGSFGFLVCHYIWAYXAGWHYIMTNCGSFGFLCHZQPSMKILSYXYMSQXW";
	//char plaintext[1000] = "TYWUR USHPO SLJNQ AYJLI FTMJY YZFPV EUZTS GAHTU WNSFW EEEVA MYFFD CZTMJ WSQEJ VWXTU QNANT MTIAW AOOJS HPPIN TYDDM VKQUF\LGMLB XIXJU BQWXJ YQZJZ YMMZH DMFNQ VIAYE FLVZI ZQCSS AEEXV SFRDS DLBQT YDTFQ NIVKU ZPJFJ HUSLK LUBQV JULAB XYWCD IEOWH\FTMXZ MMZHC AATFX YWGMF XYWZU QVPYF AIAFJ GEQCV KNATE MWGKX SMWNA NIUSH PFSRJ CEQEE VJXGG BLBQI MEYMR DSDHU UZXVV VGFXV JZXUI JLIRM RKZYY ASETY MYWWJ IYTMJ KFQQT ZFAQK IJFIP FSYAG\QXZVK UZPHF ZCYOS LJNQE MVK";
	//char plaintext[1000] = "HW UIE AWPQLP GG UHP MOJTPV TUAEWT, JN ZJEFR EG GPRX S NPRP HFSFPUU VNTGO,FSESCMIDZ KVSEADF, IYKVSE OGNFSEAD URLFRVIWAUZ, PCGWJDP XPS TSW DPMXGOEEQWODE, AJPNOEW UIE RWOFRLD XFLQSSF, AYV TFCFJF UHP TMFSDAOHS ZX MJBPJUZTZ GVSSPDWFS LFE PUC HPTTPJJUY, OG PSDLAO BNO WTUAMDJTH EZJT CZFTUIEMUJOYXPS TSW VOIEWE TTLLFT OQ SNFRTUB.";
	//char plaintext[1000] = "VQ VGQ RDARKQ QE FJD GPHFGC EVZFGR, UP NDFDD VN RQQY C LATD BGQRGBF WMUQM, QUSMDKUUG VWRFKBQ, KMEWQQ FNYGRFKB FTZZSTUNHFA, ODQUUFD RQQ FJD OQLYQM PGEQPBQ, RQAONFG STG FQPDDCK IGKRCQQ, CMP UDOWQQ VGQ DKQURUPFE QE XKAQTSK VN AWQEGKHGR MPC AWQ BQRFGQUVX, PQ NDFZUP ZZF DEVZNNHEJ STKR OQMEVHFWSUQM RQQ FJD GPHFGC EVZFGR AH ZYGQUEZ.";
	//12
	//char plaintext[1000] = "KW RAA EEBJHT AT LFX QCIGYZ HFOLCL, EC OEXAG FC XMKI P MBLA EQFXCVP JNVIJ, TEHSZEEHH WOOIUQW, GGOJRR XKBQGLGV PGAAKQXXWLW, INDVVXA UAF LFX YDMZIJ SQTWLVA, ERBGKIQ HZC ZACEEUH LQZXYKA, PNQ MARGFW RAA QLRMOXZUK MY HXBRLPN FC GSKOTLIYO PZR GSK LDSGYNXFM, VM HNSAVH WCP SKRTXAIFB PWUG UMGOIIGOPXAB XMK PWE HHEIQR KRTPTS BZ WBQFAAT.";
	//11
	char plaintext[1000] = "KW RAA EEBJHT CX RAA JNVNAS GLYMAH, IA INSSJ RH BDRZ U IDFW NXNUEPN QCWGL, XOIAOFEHV BSLPXCR, CJHIJC WKBEFNER HJYGMJIYCPN, DJMOESE SIN IVW AHIBOA XAUSFAX, LGOZIPT HZC ZACEEUH LSDDTNT, AAX OTQMPX PWE OFAHGALZO DF YCXTFLW MK DUEMAAJWQ TJS OHL LDGLCKEIY, QI KGRSGG WCD RMPPPDGLD IHVM YDBKRBPJTVIJ UCJ RAA JNVNAS GLYMAH OS UITFAAT.";
	normalize_text(plaintext, plaintext);
	
	printf("Full text (only characters):\n%s\n\n", plaintext);
	
	printf("Kasiski:\n");
	analyze_kasiski(plaintext);
	
    return EXIT_SUCCESS;
}
