#include "pRNG.h"
#include <Arduino.h>

unsigned int Firstprime, Secondprime;
unsigned int Field, phin, Enc, Dec, Privatekey, Publickey;
bool Hasrun= false;
unsigned long time=millis();
unsigned long Text = 123456;


pRNG prn;


unsigned int modMult(unsigned int a, unsigned int b, unsigned int mod) //modulo multiplication function
{ 
    unsigned int res = 0; // Initialize result 
    a = a % mod; 
    while (b > 0) 
    { 
        // If b is odd, add 'a' to result 
        if (b % 2 == 1) 
            res = (res + a) % mod; 
  
        // Multiply 'a' with 2 
        a = (a * 2) % mod;   
        // Divide b by 2 
        b /= 2; 
    } 
  
    // Return result 
    return res%mod;
  
  

} 
bool prime(unsigned int number) //primality check for prime numbers
{
   
     for (unsigned int i = 2; i <=sqrt(number); ++i) 
        {
            if (number % i == 0) 
            {
                return false;
            }
         }
        return true;
  }
  

unsigned int PRN()   //generation of a prime random number
{
 unsigned int n1;
 do
  { 
    n1= prn.getRndByte();
    //n1= random(100);
   }while(n1==0||prime(n1)==false); 
   return n1;
}
unsigned int gcd(unsigned int a, unsigned int b) //function to check GCD
{ 
    unsigned int temp; 
    while (1) 
    { 
        temp = a%b; 
        if (temp == 0) 
         return b; 

       a = b; 
       b= temp;
         
    } 
} 
unsigned int E_gen(unsigned int n, unsigned int phi)   //publickey generation e
{
    for(unsigned int i=2; i<n; i++)
     {
       if(gcd(i,n)==1 && gcd(i,phi)==1)
       {
         return i;
        }
        
     }
   Serial.println("Public key generated");
 }

unsigned int D_gen(unsigned int en, unsigned int phi) //privatekey generation d
{
  for(unsigned int i=2; i<phi; i++)
  {
    if(modMult(en,i,phi)==1)
          return i;
           
  }
   Serial.println("Private key generated");
  }
unsigned int power(unsigned int base, unsigned int expo, unsigned int mod)  
{  
    
   if(expo==0)
    return 1;
   unsigned long z=base%mod;
   for(unsigned long i=1; i<expo;i++)
    {
      z=(z*base)%mod;
    }
     return z;
    /* if (expo==0)
      return 1;
     else
      {
       unsigned int R= power(base,expo/2, mod);
         if (expo%2==0)
          return (R*R)%mod;
         else
            return (R*base*R)%mod;
      }*/
      
} 
void setup()
{
   Serial.begin(9600);
   
   //randomSeed(analogRead(A0));
    Firstprime=PRN();
   Serial.println(Firstprime);
   do
   {  
   Secondprime=PRN();
   Serial.println(Secondprime);
    }while(Firstprime==Secondprime); 
  Field=Firstprime*Secondprime;
  phin=(Firstprime-1)*(Secondprime-1);
  Publickey=E_gen(Field, phin);
  Privatekey=D_gen(Publickey,phin);  
  
}

void loop()
{
  if(Hasrun==false)
  {
    
    Serial.print("Public key is:");
    Serial.println(Publickey);
    Serial.print("Private key is:");
    Serial.println(Privatekey);
    Serial.println("Encrypting....");
    Enc= power(Text,Publickey, Field);
    Serial.println(Enc);
    Serial.println("Decrypting...");
    Dec=power(Enc,Privatekey, Field);
    Serial.println(Dec);
    Serial.println(millis()-time);
    Hasrun=true;
      
  }
}
