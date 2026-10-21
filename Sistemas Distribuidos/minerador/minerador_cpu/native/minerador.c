/* ABI independente do Python. Usa a implementação SHA-256 do OpenSSL carregado
 * pelo Python; não implementa nem modifica o algoritmo criptográfico. */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#ifndef MINERADOR_SEM_SHA_DIRETO
#include <openssl/sha.h>
#include <openssl/opensslv.h>
typedef SHA256_CTX ContextoSHA;
#else
typedef struct contexto_sha_opaco ContextoSHA;
#endif

#ifdef _WIN32
#define API __declspec(dllexport)
#else
#define API __attribute__((visibility("default")))
#endif

typedef struct evp_md_ctx_st EVP_MD_CTX;
typedef struct evp_md_st EVP_MD;
typedef struct engine_st ENGINE;

typedef struct {
    EVP_MD_CTX *(*ctx_new)(void);
    void (*ctx_free)(EVP_MD_CTX *);
    int (*init)(EVP_MD_CTX *, const EVP_MD *, ENGINE *);
    int (*update)(EVP_MD_CTX *, const void *, size_t);
    int (*final)(EVP_MD_CTX *, unsigned char *, unsigned int *);
    const EVP_MD *(*sha256)(void);
    int (*direto_init)(ContextoSHA *);
    int (*direto_update)(ContextoSHA *, const void *, size_t);
    int (*direto_final)(unsigned char *, ContextoSHA *);
} Funcoes;

typedef struct {
    Funcoes f;
    EVP_MD_CTX *primeiro;
    EVP_MD_CTX *segundo;
    unsigned char *mensagem;
    unsigned char *sufixo;
    size_t prefixo_len;
    size_t sufixo_len;
    size_t digitos_anteriores;
    unsigned int direto;
#ifndef MINERADOR_SEM_SHA_DIRETO
    ContextoSHA base_primeiro;
    ContextoSHA base_segundo;
#endif
} Minerador;

typedef struct {
    uint64_t tentativas;
    uint64_t proximo_nonce;
    uint64_t nonce;
    unsigned char digest[32];
} Resultado;

API int minerador_abi(void) { return 2; }

/* A estrutura SHA256_CTX vem dos headers oficiais, sem presumir seu tamanho.
 * A via direta só é habilitada com a mesma versão principal do OpenSSL. */
API unsigned long minerador_openssl_major(void) {
#ifndef MINERADOR_SEM_SHA_DIRETO
    return OPENSSL_VERSION_NUMBER >> 28;
#else
    return 0;
#endif
}

API void minerador_destruir(Minerador *m) {
    if (!m) return;
    if (m->primeiro) m->f.ctx_free(m->primeiro);
    if (m->segundo) m->f.ctx_free(m->segundo);
    free(m->mensagem);
    free(m->sufixo);
    free(m);
}

API Minerador *minerador_criar(const Funcoes *f,
    const unsigned char *prefixo, size_t prefixo_len,
    const unsigned char *sufixo, size_t sufixo_len, unsigned int direto) {
    if (!f || !f->ctx_new || !f->ctx_free || !f->init || !f->update ||
        !f->final || !f->sha256 || direto > 1 || prefixo_len > SIZE_MAX - 20 ||
        sufixo_len > SIZE_MAX - prefixo_len - 20) return NULL;
#ifdef MINERADOR_SEM_SHA_DIRETO
    if (direto) return NULL;
#else
    if (direto && (!f->direto_init || !f->direto_update || !f->direto_final)) return NULL;
#endif
    Minerador *m = (Minerador *)calloc(1, sizeof(*m));
    if (!m) return NULL;
    m->f = *f;
    m->prefixo_len = prefixo_len;
    m->sufixo_len = sufixo_len;
    m->direto = direto;
    m->mensagem = (unsigned char *)malloc(prefixo_len + 20 + sufixo_len);
    m->sufixo = (unsigned char *)malloc(sufixo_len ? sufixo_len : 1);
    if (!m->mensagem || !m->sufixo) {
        minerador_destruir(m);
        return NULL;
    }
    memcpy(m->mensagem, prefixo, prefixo_len);
    memcpy(m->sufixo, sufixo, sufixo_len);
#ifndef MINERADOR_SEM_SHA_DIRETO
    if (direto) {
        if (!f->direto_init(&m->base_primeiro) ||
            !f->direto_update(&m->base_primeiro, prefixo, prefixo_len) ||
            !f->direto_init(&m->base_segundo)) {
            minerador_destruir(m);
            return NULL;
        }
        return m;
    }
#endif
    m->primeiro = f->ctx_new();
    m->segundo = f->ctx_new();
    if (!m->primeiro || !m->segundo ||
        !f->init(m->primeiro, f->sha256(), NULL) ||
        !f->init(m->segundo, f->sha256(), NULL)) {
        minerador_destruir(m);
        return NULL;
    }
    return m;
}

static size_t decimal(uint64_t nonce, unsigned char saida[20]) {
    unsigned char inverso[20];
    size_t n = 0;
    do {
        inverso[n++] = (unsigned char)('0' + nonce % 10);
        nonce /= 10;
    } while (nonce);
    for (size_t i = 0; i < n; ++i) saida[i] = inverso[n - i - 1];
    return n;
}

static int atende(const unsigned char digest[32], unsigned int dificuldade) {
    unsigned int completos = dificuldade / 2;
    for (unsigned int i = 0; i < completos; ++i)
        if (digest[i] != 0) return 0;
    return !(dificuldade & 1) || (digest[completos] & 0xf0) == 0;
}

/* Atualiza somente os dígitos afetados pela soma, sem converter novamente os
 * 19/20 dígitos do nonce a cada tentativa. Crescimentos de largura e wraps
 * usam a conversão completa, que também trata o limite de uint64_t. */
static int somar_decimal(unsigned char texto[20], size_t tamanho, uint64_t passo) {
    while (tamanho && passo) {
        unsigned int soma = (unsigned int)(texto[--tamanho] - '0') + (unsigned int)(passo % 10);
        passo = passo / 10 + soma / 10;
        texto[tamanho] = (unsigned char)('0' + soma % 10);
    }
    return passo == 0;
}

static int hash_duplo(Minerador *m, size_t digitos, unsigned char digest[32]) {
    unsigned char primeiro[32];
#ifndef MINERADOR_SEM_SHA_DIRETO
    if (m->direto) {
        ContextoSHA a = m->base_primeiro;
        ContextoSHA b = m->base_segundo;
        return m->f.direto_update(&a, m->mensagem + m->prefixo_len, digitos + m->sufixo_len) &&
            m->f.direto_final(primeiro, &a) &&
            m->f.direto_update(&b, primeiro, 32) && m->f.direto_final(digest, &b);
    }
#endif
    unsigned int tamanho = 0;
    return m->f.init(m->primeiro, NULL, NULL) &&
        m->f.update(m->primeiro, m->mensagem, m->prefixo_len + digitos + m->sufixo_len) &&
        m->f.final(m->primeiro, primeiro, &tamanho) && tamanho == 32 &&
        m->f.init(m->segundo, NULL, NULL) && m->f.update(m->segundo, primeiro, 32) &&
        m->f.final(m->segundo, digest, &tamanho) && tamanho == 32;
}

/* limite == 0 representa 2**64, com overflow unsigned definido pelo C. */
API int minerador_buscar(Minerador *m, uint64_t nonce, uint64_t passo,
    uint64_t limite, uint32_t lote, unsigned int dificuldade, Resultado *r) {
    if (!m || !r || !passo || dificuldade > 64 ||
        (limite && (passo > limite || nonce >= limite))) return -1;
    memset(r, 0, sizeof(*r));
    r->proximo_nonce = nonce;
    unsigned char texto[20];
    size_t digitos = decimal(nonce, texto);
    for (uint32_t i = 0; i < lote; ++i) {
        unsigned char digest[32];
        if (digitos != m->digitos_anteriores) {
            memcpy(m->mensagem + m->prefixo_len + digitos, m->sufixo, m->sufixo_len);
            m->digitos_anteriores = digitos;
        }
        memcpy(m->mensagem + m->prefixo_len, texto, digitos);
        if (!hash_duplo(m, digitos, digest)) return -1;
        r->tentativas++;
        uint64_t proximo = limite && nonce >= limite - passo
            ? nonce - (limite - passo) : nonce + passo;
        r->proximo_nonce = proximo;
        if (atende(digest, dificuldade)) {
            r->nonce = nonce;
            memcpy(r->digest, digest, 32);
            return 1;
        }
        if (proximo < nonce || !somar_decimal(texto, digitos, passo))
            digitos = decimal(proximo, texto);
        nonce = proximo;
    }
    return 0;
}
