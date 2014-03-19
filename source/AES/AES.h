#pragma once

#include <string> 
  
typedef unsigned char byte; 
typedef unsigned long word; 
  
class AES { 
  
    private: 
  
        static byte sbox[ 16 ][ 16 ]; 
        static byte inv_sbox[ 16 ][ 16 ]; 
        static word rcon[ 52 ]; 
  
        byte key_length; 
        byte num_rounds; 
  
        word *w; 
        byte state[ 4 ][ 4 ]; 
  
        static byte gmul( byte a, byte b); 
        static void rot_word( word *b ); 
        static void sub_word( word *b ); 
                
        bool set_key( char *key ); 
        void expand_key( byte *key ); 
  
        void add_round_key( byte round ); 
        
        void shift_rows( void ); 
        void inv_shift_rows( void ); 
        
        void mix_columns( void ); 
        void inv_mix_columns( void ); 
  
        void cipher( void ); 
        void inv_cipher( void ); 
        
    public: 
  
        size_t encrypt( char **data, size_t length, char *key ); 
        size_t decrypt( char **data, size_t length, char *key ); 
  
        AES(); 
        ~AES(); 
  
}; 
