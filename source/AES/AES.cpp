#include "AES.h" 
  
byte AES::sbox[16][16] = { 
    { 0x63, 0x7C, 0x77, 0x7B, 0xF2, 0x6B, 0x6F, 0xC5, 0x30, 0x01, 0x67, 0x2B, 0xFE, 0xD7, 0xAB, 0x76 }, 
    { 0xCA, 0x82, 0xC9, 0x7D, 0xFA, 0x59, 0x47, 0xF0, 0xAD, 0xD4, 0xA2, 0xAF, 0x9C, 0xA4, 0x72, 0xC0 }, 
    { 0xB7, 0xFD, 0x93, 0x26, 0x36, 0x3F, 0xF7, 0xCC, 0x34, 0xA5, 0xE5, 0xF1, 0x71, 0xD8, 0x31, 0x15 }, 
    { 0x04, 0xC7, 0x23, 0xC3, 0x18, 0x96, 0x05, 0x9A, 0x07, 0x12, 0x80, 0xE2, 0xEB, 0x27, 0xB2, 0x75 }, 
    { 0x09, 0x83, 0x2C, 0x1A, 0x1B, 0x6E, 0x5A, 0xA0, 0x52, 0x3B, 0xD6, 0xB3, 0x29, 0xE3, 0x2F, 0x84 }, 
    { 0x53, 0xD1, 0x00, 0xED, 0x20, 0xFC, 0xB1, 0x5B, 0x6A, 0xCB, 0xBE, 0x39, 0x4A, 0x4C, 0x58, 0xCF }, 
    { 0xD0, 0xEF, 0xAA, 0xFB, 0x43, 0x4D, 0x33, 0x85, 0x45, 0xF9, 0x02, 0x7F, 0x50, 0x3C, 0x9F, 0xA8 }, 
    { 0x51, 0xA3, 0x40, 0x8F, 0x92, 0x9D, 0x38, 0xF5, 0xBC, 0xB6, 0xDA, 0x21, 0x10, 0xFF, 0xF3, 0xD2 }, 
    { 0xCD, 0x0C, 0x13, 0xEC, 0x5F, 0x97, 0x44, 0x17, 0xC4, 0xA7, 0x7E, 0x3D, 0x64, 0x5D, 0x19, 0x73 }, 
    { 0x60, 0x81, 0x4F, 0xDC, 0x22, 0x2A, 0x90, 0x88, 0x46, 0xEE, 0xB8, 0x14, 0xDE, 0x5E, 0x0B, 0xDB }, 
    { 0xE0, 0x32, 0x3A, 0x0A, 0x49, 0x06, 0x24, 0x5C, 0xC2, 0xD3, 0xAC, 0x62, 0x91, 0x95, 0xE4, 0x79 }, 
    { 0xE7, 0xC8, 0x37, 0x6D, 0x8D, 0xD5, 0x4E, 0xA9, 0x6C, 0x56, 0xF4, 0xEA, 0x65, 0x7A, 0xAE, 0x08 }, 
    { 0xBA, 0x78, 0x25, 0x2E, 0x1C, 0xA6, 0xB4, 0xC6, 0xE8, 0xDD, 0x74, 0x1F, 0x4B, 0xBD, 0x8B, 0x8A }, 
    { 0x70, 0x3E, 0xB5, 0x66, 0x48, 0x03, 0xF6, 0x0E, 0x61, 0x35, 0x57, 0xB9, 0x86, 0xC1, 0x1D, 0x9E }, 
    { 0xE1, 0xF8, 0x98, 0x11, 0x69, 0xD9, 0x8E, 0x94, 0x9B, 0x1E, 0x87, 0xE9, 0xCE, 0x55, 0x28, 0xDF }, 
    { 0x8C, 0xA1, 0x89, 0x0D, 0xBF, 0xE6, 0x42, 0x68, 0x41, 0x99, 0x2D, 0x0F, 0xB0, 0x54, 0xBB, 0x16 } 
}; 
  
byte AES::inv_sbox[ 16 ][ 16 ] = { 
    { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb }, 
    { 0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb }, 
    { 0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e }, 
    { 0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25 }, 
    { 0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92 }, 
    { 0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84 }, 
    { 0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06 }, 
    { 0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b }, 
    { 0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73 }, 
    { 0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e }, 
    { 0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b }, 
    { 0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4 }, 
    { 0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f }, 
    { 0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef }, 
    { 0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61 }, 
    { 0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d } 
}; 
  
word AES::rcon[ 52 ] = { 
    0x00000000, 0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 
    0x80000000, 0x1B000000, 0x36000000, 0x6C000000, 0xD8000000, 0xAB000000, 0x4D000000, 0x9A000000, 
    0x2F000000, 0x5E000000, 0xBC000000, 0x63000000, 0xC6000000, 0x97000000, 0x35000000, 0x6A000000, 
    0xD4000000, 0xB3000000, 0x7D000000, 0xFA000000, 0xEF000000, 0xC5000000, 0x91000000, 0x39000000, 
    0x72000000, 0xE4000000, 0xD3000000, 0xBD000000, 0x61000000, 0xC2000000, 0x9F000000, 0x25000000, 
    0x4A000000, 0x94000000, 0x33000000, 0x66000000, 0xCC000000, 0x83000000, 0x1D000000, 0x3A000000, 
    0x74000000, 0xE8000000, 0xCB000000, 0x8D000000 
}; 
  
AES::AES() : key_length( 0 ), num_rounds( 0 ), w( 0 ) 
{ 
    memset( state, 0, 16 ); 
} 
  
AES::~AES() 
{ 
} 
  
byte AES::gmul( byte a, byte b) 
{ 
    unsigned char p = 0; 
    unsigned char hi; 
  
    for( byte i = 0; i < 8; ++i ) { 
        if( ( b & 1 ) == 1 ) 
            p ^= a; 
        hi = ( a & 0x80 ); 
        a <<= 1; 
        if( hi == 0x80 ) 
            a ^= 0x1b;      
        b >>= 1; 
    } 
    return p; 
} 
  
void AES::rot_word( word *b ) 
{ 
    byte tmp[ 4 ]; 
    memcpy( tmp, b, 4 ); 
  
    byte tmpb = tmp[ 3 ]; 
    
    for( byte i = 3; i > 0; --i ) 
        tmp[ i ] = tmp[ i - 1 ]; 
    
    tmp[ 0 ] = tmpb; 
    
    memcpy( b, tmp, 4); 
} 
  
void AES::sub_word( word *b ) 
{ 
    byte tmp[ 4 ]; 
    memcpy( tmp, b, 4 ); 
    
    for( byte i = 0; i < 4; ++i ) 
        tmp[ i ] = sbox[ tmp[ i ] >> 4 ][ tmp[ i ] & 0x0f ]; 
    
    memcpy( b, tmp, 4 ); 
} 
  
void AES::expand_key( byte *key ) 
{ 
    if( w ) 
        delete [] w; 
    
    w = new word[ 4 * ( num_rounds + 1 ) ]; 
    
    for( byte iw = 0, ib = 0; iw < key_length; ++iw, ib += 4 ) 
        w[ iw ] = ( key[ ib ] << 24 ) + ( key[ ib + 1 ] << 16 ) + ( key[ ib + 2 ] << 8 ) + key[ ib + 3 ]; 
    
    word tmp; 
  
    for( byte i = key_length; i < ( 4 * ( num_rounds + 1 ) ); ++i ) { 
  
        tmp = w[ i - 1 ]; 
  
        if( ( i % key_length ) == 0 ) { 
            rot_word( &tmp ); 
            sub_word( &tmp ); 
            tmp ^= rcon[ i / key_length ]; 
        } else if( ( key_length > 6 ) && ( ( i % key_length ) == 4 ) ) { 
            sub_word( &tmp ); 
        } 
  
        w[ i ] = w[ i - key_length ] ^ tmp; 
    } 
} 
  
void AES::add_round_key( byte round ) 
{ 
    byte tmp = 0; 
    word wtmp = 0; 
    for( byte col = 0; col < 4; ++col ) 
        for( byte row = 0; row < 4; ++row ) 
            state[ row ][ col ] ^= ( w[ round * 4 + col ] >> ( 24 - row * 8 ) ) & 0x000000ff; 
} 
  
void AES::shift_rows( void ) 
{ 
    byte tmp = 0; 
    tmp = state[ 1 ][ 0 ]; 
    state[ 1 ][ 0 ] = state[ 1 ][ 1 ]; 
    state[ 1 ][ 1 ] = state[ 1 ][ 2 ]; 
    state[ 1 ][ 2 ] = state[ 1 ][ 3 ]; 
    state[ 1 ][ 3 ] = tmp; 
    
    tmp = state[ 2 ][ 0 ]; 
    state[ 2 ][ 0 ] = state[ 2 ][ 2 ]; 
    state[ 2 ][ 2 ] = tmp; 
    tmp = state[ 2 ][ 1 ]; 
    state[ 2 ][ 1 ] = state[ 2 ][ 3 ]; 
    state[ 2 ][ 3 ] = tmp; 
  
    tmp = state[ 3 ][ 3 ]; 
    state[ 3 ][ 3 ] = state[ 3 ][ 2 ]; 
    state[ 3 ][ 2 ] = state[ 3 ][ 1 ]; 
    state[ 3 ][ 1 ] = state[ 3 ][ 0 ]; 
    state[ 3 ][ 0 ] = tmp; 
} 
  
void AES::inv_shift_rows( void ) 
{ 
    byte tmp = 0; 
    tmp = state[ 3 ][ 0 ]; 
    state[ 3 ][ 0 ] = state[ 3 ][ 1 ]; 
    state[ 3 ][ 1 ] = state[ 3 ][ 2 ]; 
    state[ 3 ][ 2 ] = state[ 3 ][ 3 ]; 
    state[ 3 ][ 3 ] = tmp; 
    
    tmp = state[ 2 ][ 0 ]; 
    state[ 2 ][ 0 ] = state[ 2 ][ 2 ]; 
    state[ 2 ][ 2 ] = tmp; 
    tmp = state[ 2 ][ 1 ]; 
    state[ 2 ][ 1 ] = state[ 2 ][ 3 ]; 
    state[ 2 ][ 3 ] = tmp; 
  
    tmp = state[ 1 ][ 3 ]; 
    state[ 1 ][ 3 ] = state[ 1 ][ 2 ]; 
    state[ 1 ][ 2 ] = state[ 1 ][ 1 ]; 
    state[ 1 ][ 1 ] = state[ 1 ][ 0 ]; 
    state[ 1 ][ 0 ] = tmp; 
} 
  
void AES::mix_columns( void ) 
{ 
    unsigned char a[4]; 
    unsigned char b[4]; 
    unsigned char c = 0; 
    unsigned char h = 0;    
    
    for( byte col = 0; col < 4; ++col ) { 
        
        for( byte row = 0; row < 4; ++row ) { 
        
            a[ row ]    = state[ row ][ col ]; 
            h           = state[ row ][ col ] & 0x80; 
            b[ row ]    = state[ row ][ col ] << 1; 
            
            if( h == 0x80 ) 
                b[ row ] ^= 0x1b; 
        } 
        
        state[ 0 ][ col ] = b[ 0 ] ^ a[ 3 ] ^ a[ 2 ] ^ b[ 1 ] ^ a[ 1 ]; 
        state[ 1 ][ col ] = b[ 1 ] ^ a[ 0 ] ^ a[ 3 ] ^ b[ 2 ] ^ a[ 2 ]; 
        state[ 2 ][ col ] = b[ 2 ] ^ a[ 1 ] ^ a[ 0 ] ^ b[ 3 ] ^ a[ 3 ]; 
        state[ 3 ][ col ] = b[ 3 ] ^ a[ 2 ] ^ a[ 1 ] ^ b[ 0 ] ^ a[ 0 ]; 
    } 
} 
  
void AES::inv_mix_columns( void ) 
{ 
    unsigned char a[4]; 
    
    for( byte col = 0; col < 4; ++col ) { 
  
        for( byte row = 0; row < 4; ++ row ) 
            a[ row ] = state[ row ][ col ]; 
  
        state[ 0 ][ col ] = gmul( a[ 0 ], 14 ) ^ gmul( a[ 3 ], 9 ) ^ gmul( a[ 2 ], 13 ) ^ gmul( a[ 1 ], 11 ); 
        state[ 1 ][ col ] = gmul( a[ 1 ], 14 ) ^ gmul( a[ 0 ], 9 ) ^ gmul( a[ 3 ], 13 ) ^ gmul( a[ 2 ], 11 ); 
        state[ 2 ][ col ] = gmul( a[ 2 ], 14 ) ^ gmul( a[ 1 ], 9 ) ^ gmul( a[ 0 ], 13 ) ^ gmul( a[ 3 ], 11 ); 
        state[ 3 ][ col ] = gmul( a[ 3 ], 14 ) ^ gmul( a[ 2 ], 9 ) ^ gmul( a[ 1 ], 13 ) ^ gmul( a[ 0 ], 11 ); 
    } 
    
} 
  
bool AES::set_key( char *key ) 
{ 
    
    size_t key_len = strlen( key ); 
    
    byte *w_key         = 0; 
    byte des_key_len    = 0; 
  
    if( key_len <= 16 ) { 
  
        key_length = 4; 
        num_rounds = 10; 
        des_key_len = 16; 
    
    } else if( key_len <= 24 ) { 
  
        key_length = 6; 
        num_rounds = 12; 
        des_key_len = 24; 
    
    } else if( key_len <= 32 ) { 
        
        key_length = 8; 
        num_rounds = 14; 
        des_key_len = 32; 
    
    } else false; 
  
    w_key = new byte[ des_key_len ]; 
  
    for( byte i = 0, t = 0; t < des_key_len; ++i, ++t ) { 
        if( i == key_len ) 
            i = 0; 
        *( w_key + t ) = *( key + i ); 
    } 
        
    expand_key( w_key ); 
    
    delete [] w_key; 
  
    return true; 
} 
  
void AES::cipher( void ) 
{ 
    add_round_key( 0 ); 
  
    for( byte round = 1; round < num_rounds; ++round ) { 
        
        for( byte row = 0; row < 4; ++row ) 
            for( byte col = 0; col < 4; ++col ) 
                state[ row ][ col ] = sbox[ state[ row ][ col ] >> 4 ][ state[ row ][ col ] & 0x0f ]; 
  
        shift_rows( ); 
        mix_columns( ); 
        add_round_key( round ); 
    } 
    
    for( byte row = 0; row < 4; ++row ) 
        for( byte col = 0; col < 4; ++col ) 
            state[ row ][ col ] = sbox[ state[ row ][ col ] >> 4 ][ state[ row ][ col ] & 0x0f ]; 
  
    shift_rows( ); 
    add_round_key( num_rounds ); 
} 
  
void AES::inv_cipher( void ) 
{ 
    add_round_key( num_rounds ); 
  
    for( byte round = ( num_rounds - 1 ); round > 0; --round ) { 
        
        inv_shift_rows( ); 
        
        for( byte row = 0; row < 4; ++row ) 
            for( byte col = 0; col < 4; ++col ) 
                state[ row ][ col ] = inv_sbox[ state[ row ][ col ] >> 4 ][ state[ row ][ col ] & 0x0f ]; 
        
        add_round_key( round ); 
        inv_mix_columns( ); 
    } 
  
    inv_shift_rows( ); 
    
    for( byte row = 0; row < 4; ++row ) 
        for( byte col = 0; col < 4; ++col ) 
            state[ row ][ col ] = inv_sbox[ state[ row ][ col ] >> 4 ][ state[ row ][ col ] & 0x0f ]; 
  
    add_round_key( 0 ); 
} 
  
size_t AES::encrypt( char **data, size_t length, char *key ) 
{ 
    if( length == 0 ) 
        return 0; 
  
    if( !set_key( key ) ) 
        return 0; 
  
    size_t old_length = length; 
    while( length % 16 ) 
        ++length; 
    
    char *buffer = new char[ length ]; 
    memset( buffer, 0, length ); 
    memcpy( buffer, *data, old_length ); 
  
    delete [] *data; 
    *data = buffer; 
  
    size_t cur_block = 0; 
    
    do { 
        
        for( byte col = 0; col < 4; ++col ) 
            for( byte row = 0; row < 4; ++row ) 
                state[ row ][ col ] = static_cast< byte >( ( *data )[ cur_block * 16 + row + 4 * col ] ); 
  
        cipher( ); 
  
        for( byte col = 0; col < 4; ++col ) 
            for( byte row = 0; row < 4; ++row ) 
                ( *data )[ cur_block * 16 + row + 4 * col ] = static_cast< char >( state[ row ][ col ] ); 
  
        ++cur_block; 
    
    } while( cur_block * 16 != length ); 
  
    return cur_block; 
} 
  
size_t AES::decrypt( char **data, size_t length, char *key ) 
{ 
    if( length == 0 ) 
        return 0; 
  
    if( !set_key( key ) ) 
        return 0; 
  
    size_t old_length = length; 
    while( length % 16 ) 
        ++length; 
    
    char *buffer = new char[ length ]; 
    memset( buffer, 0, length ); 
    memcpy( buffer, *data, old_length ); 
  
    delete [] *data; 
    *data = buffer; 
  
    size_t cur_block = 0; 
    
    do { 
        
        for( byte col = 0; col < 4; ++col ) 
            for( byte row = 0; row < 4; ++row ) 
                state[ row ][ col ] = static_cast< byte >( ( *data )[ cur_block * 16 + row + 4 * col ] ); 
  
        inv_cipher( ); 
  
        for( byte col = 0; col < 4; ++col ) 
            for( byte row = 0; row < 4; ++row ) 
                ( *data )[ cur_block * 16 + row + 4 * col ] = static_cast< char >( state[ row ][ col ] ); 
  
        ++cur_block; 
    
    } while( cur_block * 16 != length ); 
  
    --length; 
    
    while( ! ( ( *buffer ) + length ) ) 
        --length; 
    
    ++length; 
    
    return length; 
}
