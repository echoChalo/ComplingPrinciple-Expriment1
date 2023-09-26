#include <iostream>
#include "Experiment1.h"
#define Bit 1<<3

using namespace std;

/* 
This is a simple test text.
Here is the text.
*/
int main()
{// 判断i是否大于3
   int i;
   char a = 'y';
   char b = 'n';
   char c = '\\', *d;
   cin>>i;
   if (i>3) cout<<"ok";
   else cout<<"wrong";
   
   switch(b){
      case 'y':
         cout<<"a 等于 b"<<endl;
         break;
      default:
         cout<<"a is not equal b"<<endl;
         break;
   } 

   if(a==c?0:1){cout<<"a！=c"<<endl;}
   else cout<<"a==c"<<endl;

   double d = 1.01e+2, e = 2.01E-3, f = -9.5;
   int hex = 0x1a2b3c4d;
   int otc = 01234567;
   cout << hex*otc << endl;

   while (true) {
       cout << d << endl;
       if (Bit)break;
   }

   int m = 10, n = 9;
   cout << m & n << m | n << m ^ n << endl;

   string str = "aaaadadadada";
   for (int i = 0; i < str.length(); i++) {
       cout << str[i];
   }

   return 0;
}