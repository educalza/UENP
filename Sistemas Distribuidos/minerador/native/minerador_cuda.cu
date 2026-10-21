/* Especialização gerada pelo host: DIGITOS, OFFSET_NONCE, BLOCOS e ESTADO_*.
 * Cada thread calcula exatamente o mesmo SHA256(SHA256(JSON)) do protocolo. */
typedef unsigned int u32;
typedef unsigned long long u64;

__device__ __constant__ u32 K[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

__device__ __forceinline__ u32 ror(u32 x, int n) {
    return (x >> n) | (x << (32 - n));
}

__device__ __forceinline__ void comprimir(u32 s[8], u32 w[16]) {
    u32 a=s[0], b=s[1], c=s[2], d=s[3], e=s[4], f=s[5], g=s[6], h=s[7];
    #pragma unroll
    for (int i=0; i<64; ++i) {
        if (i>=16) {
            u32 x=w[(i+1)&15], y=w[(i+14)&15];
            w[i&15] += (ror(x,7)^ror(x,18)^(x>>3)) + w[(i+9)&15]
                       + (ror(y,17)^ror(y,19)^(y>>10));
        }
        u32 t1=h+(ror(e,6)^ror(e,11)^ror(e,25))+((e&f)^(~e&g))+K[i]+w[i&15];
        u32 t2=(ror(a,2)^ror(a,13)^ror(a,22))+((a&b)^(a&c)^(b&c));
        h=g; g=f; f=e; e=d+t1; d=c; c=b; b=a; a=t1+t2;
    }
    s[0]+=a; s[1]+=b; s[2]+=c; s[3]+=d;
    s[4]+=e; s[5]+=f; s[6]+=g; s[7]+=h;
}

extern "C" __global__ void buscar(
    const u32 *__restrict__ modelo, u64 inicio, u64 passo,
    u32 quantidade, u32 dificuldade, u32 *saida
) {
    u32 indice=blockIdx.x*blockDim.x+threadIdx.x;
    if (indice>=quantidade) return;
    u64 nonce=inicio+(u64)indice*passo;
    enum { PRIMEIRA=OFFSET_NONCE/4, PALAVRAS=((OFFSET_NONCE%4)+DIGITOS+3)/4 };
    u32 remendo[PALAVRAS]={0};
    #pragma unroll
    for (int i=DIGITOS-1; i>=0; --i) {
        u32 byte=(u32)(nonce%10)+'0';
        nonce/=10;
        int pos=OFFSET_NONCE+i;
        remendo[pos/4-PRIMEIRA] |= byte << (24-8*(pos%4));
    }
    u32 s[8]={ESTADO_0,ESTADO_1,ESTADO_2,ESTADO_3,ESTADO_4,ESTADO_5,ESTADO_6,ESTADO_7};
    #if BLOCOS <= 8
    #pragma unroll
    #endif
    for (int bloco=0; bloco<BLOCOS; ++bloco) {
        u32 w[16];
        #pragma unroll
        for (int i=0; i<16; ++i) {
            int pos=bloco*16+i;
            w[i]=modelo[pos];
            if (pos>=PRIMEIRA && pos<PRIMEIRA+PALAVRAS) w[i] |= remendo[pos-PRIMEIRA];
        }
        comprimir(s,w);
    }
    u32 w[16];
    #pragma unroll
    for (int i=0; i<8; ++i) w[i]=s[i];
    w[8]=0x80000000;
    #pragma unroll
    for (int i=9; i<15; ++i) w[i]=0;
    w[15]=256;
    u32 segundo[8]={0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
                   0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19};
    comprimir(segundo,w);
    if (indice==0) {
        #pragma unroll
        for (int i=0; i<8; ++i) saida[i+1]=segundo[i];
    }
    unsigned int inteiras=dificuldade/8, resto=dificuldade%8;
    for (unsigned int i=0; i<inteiras; ++i) if (segundo[i]) return;
    if (resto && (segundo[inteiras] >> (32-resto*4))) return;
    atomicMin(saida,indice);
}
