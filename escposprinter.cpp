/*******************************************
 * see header-file for further informations
 ********************************************/

#include "Arduino.h"
#include "escposprinter.h"

static const char LF = 0x0A;
static const char GS = 0x1D;
static const char DLE = 0x10;
static const char EOT = 0x04;
static const char ESC = 0x1B;
static const char FS = 0x1C;  
  
EscPos::EscPos(int rxPin, int txPin)
{
  this->_rxPin = rxPin;
  this->_txPin = txPin;
  this->start();
}

void EscPos::start(){
  pinMode(this->_txPin, OUTPUT);
  pinMode(this->_rxPin, INPUT);  
  this->_printer = new SoftwareSerial (this->_rxPin, this->_txPin);
  this->_printer->begin(9600);
}

int EscPos::read(){
    int result;
    result = this->_printer->read();
    return result;
}
size_t EscPos::write(uint8_t c) {
  this->_printer->write(c);
  return 1;
}
size_t EscPos::cwrite(char c) {
  this->_printer->write(c);
  return 1;
}

// query error of printer. return 0 if ok, 4 for cover open, 32 for paper end, 64 for other error.
int EscPos::getStatus(){
  this->write(DLE);    
  this->write(EOT);  
  this->write(0x02);
  int result;
  result = this->_printer->read() - 18;
  return result;
}

// Print and feed n lines
void EscPos::feed(uint8_t n){
  this->write(ESC);  
  this->write('d');
  this->write(n);    
}
// Print one line
void EscPos::feed(){
  this->feed(1);    
}

// Print 80mm paper
void EscPos::set80mm(){
  this->write(GS);  
  this->write('W');
  this->write(64);      
  this->write(2); 
}
// Print 58mm paper
void EscPos::set58mm(){
  this->write(GS);  
  this->write('W');
  this->write(164);      
  this->write(1); 
}

// Set line spacing to n/180-inch
void EscPos::lineSpacing(uint8_t n = 60){
  this->write(ESC);  
  this->write('3');
  this->write(n);  
}
// Select default line spacing to 1/6 inch (n=60)
void EscPos::defaultLineSpacing(){
  this->write(ESC);  
  this->write('2');
}

// Select an international character set
//  0 = U.S.A. 
//  1 = France 
//  2 = Germany 
//  3 = U.K. 
//  4 = Denmark I 
//  5 = Sweden 
//  6 = Italy 
//  7 = Spain 
//  8 = Japan 
//  9 = Norway 
// 10 = Denmark II 
// 11 = Spain II
// 12 = Latin America
// 13 = Korea
// 14 = Slovenia / Croatia
// 15 = China
void EscPos::characterSet(uint8_t n = 0){
  this->write(ESC);  
  this->write('R');
  this->write(n);  
}
//0 CP437 [美国，欧洲标准] 24 CP737 [希腊]
//2 CP850 [多语言] 26 泰文
//3 CP860 [葡萄牙] 27 CP720[阿拉伯语]
//4 CP863 [加拿大-法语] 28 CP855
//5 CP865 [北欧] 29 CP857[土耳其语]
//6 WCP1251 [斯拉夫语] 30 WCP1250[中欧]
//7 CP866 斯拉夫2 31 CP775
//8 MIK[斯拉夫/保加利亚] 32 WCP1254[土耳其语]
//9 CP755 [东欧，拉脱维亚 2] 33 WCP1255[希伯来语]
//10 [伊朗，波斯] 34 WCP1256[阿拉伯语]
//11 保留 35 WCP1258[越南语]
//12 保留 36 ISO-8859-2[拉丁语2]
//13 保留 37 ISO-8859-3[拉丁语3]
//14 保留 38 ISO-8859-4[波罗的语]
//15 CP862 [希伯来] 39 ISO-8859-5[斯拉夫语]
//18 CP852 [拉丁语 2] 42 ISO-8859-8[希伯来语]
//19 CP858 [多种语言拉丁语 1+欧符] 43 ISO-8859-9[土耳其语]
//20 伊朗Ⅱ[波斯语] 44 ISO-8859-15[拉丁语9]
//21 拉脱维亚 45 [泰文2]
//22 CP864 [阿拉伯语] 46 CP856
//23 ISO-8859-1 [西欧] 47 Cp874
//255 GBK2312
void EscPos::codeSet(uint8_t n = 0){
  this->write(ESC);  
  this->write('t'); 
  this->write(2);   
}
void EscPos::setChinOff(){
  this->write(FS);  
  this->write('.');
}
void EscPos::setChinUtf(){
    this->write(ESC);  
    this->write('9');
    this->write('1');
}

void EscPos::effectDoubleHeight(){
  this->write(ESC);    
  this->write('!');  
  this->write(16);
}
void EscPos::effectBold(){
  this->write(ESC);  
  this->write('!');    
  this->write(8);
}
void EscPos::effectDoubleWidth(){
  this->write(ESC);  
  this->write('!');    
  this->write(32);
}
void EscPos::effectUnderline() {
  this->write(ESC);  
  this->write('!');    
  this->write(128);
}
void EscPos::effectOff(){
  this->write(ESC);  
  this->write('!');    
  this->write(0);
}

void EscPos::reverseOn() {
  this->write(GS);  
  this->write('B');
  this->write(1);
}
void EscPos::reverseOff() {
  this->write(GS);  
  this->write('B');    
  this->write(0);
}

void EscPos::justifyLeft() {
  this->write(ESC);  
  this->write('a');
  this->write(0);
}
void EscPos::justifyCenter() {
  this->write(ESC);  
  this->write('a');    
  this->write(1);
}
void EscPos::justifyRight() {
  this->write(ESC);  
  this->write('a');    
  this->write(2);
}

// barcode
void EscPos::barcodeHeight() {
  this->write(GS);  
  this->write('h');    
  this->write(162);
}
void EscPos::barcodeWidth() {
  this->write(GS);  
  this->write('w');    
  this->write(3);
}
// n range 0-3 - 0=no 1=above 2=below 3=both
void EscPos::barcodeTextPosition(uint8_t n = 2) {
  this->write(GS);  
  this->write('H');    
  this->write(n);
}
void EscPos::barcodeStartPrint() {
  this->write(GS);  
  this->write('k');    
  this->write(4);
}
// use EscPos->print("barcode")
void EscPos::barcodeEndPrint() {
  this->write(0x00);
}

// qrcode - len is strng length
void EscPos::qrcodeStartPrint(uint8_t len) {
  this->write(GS);  
  this->write('(');    
  this->write('k');
  this->write(4);
  this->write(0);
  this->write(49);
  this->write(65);
  this->write(50);
  this->write(0);
  this->write(GS);  
  this->write('(');    
  this->write('k');
  this->write(3);
  this->write(0);
  this->write(49);
  this->write(67);
  this->write(3);
  this->write(GS);  
  this->write('(');    
  this->write('k');
  this->write(3);
  this->write(0);
  this->write(49);
  this->write(69);
  this->write(48);
  this->write(GS);  
  this->write('(');    
  this->write('k');
  len = len + 3;
  uint8_t ll = len % 256;
  len = (uint8_t)(len / 256);
  uint8_t lh = len % 256;
  this->write(ll);
  this->write(lh);
  this->write(49);
  this->write(80);
  this->write(48);
}
// use EscPos->print("qrcode")
void EscPos::qrcodeEndPrint() {
  this->write(GS);  
  this->write('(');    
  this->write('k');
  this->write(3);
  this->write(0);
  this->write(49);
  this->write(81);
  this->write(48);
}

void EscPos::partialCut() {
  this->write(GS);
  this->write('V');
  this->write(66);
  this->write(0xA); // print buffer and line feed
}
void EscPos::fullCut() {
  this->write(GS);
  this->write('V');
  this->write(65);
  this->write(0xA); // print buffer and line feed
}

