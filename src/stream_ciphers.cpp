#include <iostream>
#include <cmath>
#ifndef MARMOSET_TESTING
int main();
#endif


char *encode( char *plaintext, unsigned long key );
char *decode( char *cihpertext, unsigned long key );

//encodes the inputed text
char *encode( char *plaintext, unsigned long key ) {
	//creates a state array with characters from 0 to 255
	unsigned char stateArray[256] {};
	for (unsigned int k = 0; k < 256; ++k){
		stateArray[k] = k;
	}


	//takes the key and turns it into a binary array
	unsigned int bitKey[64]{0};
	unsigned int mask = 1;
	unsigned int count = 0;
	while (key > 0){
		bitKey[count] = key%2;
		key = key/2;
		count += 1;
	}


	//determines the length of the inputed string
	unsigned int length = 0;
	while (plaintext[length] != '\0'){
		length += 1;
	}
	//creates a new array that holds the string but length of multiple of 4
	unsigned int newLength = length;
	unsigned char newPlainText[length];

	if (length%4 != 0){
		newLength += (4 - length%4);
	}
	for (unsigned int k = 0; k < newLength; ++k){
		newPlainText[k] = '\0';
	}
	for (unsigned int k = 0; k < length; ++k) {
		newPlainText[k] = plaintext[k];
	}

	//scrambles the state array and sets values of j an i for scrambling
	unsigned int i = 0;
	unsigned int j = 0;
	for (unsigned int n = 0; n < 256; ++n) {
		int k = i%64;
		j = (j + stateArray[i] + bitKey[k])%256;
		unsigned char temp = stateArray[j];
		stateArray[j] = stateArray[i];
		stateArray[i] = temp;
		i = (i+1)%256;
	}

	//array for characters after scrambling
	unsigned char xored[newLength] {'\0'};
	//scrambles the state array and xor's the original character with a character R
	for (unsigned int n = 0; n < newLength; ++n) {
		i = (i+1)%256;
		j = (j + stateArray[i])%256;
		unsigned char temp = stateArray[j];
		stateArray[j] = stateArray[i];
		stateArray[i] = temp;
		unsigned char r = (stateArray[i] + stateArray[j])%256;
		unsigned char R = stateArray[r];
		xored[n] = newPlainText[n] ^ R;

	}


	//takes the xor values and applies ascii armor
	unsigned int askiDECIMAL[newLength/4] = {0};
	//loops through each set of four characters to convert to ascii armor
	for (unsigned int k = 0; k < newLength/4 ; ++k){
		//converts the characters to their binary values and combines it into one long binary
		unsigned int binAski[32] = {0};
		for (int n = 0; n < 4; ++n){
			for (int p = 7; p > -1; --p) {
				if ((xored[n + 4*k] & (mask << p)) >> p){
					binAski[n*8 + 7-p] = 1;
				}
			}
		}
		//turns the long binary number into the decimal value
		unsigned int b = 0;
		for (unsigned int a = 0; a < 32; a++){
			if (binAski[a] == 1) {
				b += pow(2, 31-a);
			}
		}
		askiDECIMAL[k] = b;

	}

	//create length and arrays for encoded text
	unsigned int askiLength = (newLength * 5 / 4) + 1;
	char *asky = new char[askiLength+5];
	for (unsigned int why = 0; why < askiLength+5; why++){
		asky[why] = '\0';
	}
	unsigned int harder = 0;
	//turns the 32 bit value into base 85 and adds 33 to create array of ascii armor characters
	for (unsigned int c = 0; c < newLength/4; ++c){
		for (int myLove = 0; myLove < 5; myLove++){
			int dog = pow(85, 4-myLove);
			asky[harder] = askiDECIMAL[c]/dog + 33;
			askiDECIMAL[c] -= dog * (askiDECIMAL[c]/dog);
			harder += 1;
		}
	}

	return asky;
}

//function to decode the text
char *decode( char *cihpertext, unsigned long key ){
	//calculates the length of the cipher text
	unsigned int length = 0;
	while (cihpertext[length] != '\0' ){
		length++;
	}


	unsigned int bit32[length/5] {0};
	unsigned char finalVal[length/5 * 4] {'\0'};
	//takes each set of 5 characters and converts them back into the original 4 characters
	for (unsigned int k = 0; k < length/5; k++){
		unsigned int binary[32] {0};
		//combines the group of 5 characters into one 32 bit value
		for (int lau = 0; lau < 5; ++lau){
			bit32[k] += (cihpertext[k*5 + lau] - 33) * pow(85, 4-lau);
		}


		//turns the 32 bit value into one long binary array
		for (int h = 0; h < 32; ++h){
			unsigned int l = bit32[k] / pow(2, 31-h);
			if (l != 0){
				bit32[k] -= pow(2, 31-h);
				binary[h] = l;

			}

		}


		//splits the binary array into 4 parts and converts it back into decimal to give the value of each character
		for (int c = 0; c < 4; c++){
			unsigned int holdup = 0;
			for (int f = 0; f < 8; ++f){
				holdup += binary[c*8 + f] * pow(2, 7-f);
			}
			finalVal[c + k*4] = holdup;

		}

	}


	//creates state array of characters from 0 to 255
	unsigned char stateArray[256] {};
	for (unsigned int k = 0; k < 256; ++k){
		stateArray[k] = k;
	}

	//takes the key and turns it into a binary array
	unsigned int bitKey[64]{0};
	unsigned int count = 0;

	while (key > 0){
		bitKey[count] = key%2;
		key = key/2;
		count += 1;
	}


	//scrambles the state array and sets values of j an i for scrambling
	unsigned int i = 0;
	unsigned int j = 0;
	for (unsigned int n = 0; n < 256; ++n) {
		int k = i%64;
		j = (j + stateArray[i] + bitKey[k])%256;
		unsigned char temp = stateArray[j];
		stateArray[j] = stateArray[i];
		stateArray[i] = temp;
		i = (i+1)%256;
	}



	//array for returning the decoded text
	char *decipher = new char[length/5*4];
	//scrambles the state array and uses those values to undo the xor and return original value.
	for (unsigned int n = 0; n < (length/5*4); ++n) {
		i = (i+1)%256;
		j = (j + stateArray[i])%256;
		unsigned char temp = stateArray[j];
		stateArray[j] = stateArray[i];
		stateArray[i] = temp;
		unsigned char r = (stateArray[i] + stateArray[j])%256;
		unsigned char R = stateArray[r];
		decipher[n] = finalVal[n] ^ R;

	}

	//prevent random characters to be printed
	decipher[length/5*4] = '\0';
	return decipher;


}





#ifndef MARMOSET_TESTING
int main() {
	unsigned long code = 1234;
	char plaintext[] {"testing"};
	char *ciphertext {encode( plaintext, code)};
	std::cout << ciphertext << std::endl;
	char *cipertext {decode (ciphertext, code)};
    std::cout << cipertext << std::endl;
	return 0;
}
#endif
